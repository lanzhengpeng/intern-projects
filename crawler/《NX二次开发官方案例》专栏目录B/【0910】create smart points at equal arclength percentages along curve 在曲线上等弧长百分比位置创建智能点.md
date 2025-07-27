### 【0910】create smart points at equal arclength percentages along curve 在曲线上等弧长百分比位置创建智能点

#### 代码

```cpp
    /*HEAD CREATE_SMART_POINTS_AT_EQUAL_ARCLENGTH_PERCENTAGES_ALONG_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新增功能，它用于打印系统日志。在文档中提到，对于该功能只需进行翻译，不需要添加任何额外内容。 */  
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
    /* qq3123197280 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
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
    static void create_smart_points_spaced_along_curve(tag_t curve, double inc_percent)  
    {  
        tag_t  
            last,  
            offset,  
            start;  
        double  
            percent;  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.0, &start));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            inc_percent/100, &offset));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
            curve, start, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
        for (percent = inc_percent; percent <= 100; percent = percent+inc_percent)  
        {  
            UF_CALL(UF_SO_create_point_along_curve(curve,  
                UF_SO_update_after_modeling, curve, last, offset,  
                UF_SO_point_along_curve_percent, FALSE, &last));  
            UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve;  
        double  
            ip = 10.0;  
        while (((curve = select_a_curve("Create points along")) != NULL_TAG)  
            && prompt_for_a_number("Incremental", "Percentage", &ip))  
            create_smart_points_spaced_along_curve(curve, ip);  
    }  
    /* qq3123197280 */  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在发生错误时输出错误信息和调用堆栈。
> 2. 曲线选择掩码：定义了曲线选择掩码函数mask_for_curves，用于在对话框中仅允许选择曲线类型。
> 3. 曲线选择函数：定义了一个曲线选择函数select_a_curve，用于通过对话框选择一个曲线对象。
> 4. 数值输入提示：定义了一个数值输入提示函数prompt_for_a_number，用于提示用户输入一个数值。
> 5. 沿曲线等间距创建智能点：定义了一个沿曲线等间距创建智能点的函数create_smart_points_spaced_along_curve，用于在曲线的等间距百分比位置创建智能点。
> 6. 主函数：在主函数do_it中，通过循环调用select_a_curve和prompt_for_a_number，提示用户选择曲线并输入百分比间距，然后调用create_smart_points_spaced_along_curve在曲线上创建等间距的智能点。
> 7. NX初始化和终止：在ufusr函数中，对NX进行初始化，调用do_it，然后终止NX。
> 8. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于在卸载时立即终止NX。
>
> 总的来说，这段代码实现了在NX中通过对话框选择曲线，并输入百分比间距，在曲线上创建等间距的智能点的功能。
>
