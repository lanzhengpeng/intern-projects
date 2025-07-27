### 【0420】ask curves intersection 询问曲线交点

#### 代码

```cpp
    /*HEAD ASK_CURVES_INTERSECTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是 V18 版本中新增的函数。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
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
        int  
            irc;  
        tag_t  
            one,  
            two;  
        double  
            dist,  
            on_one[3],  
            on_two[3],  
            one_p,  
            two_p;  
        UF_CURVE_intersect_info_t  
            out_info;  
        while (((one = select_a_curve_or_edge("1st Intersect")) != NULL_TAG)  
            && ((two = select_a_curve_or_edge("2nd Intersect")) != NULL_TAG))  
        {  
            if (!UF_CALL(UF_MODL_ask_minimum_dist(one, two, FALSE, NULL, FALSE,  
                NULL, &dist, on_one, on_two)))  
            {  
                WRITE_F(dist);  
                WRITE3F(on_one);  
                WRITE3F(on_two);  
                display_temporary_colored_point(on_one, UF_OBJ_YELLOW);  
                display_temporary_colored_point(on_two, UF_OBJ_CYAN);  
            }  
            FTN(uf5930)(&one, &two, on_two, &one_p, &two_p, on_one, &irc);  
            WRITE_D(irc);  
            if ((irc == 1) || (irc == 2))  
            {  
                WRITE_F(one_p);  
                WRITE_F(two_p);  
                WRITE3F(on_one);  
                display_temporary_colored_point(on_one, UF_OBJ_RED);  
            }  
            if (!UF_CALL(UF_CURVE_intersect(one, two, on_two, &out_info)))  
            {  
                WRITE_D(out_info.type_of_intersection);  
                if (out_info.type_of_intersection > 0)  
                {  
                    display_temporary_colored_point(out_info.curve_point,  
                        UF_OBJ_GREEN);  
                    WRITE3F(out_info.curve_point);  
                    WRITE_F(out_info.curve_parm);  
                    WRITE_F(out_info.entity_parms[0]);  
                }  
            }  
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

> 这段代码是一个NX二次开发示例，主要实现以下功能：
>
> 1. 错误报告函数：定义了report_error函数，用于在发生UF函数调用错误时报告错误信息。
> 2. 输出函数：定义了write_integer_to_listing_window和write_double_to_listing_window函数，用于将整数和浮点数输出到列表窗口。
> 3. 曲线和边选择：定义了mask_for_curves_and_edges函数，用于设置选择掩码，以选择曲线和边。
> 4. 选择函数：定义了select_a_curve_or_edge函数，用于提示用户选择一个曲线或边，并返回选择对象的tag。
> 5. 显示点函数：定义了display_temporary_colored_point函数，用于在指定位置显示一个临时彩色点。
> 6. 主函数：在do_it函数中，通过循环让用户选择两个曲线或边，然后计算并输出它们的最小距离，以及在第一个曲线上对应的参数值。同时，使用UF_CURVE_intersect函数计算两个曲线的交点信息并输出。
> 7. UFusr函数：定义了标准的ufusr函数，用于初始化和调用主函数。
> 8. 卸载函数：定义了ufusr_ask_unload函数，用于在卸载时立即终止。
>
> 总体来看，这段代码通过UF函数实现曲线和边的交互操作，包括选择、计算距离和交点等。
>
