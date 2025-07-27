### 【2870】show derivatives at curve points 显示曲线点的导数

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_eval.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #define ECHOF(X)   (printf("%s = %f\n", #X, X))  
    #define ECHO3F(X)  (printf("%s = %f, %f, %f\n", #X, *X,*(X+1),*(X+2)))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void show_curve_derivatives(tag_t curve, int n_pts)  
    {  
        int  
            ii;  
        double  
            derivs[6],  
            limits[2],  
            p,  
            point[3];  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(curve, &eval));  
        UF_CALL(UF_EVAL_ask_limits(eval, limits));  
        for (ii = 0; ii < n_pts; ii++)  
        {  
            p = limits[0] + ii * ((limits[1] - limits[0])/(n_pts - 1));  
            UF_CALL(UF_EVAL_evaluate(eval, 2, p, point, derivs));  
            UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
                UF_DISP_USE_ACTIVE_PLUS, point, &attrib, UF_DISP_POINT));  
            UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, point, &derivs[0], 0);  
            ECHO3F(&derivs[0]);  
            UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, point, &derivs[3], 0);  
            ECHO3F(&derivs[3]);  
        }  
        UF_CALL(UF_EVAL_free(eval));  
    }  
    static void do_it(void)  
    {  
        int  
            n = 10;  
        tag_t  
            curve;  
        while (((curve = select_a_curve_or_edge("Select curve/edge")) != NULL_TAG)  
            && (prompt_for_an_integer("How many?", "Points", &n)) && (n > 1))  
            show_curve_derivatives(curve, n);  
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

> 这段代码是一个NX Open C++ API开发的示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印出调用NX API时的错误信息。
> 2. 定义了一个选择曲线或边的函数select_a_curve_or_edge，通过用户对话框让用户选择一个曲线或边。
> 3. 定义了一个提示用户输入整数的函数prompt_for_an_integer，用于获取用户输入的点数。
> 4. 定义了一个显示曲线导数的函数show_curve_derivatives，通过UF_EVAL模块计算曲线在指定参数处的切线方向，并用锥形标示出来。
> 5. 在do_it函数中，循环调用select_a_curve_or_edge和prompt_for_an_integer，获取用户选择的曲线和点数，然后调用show_curve_derivatives显示曲线导数。
> 6. ufusr是主函数，初始化NX环境，调用do_it执行主功能，然后终止NX环境。
> 7. ufusr_ask_unload函数用于卸载用户自定义的动态库。
>
> 总体来说，这段代码通过NX Open C++ API实现了让用户选择曲线，然后显示曲线指定点数处的切线方向的功能。代码结构清晰，各部分功能明确，是一个典型的NX Open二次开发示例。
>
