### 【0809】create label to intersection of two objects 创建两个对象交点处的标签

#### 代码

```cpp
    /*HEAD CREATE_LABEL_TO_INTERSECTION_OF_TWO_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：UF_print_syslog是在V18版本中新增的。 */  
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
    static int mask_for_drf_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[7] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_solid_silhouette_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 7, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_drf_curve_in_view(char *prompt, tag_t *view)  
    {  
        int  
            resp;  
        tag_t  
            object;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_curves, NULL, &resp,  
            &object, cp, view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        tag_t  
            curve1,  
            curve2,  
            label;  
        double  
            cp[3] = { 0,0,0 },  
            pos[3];  
        char  
            label_text[1][MAX_LINE_LENGTH + 1];  
        UF_DRF_object_t  
            curve;  
        UF_CALL(UF_DRF_init_object_structure(&curve));  
        while (((curve1 = select_drf_curve_in_view(  
                "Label intersection - Select first object",  
                    &curve.object_view_tag)) != NULL_TAG) &&  
               ((curve2 = select_drf_curve_in_view(  
                "Label intersection - Select first object",  
                    &curve.object_view_tag)) != NULL_TAG) &&  
               (select_pos("Label - Indicate origin", pos)))  
        {  
            if (!UF_CALL(UF_SO_create_point_two_curves(curve1,  
                UF_SO_update_after_modeling, curve1, curve2, NULL_TAG, NULL_TAG,  
                &curve.object_tag)))  
            {  
                sprintf(&label_text[0][0], "LABEL %d", ii++);  
                UF_CALL(UF_DRF_create_label(1, label_text, pos,  
                    UF_DRF_leader_attach_object, &curve, cp, &label));  
            }  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码的功能是：
>
> 1. 选择两个曲线对象，并在它们相交的位置创建一个标签。
> 2. 用户交互式地选择两个曲线对象，选择位置后，代码在两个曲线的交点处创建一个标签。
> 3. 标签文本为"LABEL x"，其中x为序号，每次创建标签时递增。
> 4. 使用UF_DRF_create_label函数创建标签，标签附着在曲线交点上。
> 5. 使用UF_SO_create_point_two_curves函数计算两个曲线的交点。
> 6. 通过自定义的错误报告函数，在出错时显示错误信息。
> 7. 定义了选择曲线和输入位置点的函数。
> 8. 在UFusr函数中初始化和终止NX，并调用主函数do_it。
> 9. ufusr_ask_unload函数指定了立即卸载二次开发程序。
>
> 总体来说，这段代码实现了在NX中通过用户交互选择曲线，并在它们的交点处添加标签的功能。代码结构清晰，包含了错误处理和用户交互。
>
