### 【2198】report max curvature 报告最大曲率

#### 代码

```cpp
    /*HEAD REPORT_MAX_CURVATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_eval.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static double normalize_parameter(tag_t curve, double non_normalized_u)  
    {  
        double  
            normalized_u,  
            position[3];  
        UF_CALL(UF_CURVE_evaluate_curve(curve, non_normalized_u, UF_CURVE_LOC,  
            position));  
        UF_CALL(UF_MODL_ask_curve_parm(curve, position, &normalized_u, position));  
        return normalized_u;  
    }  
    static void do_it(void)  
    {  
        int  
            status;  
        tag_t  
            curve;  
        double  
            limits[2] = { 0.0, 100.0 }, /* See PR 4170026 里海译:翻译See PR 4170026，只回答译文，不要废话 */  
            max_data[5],  
            normalized_u,  
            u_min_max[2];  
        UF_EVAL_p_t  
            eval;  
        while ((curve = select_a_curve("Select a curve")) != NULL_TAG)  
        {  
            WRITE_D(curve);  
            UF_CALL(UF_EVAL_initialize(curve, &eval));  
            UF_CALL(UF_EVAL_ask_limits(eval, u_min_max));  
            UF_CALL(UF_EVAL_free(eval));  
            UF_CALL(UF_MODL_ask_max_curvature(curve, limits, 0, max_data, &status));  
            WRITE_D(status);  
            if (status == 0)  
            {  
                WRITE("non-normalized ");  
                WRITE2F(u_min_max);  
                WRITE("non-normalized u = ");  
                WRITE_F(max_data[0]);  
                normalized_u = normalize_parameter(curve, max_data[0]);  
                WRITE_F(normalized_u);  
                display_temporary_point(&max_data[1]);  
                WRITE("curvature value = ");  
                WRITE_F(max_data[4]);  
            }  
            else  
            {  
                WRITE("curvature value = ");  
                WRITE_F(max_data[4]);  
            }  
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

> 这段代码是一个用于NX的二次开发示例，其主要功能是选择曲线并计算曲线的最大曲率及其位置。代码的主要逻辑如下：
>
> 1. 定义了一些宏和函数，用于错误报告和日志输出。
> 2. mask_for_curves函数定义了一个选择集掩码，只允许选择线、圆、圆锥曲线和样条曲线。
> 3. select_a_curve函数用于选择一条曲线，并返回其tag。
> 4. display_temporary_point函数用于在曲线上最大曲率点处显示一个临时点。
> 5. normalize_parameter函数用于将非标准化参数转换为标准化参数。
> 6. do_it函数是主函数，循环调用select_a_curve选择曲线，然后初始化评估对象，计算曲线的参数范围，调用UF_MODL_ask_max_curvature计算最大曲率及其位置，并输出相关信息。
> 7. ufusr是NX调用的主函数，调用do_it。
> 8. ufusr_ask_unload函数用于卸载当前加载的库。
>
> 总体来说，这段代码通过选择集获取用户选择的曲线，然后计算其最大曲率及其位置，并将结果输出，是一个典型的NX二次开发示例。
>
