### 【0611】create associative points at equal arclength percentages along curve 在曲线上等弧长百分比处创建相关点

#### 代码

```cpp
    /*HEAD CREATE_ASSOCIATIVE_POINTS_AT_EQUAL_ARCLENGTH_PERCENTAGES_ALONG_CURVE CCC UFUN */  
    /*  
        When interactively editing the paramters of these points they may  
        flip to be offset off the curve in the other direction.  
        See PR 5861542.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
    #include <uf_point.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中的新增功能。 */  
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
    static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve_or_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve or edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void create_associative_points_spaced_along_curve(tag_t curve,  
        double inc_percent)  
    {  
        tag_t  
            exp_tag,  
            from_point,  
            offset,  
            point_feat;  
        double  
            percent;  
        char  
            exp_str[30];  
        set_undo_mark("Create points on curve");  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_dont_update, 0.0, &offset));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_within_modeling,  
            curve, offset, &from_point));  
        for (percent = 0; percent <= 100; percent = percent+inc_percent)  
        {  
            sprintf(exp_str, "%f", percent);  
            strip_trailing_zeros(exp_str);  
            UF_CALL(UF_MODL_create_exp_tag(exp_str, &exp_tag));  
            UF_CALL(UF_SO_create_scalar_exp(curve, UF_SO_update_within_modeling,  
                exp_tag, &offset));  
            UF_CALL(UF_POINT_create_along_curve(curve, from_point, offset,  
                UF_SO_point_along_curve_percent, FALSE, &point_feat));  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve;  
        double  
            ip = 10.0;  
        while (((curve = select_a_curve_or_edge("Create points along")) != NULL_TAG)  
            && prompt_for_a_number("Incremental", "Percentage", &ip))  
            create_associative_points_spaced_along_curve(curve, ip);  
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

> 这段代码是用于在NX中沿曲线创建等间隔的点的二次开发代码。
>
> 主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于打印错误信息。
> 2. 定义了mask_for_curves_and_edges函数，用于设置选择过滤，仅允许选择线、圆、圆锥、样条线等曲线。
> 3. 定义了select_a_curve_or_edge函数，用于弹出对话框，让用户选择一个曲线或边，并高亮显示。
> 4. 定义了prompt_for_a_number函数，用于弹出对话框，让用户输入百分比增量。
> 5. 定义了strip_trailing_zeros函数，用于去除字符串尾部的零。
> 6. 定义了set_undo_mark函数，用于设置撤销标记点。
> 7. 定义了create_associative_points_spaced_along_curve函数，用于沿指定曲线，按照指定百分比增量创建等间隔的点。
> 8. 定义了do_it函数，用于循环让用户选择曲线并输入百分比增量，然后调用函数创建点。
> 9. 定义了ufusr函数，用于初始化NX，调用do_it函数，然后终止NX。
> 10. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 通过这些函数，该代码实现了在NX中沿曲线创建等间隔点的功能，用户可以交互式地选择曲线并设置百分比增量，非常实用。
>
