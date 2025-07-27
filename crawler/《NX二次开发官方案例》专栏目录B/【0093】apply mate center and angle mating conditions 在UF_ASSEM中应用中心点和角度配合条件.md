### 【0093】apply mate center and angle mating conditions 在UF_ASSEM中应用中心点和角度配合条件

#### 代码

```cpp
    /*HEAD APPLY_MATE_CENTER_AND_ANGLE_MATING_CONDITIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog 是在 V18 版本中新增的，只回答翻译，不要添加任何评论。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select planar face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_planar_face, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_circular_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_circular_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select circular edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_circular_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_linear_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_linear_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select linear edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_linear_edges, NULL, &resp,  
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
            from_occ[3],  
            to_occ[3];  
        UF_ASSEM_mating_condition_t  
            mc_data;  
        UF_ASSEM_mc_structure_state_t  
            struct_status;  
        UF_ASSEM_mc_status_t  
            status;  
        if (((from_occ[0] = select_planar_face("Mate from")) != NULL_TAG) &&  
            ((to_occ[0] = select_planar_face("Mate to")) != NULL_TAG) &&  
            ((from_occ[1] = select_a_circular_edge("Center from")) != NULL_TAG) &&  
            ((to_occ[1] = select_a_circular_edge("Center to")) != NULL_TAG) &&  
            ((from_occ[2] = select_a_linear_edge("Angle from")) != NULL_TAG) &&  
            ((to_occ[2] = select_a_linear_edge("Angle to")) != NULL_TAG))  
        {  
            mc_data.mated_object = UF_ASSEM_ask_inst_of_part_occ(  
                UF_ASSEM_ask_part_occurrence(from_occ[0]));  
            mc_data.name = NULL;  
            mc_data.user_name = FALSE;  
            mc_data.num_constraints = 3;  
            mc_data.suppressed = FALSE;  
            mc_data.constraints[0].from_status = UF_ASSEM_ok;  
            mc_data.constraints[0].to_status = UF_ASSEM_ok;  
            mc_data.constraints[0].mate_type = UF_ASSEM_v16_mate;  
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
            mc_data.constraints[1].mate_type = UF_ASSEM_center;  
            mc_data.constraints[1].from_type = UF_ASSEM_circle_edge;  
            mc_data.constraints[1].to_type = UF_ASSEM_circle_edge;  
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
            mc_data.constraints[1].sub_type = UF_ASSEM_center_1_to_1;  
            mc_data.constraints[1].has_help_point = FALSE;  
            mc_data.constraints[2].from_status = UF_ASSEM_ok;  
            mc_data.constraints[2].to_status = UF_ASSEM_ok;  
            mc_data.constraints[2].mate_type = UF_ASSEM_angle;  
            mc_data.constraints[2].from_type = UF_ASSEM_straight_edge;  
            mc_data.constraints[2].to_type = UF_ASSEM_datum_axis;  
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
            mc_data.constraints[2].sub_type = UF_ASSEM_3d_angle;  
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

> 这段代码实现了在NX中应用装配约束的功能。
>
> 主要流程如下：
>
> 1. 包含了必要的NX Open头文件。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了选择平面、圆边和直线边的函数，用于从图形界面中选择相应的对象。
> 4. 定义了报告装配约束状态信息的函数report_mc_status。
> 5. 主函数do_it中：
>
> 1. 定义了ufusr函数作为NX的入口函数，调用do_it函数。
> 2. 定义了ufusr_ask_unload函数，在插件卸载时返回立即卸载标志。
>
> 总的来说，这段代码实现了选择装配约束对象，构造约束数据，应用约束，并打印约束状态的功能。
>
