### 【2867】show curve ends 展示曲线端点

#### 代码

```cpp
    /*HEAD SHOW_CURVE_ENDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_eval.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            limits[2];  
        UF_EVAL_p_t  
            evaluator;  
        UF_CALL(UF_EVAL_initialize(curve, &evaluator));  
        UF_CALL(UF_EVAL_ask_limits(evaluator, limits));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[0], start, NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[1], end, NULL));  
        UF_CALL(UF_EVAL_free(evaluator));  
    }  
    /* qq3123197280 */  
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
        UF_CALL(UF_UI_set_cursor_view(0));  
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
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve;  
        double  
            end[3],  
            start[3];  
        while ((curve = select_a_curve_or_edge("Show start & end")) != NULL_TAG)  
        {  
            ask_curve_ends(curve, start, end);  
            display_temporary_colored_point(start, UF_OBJ_GREEN);  
            display_temporary_colored_point(end, UF_OBJ_RED);  
        }  
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

> 这是段用于NX的二次开发代码，主要功能是显示曲线或边缘的起点和终点。以下是代码的详细描述：
>
> 1. 代码结构：包含了标准头文件，如stdio.h、string.h等。包含了NX API头文件，如uf.h、uf_ui.h等。定义了ECHO和UF_CALL宏，用于打印日志和错误报告。定义了report_error函数，用于处理错误。定义了ask_curve_ends函数，用于获取曲线的起点和终点。定义了mask_for_curves_and_edges函数，用于设置选择掩码。定义了select_a_curve_or_edge函数，用于选择曲线或边缘。定义了display_temporary_colored_point函数，用于显示临时彩色点。定义了do_it函数，用于主逻辑。定义了ufusr函数，作为程序的入口。定义了ufusr_ask_unload函数，用于卸载。
> 2. 包含了标准头文件，如stdio.h、string.h等。
> 3. 包含了NX API头文件，如uf.h、uf_ui.h等。
> 4. 定义了ECHO和UF_CALL宏，用于打印日志和错误报告。
> 5. 定义了report_error函数，用于处理错误。
> 6. 定义了ask_curve_ends函数，用于获取曲线的起点和终点。
> 7. 定义了mask_for_curves_and_edges函数，用于设置选择掩码。
> 8. 定义了select_a_curve_or_edge函数，用于选择曲线或边缘。
> 9. 定义了display_temporary_colored_point函数，用于显示临时彩色点。
> 10. 定义了do_it函数，用于主逻辑。
> 11. 定义了ufusr函数，作为程序的入口。
> 12. 定义了ufusr_ask_unload函数，用于卸载。
> 13. 主要逻辑：使用ask_curve_ends函数获取曲线的起点和终点。使用display_temporary_colored_point函数显示曲线的起点和终点。使用循环让用户选择曲线或边缘，并显示它们的起点和终点。
> 14. 使用ask_curve_ends函数获取曲线的起点和终点。
> 15. 使用display_temporary_colored_point函数显示曲线的起点和终点。
> 16. 使用循环让用户选择曲线或边缘，并显示它们的起点和终点。
> 17. 关键函数：ask_curve_ends：使用NX的评估器获取曲线的起点和终点。select_a_curve_or_edge：使用NX的UI函数让用户选择曲线或边缘。display_temporary_colored_point：使用NX的显示函数显示彩色点。
> 18. ask_curve_ends：使用NX的评估器获取曲线的起点和终点。
> 19. select_a_curve_or_edge：使用NX的UI函数让用户选择曲线或边缘。
> 20. display_temporary_colored_point：使用NX的显示函数显示彩色点。
> 21. 注意事项：需要正确设置选择掩码。需要处理错误。需要释放评估器。需要初始化和终止NX。
> 22. 需要正确设置选择掩码。
> 23. 需要处理错误。
> 24. 需要释放评估器。
> 25. 需要初始化和终止NX。
> 26. 使用方法：编译成NX的UF插件。在NX中加载并运行。选择曲线或边缘，即可看到它们的起点和终点。
> 27. 编译成NX的UF插件。
> 28. 在NX中加载并运行。
> 29. 选择曲线或边缘，即可看到它们的起点和终点。
>
> 这段代码实现了在NX中显示曲线或边缘的起点和终点的功能，具有实际应用价值。
>
