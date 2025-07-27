### 【1188】evaluate spline 评估样条线

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_eval.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_splines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_BCURVE_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_spline(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_splines, NULL, &resp,  
            &object, cp, &view));  
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
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    #define WRITEnF(X,n) (write_double_array_to_listing_window(#X, X, n, 1))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void evaluate_spline(tag_t item)  
    {  
        logical  
            is_spline;  
        int  
            dimensionality,  
            ii,  
            n_knots,  
            n_points;  
        double  
            data[6],  
            *knots,  
            *control_pts;  
        UF_EVAL_p_t  
            spline_eval;  
        UF_EVAL_spline_t  
            spline;  
        UF_OBJ_disp_props_t  
            attrib = { 0, UF_OBJ_WHITE, 0, 0, 1, 0 };  
        if (!UF_CALL(UF_EVAL_initialize(item, &spline_eval)))  
        {  
            UF_CALL(UF_EVAL_is_spline(spline_eval, &is_spline));  
            if (is_spline)  
            {  
                UF_CALL(UF_EVAL_ask_spline(spline_eval, &spline));  
                WRITE_L(spline.is_periodic);  
                WRITE_L(spline.is_rational);  
                WRITE_D(spline.order);  
                WRITE_D(spline.num_knots);  
                UF_CALL(UF_EVAL_ask_spline_knots(spline_eval, &n_knots, &knots));  
                WRITEnF(knots, n_knots);  
                UF_free(knots);  
                WRITE_D(spline.num_control);  
                UF_CALL(UF_EVAL_ask_spline_control_pts(spline_eval, &n_points,  
                    &control_pts));  
                WRITEnF4(control_pts, n_points);  
                for (ii = 0; ii < n_points * 4; ii = ii + 4)  
                    UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
                        UF_DISP_USE_ACTIVE_PLUS, &control_pts[ii], &attrib,  
                        UF_DISP_POINT));  
                UF_free(control_pts);  
            }  
            else  
                WRITE_L(is_spline);  
            UF_CALL(UF_EVAL_free(spline_eval));  
            UF_CALL(UF_MODL_ask_obj_dimensionality(item, &dimensionality, data));  
            WRITE_D(dimensionality);  
        }  
    }  
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        tag_t  
            spline;  
        while ((spline = select_a_spline("Select spline to evaluate")) != NULL_TAG)  
        {  
            UF_DISP_refresh();  
            evaluate_spline(spline);  
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

> 这段代码是一个NX二次开发的示例，主要功能是选择一个样条曲线并评估其属性。
>
> 主要内容包括：
>
> 1. 函数声明：定义了错误报告、样条选择、临时点显示、数据写入等辅助函数。
> 2. 样条选择：定义了mask_for_splines函数，设置选择掩码只选择样条曲线。同时定义了select_a_spline函数，通过对话框选择样条曲线并返回其tag。
> 3. 样条评估：定义了evaluate_spline函数，获取样条曲线的周期性、有理性、阶数、节点数、控制点等信息，并在图形窗口中高亮显示控制点。
> 4. 主函数：在do_it函数中，循环调用select_a_spline选择样条，然后调用evaluate_spline评估。
> 5. 入口点：定义了ufusr函数作为程序的入口点，在NX中可以直接调用。
> 6. 卸载处理：定义了ufusr_ask_unload函数，用于处理程序的卸载。
>
> 整体而言，这段代码实现了选择并评估NX样条曲线的功能，展示了NX二次开发的基本流程。
>
