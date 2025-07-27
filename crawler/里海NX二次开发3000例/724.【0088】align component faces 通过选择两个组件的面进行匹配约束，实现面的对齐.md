### 【0088】align component faces 通过选择两个组件的面进行匹配约束，实现面的对齐

#### 代码

```cpp
    /*HEAD ALIGN_COMPONENT_FACES CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18中新增的函数。 */  
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
    static int mask_for_planar_face(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_planar_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_planar_face, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
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
            to_occ;  
        UF_ASSEM_mating_condition_t  
            mc_data;  
        UF_ASSEM_mc_structure_state_t  
            struct_status;  
        UF_ASSEM_mc_status_t  
            status;  
        while (((from_occ = select_planar_face(  
                    "Select face FROM component to be mated")) != NULL_TAG) &&  
               ((to_occ = select_planar_face(  
                    "Select face TO align with")) != NULL_TAG))  
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
            mc_data.constraints[0].from_type = UF_ASSEM_planar_face;  
            mc_data.constraints[0].to_type = UF_ASSEM_planar_face;  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了用于平面面选择的函数select_planar_face，它会弹出一个对话框让用户选择平面面，并返回该平面的标签。
> 3. 定义了报告配对状态信息的函数report_mc_status。
> 4. 在do_it函数中，首先让用户选择两个平面，然后创建一个配对条件对象mc_data，设置两个平面的配对类型为对齐，然后调用UF_ASSEM_apply_mc_data函数应用这个配对条件。
> 5. 在ufusr函数中，首先初始化UF，然后调用do_it函数，最后终止UF。
> 6. ufusr_ask_unload函数用于卸载用户自定义函数。
>
> 总体来说，这段代码通过交互方式让用户选择两个平面，然后设置它们之间的对齐配对关系，实现了组件间的对齐。
>
