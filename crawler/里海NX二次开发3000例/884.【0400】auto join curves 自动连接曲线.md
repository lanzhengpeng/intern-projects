### 【0400】auto join curves 自动连接曲线

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_eval.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <stdarg.h>  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        objs = *curves;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
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
                ECHO("%.3f", array[kk++]);  
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
            *points;  
        UF_EVAL_p_t  
            spline_eval;  
        UF_EVAL_spline_t  
            spline;  
        UF_OBJ_disp_props_t  
            attrib = { 0, UF_OBJ_WHITE, 0, 0, 1, 0 };  
        if (!UF_CALL(UF_EVAL_initialize(item, &spline_eval)))  
        {  
            UF_CALL(UF_EVAL_is_spline(spline_eval, &is_spline));  
            WRITE_L(is_spline);  
            if (is_spline)  
            {  
                UF_CALL(UF_EVAL_ask_spline(spline_eval, &spline));  
                WRITE_L(spline.is_periodic);  
                WRITE_L(spline.is_rational);  
                WRITE_D(spline.order);  
                WRITE_D(spline.num_knots);  
                WRITE_D(spline.num_control);  
                UF_CALL(UF_EVAL_ask_spline_control_pts(spline_eval, &n_points,  
                    &points));  
                WRITEnF4(points, n_points);  
                for (ii = 0; ii < n_points * 4; ii = ii + 4)  
                    UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
                        UF_DISP_USE_ACTIVE_PLUS, &points[ii], &attrib,  
                        UF_DISP_POINT));  
                UF_free(points);  
                UF_CALL(UF_EVAL_ask_spline_knots(spline_eval, &n_knots, &knots));  
                WRITEnF(n_knots, knots);  
                UF_free(knots);  
            }  
            UF_CALL(UF_EVAL_free(spline_eval));  
            UF_CALL(UF_MODL_ask_obj_dimensionality(item,&dimensionality,data));  
            WRITE_D(dimensionality);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            nc,  
            nj,  
            resp;  
        tag_t  
            *curves,  
            *joined;  
        while ((nc = select_curves("Select curves to join", &curves)) > 0)  
        {  
            joined = (tag_t *)UF_allocate_memory(nc * sizeof(tag_t), &resp);  
            UF_CALL(UF_CURVE_auto_join_curves(curves, nc, 1, joined, &nj));  
            number_objects(joined, nj);  
            for (ii = 0; ii < nj; ii++) evaluate_spline(joined[ii]);  
            UF_free(curves);  
            UF_free(joined);  
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

> 这段NX二次开发代码的主要功能是自动连接用户选择的曲线，并显示连接后的曲线信息。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于在发生错误时打印错误信息。
> 2. 定义曲线选择掩码函数mask_for_curves，用于选择特定类型的曲线。
> 3. 定义选择曲线函数select_curves，用于让用户选择要连接的曲线。
> 4. 定义显示临时文字函数display_temporary_text，用于在曲线上显示临时文字。
> 5. 定义给对象编号函数number_objects，用于给连接后的曲线编号。
> 6. 定义写数组到窗口函数write_double_array_to_listing_window，用于将数组写入日志窗口。
> 7. 定义求值样条函数evaluate_spline，用于获取样条曲线的控制点等信息。
> 8. 定义主函数do_it，用于循环选择曲线，自动连接曲线，并显示连接后的曲线信息。
> 9. 定义UF初始化函数ufusr，用于初始化UF环境并调用主函数。
> 10. 定义卸载函数ufusr_ask_unload，用于卸载用户函数。
>
> 通过以上步骤，该代码实现了自动连接曲线并显示连接后曲线信息的功能，为用户提供了一个实用的二次开发工具。
>
