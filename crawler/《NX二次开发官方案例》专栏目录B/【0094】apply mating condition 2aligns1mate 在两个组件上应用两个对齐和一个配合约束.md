### 【0094】apply mating condition 2aligns1mate 在两个组件上应用两个对齐和一个配合约束

#### 代码

```cpp
    /*HEAD APPLY_MATING_CONDITION_2ALIGNS1MATE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据提供的信息，UF_print_syslog是V18版本中新增的功能。 */  
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
    /* 里海 */  
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
            from_occ[3],  
            to_occ[3];  
        UF_ASSEM_mating_condition_t  
            mc_data;  
        UF_ASSEM_mc_structure_state_t  
            struct_status;  
        UF_ASSEM_mc_status_t  
            status;  
    /*  New function - Required in NX 译:新功能 - NX中必须

翻译结果为: New function - Required in NX */  
        UF_CALL(UF_ASSEM_init_mc(&mc_data));  
        while (((from_occ[0] = select_planar_face(  
                    "Select face FROM component to be mated")) != NULL_TAG) &&  
               ((to_occ[0] = select_planar_face(  
                    "Select face TO align with")) != NULL_TAG) &&  
               ((from_occ[1] = select_planar_face(  
                    "Select face FROM component to be mated")) != NULL_TAG) &&  
               ((to_occ[1] = select_planar_face(  
                    "Select face TO align with")) != NULL_TAG) &&  
               ((from_occ[2] = select_planar_face(  
                    "Select face FROM component to be mated")) != NULL_TAG) &&  
               ((to_occ[2] = select_planar_face(  
                    "Select face TO mate to")) != NULL_TAG))  
        {  
            mc_data.mated_object = UF_ASSEM_ask_inst_of_part_occ(  
                UF_ASSEM_ask_part_occurrence(from_occ[0]));  
            mc_data.name = NULL;  
            mc_data.user_name = FALSE;  
            mc_data.num_constraints = 3;  
            mc_data.suppressed = FALSE;  
            mc_data.constraints[0].from_status = UF_ASSEM_ok;  
            mc_data.constraints[0].to_status = UF_ASSEM_ok;  
            mc_data.constraints[0].mate_type = UF_ASSEM_v16_align;  
            mc_data.constraints[0].from_type = UF_ASSEM_planar_face;  
            mc_data.constraints[0].to_type = UF_ASSEM_planar_face;  
            mc_data.constraints[0].from =  
                UF_ASSEM_ask_prototype_of_occ(from_occ[0]);  
            mc_data.constraints[0].from_part_occ =  
                UF_ASSEM_ask_part_occurrence(from_occ[0]);  
            mc_data.constraints[0].to = UF_ASSEM_ask_prototype_of_occ(to_occ[0]);  
            mc_data.constraints[0].to_part_occ =  
                UF_ASSEM_ask_part_occurrence(to_occ[0]);  
            mc_data.constraints[0].offset = NULL_TAG;  
            mc_data.constraints[0].name = NULL;  
            mc_data.constraints[0].user_name = FALSE;  
            mc_data.constraints[0].has_help_point = FALSE;  
            mc_data.constraints[1].from_status = UF_ASSEM_ok;  
            mc_data.constraints[1].to_status = UF_ASSEM_ok;  
            mc_data.constraints[1].mate_type = UF_ASSEM_v16_align;  
            mc_data.constraints[1].from_type = UF_ASSEM_planar_face;  
            mc_data.constraints[1].to_type = UF_ASSEM_planar_face;  
            mc_data.constraints[1].from =  
                UF_ASSEM_ask_prototype_of_occ(from_occ[1]);  
            mc_data.constraints[1].from_part_occ =  
                UF_ASSEM_ask_part_occurrence(from_occ[1]);  
            mc_data.constraints[1].to = UF_ASSEM_ask_prototype_of_occ(to_occ[1]);  
            mc_data.constraints[1].to_part_occ =  
                UF_ASSEM_ask_part_occurrence(to_occ[1]);  
            mc_data.constraints[1].offset = NULL_TAG;  
            mc_data.constraints[1].name = NULL;  
            mc_data.constraints[1].user_name = FALSE;  
            mc_data.constraints[1].has_help_point = FALSE;  
            mc_data.constraints[2].from_status = UF_ASSEM_ok;  
            mc_data.constraints[2].to_status = UF_ASSEM_ok;  
            mc_data.constraints[2].mate_type = UF_ASSEM_v16_mate;  
            mc_data.constraints[2].from_type = UF_ASSEM_planar_face;  
            mc_data.constraints[2].to_type = UF_ASSEM_planar_face;  
            mc_data.constraints[2].from =  
                UF_ASSEM_ask_prototype_of_occ(from_occ[2]);  
            mc_data.constraints[2].from_part_occ =  
                UF_ASSEM_ask_part_occurrence(from_occ[2]);  
            mc_data.constraints[2].to = UF_ASSEM_ask_prototype_of_occ(to_occ[2]);  
            mc_data.constraints[2].to_part_occ =  
                UF_ASSEM_ask_part_occurrence(to_occ[2]);  
            mc_data.constraints[2].offset = NULL_TAG;  
            mc_data.constraints[2].name = NULL;  
            mc_data.constraints[2].user_name = FALSE;  
            mc_data.constraints[2].has_help_point = FALSE;  
            UF_CALL(UF_ASSEM_apply_mc_data(&mc_data, &struct_status, &status));  
            report_mc_status(status);  
            UF_CALL(UF_MODL_update());  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX的二次开发程序，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用的错误信息。
> 2. 定义了一个选择平面面的函数select_planar_face，它使用UF_UI来让用户选择平面面，并返回面的tag。
> 3. 定义了一个报告配合状态函数report_mc_status，用于打印配合求解的结果状态。
> 4. 定义了一个主函数do_it，在该函数中，通过循环调用select_planar_face，让用户选择3个要配合的面，并初始化配合数据结构UF_ASSEM_mating_condition_t，设置3个配合约束(2个对齐配合，1个配合)，然后调用UF_ASSEM_apply_mc_data来应用配合，并打印配合求解状态。
> 5. 定义了ufusr函数，这是NX二次开发程序的入口函数，在其中初始化UF，调用do_it函数，然后终止UF。
> 6. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 总的来说，这段代码实现了让用户选择3个面，并设置2个对齐配合和1个配合，然后求解配合，并打印配合求解状态的功能。
>
