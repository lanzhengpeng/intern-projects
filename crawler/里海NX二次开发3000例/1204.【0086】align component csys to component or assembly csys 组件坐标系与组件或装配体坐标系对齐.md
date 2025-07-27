### 【0086】align component csys to component or assembly csys 组件坐标系与组件或装配体坐标系对齐

#### 代码

```cpp
    /*HEAD ALIGN_COMPONENT_CSYS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static int mask_for_csys(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_coordinate_system_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_csys(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_mc_status(UF_ASSEM_mc_status_t stat)  
    {  
         switch(stat)  
         {  
           case UF_ASSEM_mc_solved:  
             printf("UF_ASSEM_mc_solve\n");  
             break;  
           case UF_ASSEM_mc_not_solved:  
             printf("UF_ASSEM_mc_NOT_solved\n");  
             break;  
           case UF_ASSEM_mc_no_mating_specified:  
             printf("UF_ASSEM_mc_no_mating_specified\n");  
             break;  
           default:  
             printf("Unknown status came back\n");  
         }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            from_occ,  
            to_occ,  
            invalid_tag,  
            work_part = UF_ASSEM_ask_work_part();  
        UF_ASSEM_mating_condition_t  
            mc_data;  
        UF_ASSEM_mc_structure_state_t  
            struct_status;  
        UF_ASSEM_mc_status_t  
            status;  
        /*  New function - Required in NX 译:新功能 - NX必备 */  
        UF_CALL(UF_ASSEM_init_mc(&mc_data));  
        while (((from_occ = select_a_csys(  
                    "Select csys FROM component to be mated")) != NULL_TAG) &&  
               ((to_occ = select_a_csys(  
                    "Select csys TO align with")) != NULL_TAG))  
        {  
            mc_data.mated_object = UF_ASSEM_ask_inst_of_part_occ(  
                UF_ASSEM_ask_part_occurrence(from_occ));  
            mc_data.name = NULL;  
            mc_data.user_name = FALSE;  
            mc_data.num_constraints = 1;  
            mc_data.suppressed = FALSE;  
            mc_data.constraints[0].from_status = UF_ASSEM_ok;  
            mc_data.constraints[0].to_status = UF_ASSEM_ok;  
            mc_data.constraints[0].mate_type = UF_ASSEM_v16_align;  
            mc_data.constraints[0].from_type = UF_ASSEM_csys;  
            mc_data.constraints[0].to_type = UF_ASSEM_csys;  
            mc_data.constraints[0].from =  
                UF_ASSEM_ask_prototype_of_occ(from_occ);  
            mc_data.constraints[0].from_part_occ =  
                UF_ASSEM_ask_part_occurrence(from_occ);  
            // Check whether TO object is in component or in assembly work part  
            if (UF_ASSEM_is_occurrence(to_occ))  
            {  
                mc_data.constraints[0].to = UF_ASSEM_ask_prototype_of_occ(to_occ);  
                mc_data.constraints[0].to_part_occ =  
                    UF_ASSEM_ask_part_occurrence(to_occ);  
            }  
            else  
            {  
                mc_data.constraints[0].to = to_occ;  
                mc_data.constraints[0].to_part_occ =  
                    UF_ASSEM_ask_root_part_occ(UF_ASSEM_ask_work_part());  
            }  
            mc_data.constraints[0].offset = NULL_TAG;  
            mc_data.constraints[0].name = NULL;  
            mc_data.constraints[0].user_name = FALSE;  
            mc_data.constraints[0].has_help_point = FALSE;  
            mc_data.part_occurrence = work_part;  
            UF_CALL( UF_ASSEM_check_mc_structure ( &mc_data, &struct_status, &invalid_tag ) );  
            UF_CALL(UF_ASSEM_apply_mc_data(&mc_data, &struct_status, &status));  
            report_mc_status(status);  
            UF_CALL(UF_MODL_update());  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码的功能是让用户选择两个坐标系，然后建立这两个坐标系之间的对齐约束。
>
> 主要功能包括：
>
> 1. 坐标系选择：用户通过对话框选择两个坐标系，一个作为源坐标系，另一个作为目标坐标系。
> 2. 建立对齐约束：代码会建立源坐标系和目标坐标系之间的对齐约束，并将约束应用到装配体中。
> 3. 更新模型：应用约束后，代码会调用UF_MODL_update()函数来更新模型显示。
> 4. 错误处理：通过自定义的report_error函数，代码会捕获并打印UF函数的返回码，方便调试。
> 5. 坐标系过滤：通过自定义的mask_for_csys函数，代码会过滤出坐标系类型的选择，确保用户只能选择坐标系。
> 6. 提示信息：通过自定义的report_mc_status函数，代码会根据约束求解结果打印提示信息。
> 7. NX版本要求：代码使用了NX10.0版本新增的UF_ASSEM系列函数，因此需要NX10.0或以上版本才能正常运行。
> 8. 加载和卸载：通过ufusr和ufusr_ask_unload函数，代码实现了在NX中的加载和卸载。
>
> 综上所述，该代码实现了在NX中通过对话框选择坐标系并建立对齐约束的功能，为用户提供了便利。
>
