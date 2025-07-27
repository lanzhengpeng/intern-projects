### 【0087】align component csys 对齐组件坐标系

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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog在V18中是新增的，只需提供译文，不要添加任何其他内容。 */  
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
        /*  New function - Required in NX 译:New function - Required in NX */  
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
            mc_data.constraints[0].mate_type = UF_ASSEM_align;  
            mc_data.constraints[0].from_type = UF_ASSEM_csys;  
            mc_data.constraints[0].to_type = UF_ASSEM_csys;  
            mc_data.constraints[0].from =  
                UF_ASSEM_ask_prototype_of_occ(from_occ);  
            mc_data.constraints[0].from_part_occ =  
                UF_ASSEM_ask_part_occurrence(from_occ);  
            mc_data.constraints[0].to = UF_ASSEM_ask_prototype_of_occ(to_occ);  
            mc_data.constraints[0].to_part_occ =  
                UF_ASSEM_ask_part_occurrence(to_occ);  
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

> 这段代码是用于NX的二次开发，其主要功能是创建组件之间的对齐约束。代码的关键点包括：
>
> 1. 定义错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义掩码函数mask_for_csys，用于在对话框中选择坐标系类型的对象。
> 3. 定义选择函数select_a_csys，用于弹出对话框让用户选择一个坐标系对象。
> 4. 定义报告函数report_mc_status，用于打印对齐约束的状态。
> 5. 定义主函数do_it，其中通过循环调用select_a_csys让用户选择源坐标系和目标坐标系，然后创建对齐约束并应用。
> 6. 定义ufusr函数，用于在NX启动时调用do_it函数。
> 7. 定义卸载函数ufusr_ask_unload，用于立即卸载该程序。
>
> 总的来说，这段代码实现了NX组件之间的对齐约束创建功能，用户可以通过选择源组件和目标组件的坐标系来创建对齐约束。
>
