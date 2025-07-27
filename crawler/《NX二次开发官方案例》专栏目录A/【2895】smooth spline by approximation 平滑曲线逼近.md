### 【2895】smooth spline by approximation 平滑曲线逼近

#### 代码

```cpp
    /*HEAD SMOOTH_SPLINE_BY_APPROXIMATION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
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
    static tag_t select_a_spline(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a spline", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_splines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        ECHO(msg);  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
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
    static int ask_curve_close_end(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            parm;  
        UF_CALL(UF_MODL_ask_curve_parm(curve, pos, &parm, pos));  
        UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, parm, UF_MODL_UNITS_PART,  
            &dist[0]));  
        UF_CALL(UF_CURVE_ask_arc_length(curve, parm, 1.0, UF_MODL_UNITS_PART,  
            &dist[1]));  
        if (dist[0] < dist[1]) return 1;  
        else return 2;  
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
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            has_corners,  
            n;  
        tag_t  
            new_spline,  
            *segs,  
            spline;  
        double  
            end[3],  
            start[3],  
            tol = 0.001;  
        uf_list_p_t  
            list;  
        while (((spline = select_a_spline("Smooth by approximation")) != NULL_TAG)  
            && prompt_for_a_number("Looser will be smoother", "Tolerance", &tol))  
        {  
            set_undo_mark("Smooth spline");  
            UF_CALL(UF_CURVE_create_simplified_curve(1, &spline, tol, &n, &segs));  
            UF_CALL(UF_MODL_create_list(&list));  
            ask_curve_ends(segs[0], start, end);  
            if (ask_curve_close_end(spline, start) == 1)  
            {  
                for (ii = 0; ii < n; ii++)  
                    UF_CALL(UF_MODL_put_list_item(list, segs[ii]));  
            }  
            else  
            {  
                for (ii = n; ii > 0; ii--)  
                    UF_CALL(UF_MODL_put_list_item(list, segs[ii-1]));  
            }  
            UF_CALL(UF_CURVE_create_joined_curve(list, 2, &new_spline,  
                &has_corners));  
            if (has_corners) WRITE_L(has_corners);  
            UF_CALL(UF_MODL_delete_list(&list));  
            UF_CALL(UF_OBJ_delete_array_of_objects(n, segs, NULL));  
            UF_free(segs);  
            UF_CALL(UF_OBJ_replace_object_data(spline, new_spline));  
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

> 这段代码是NX的二次开发代码，其主要功能是利用逼近算法对选中的样条曲线进行平滑处理。
>
> 具体来说，代码的主要步骤包括：
>
> 1. 定义了一些帮助函数，如report_error()用于输出错误信息，mask_for_splines()用于设置选择掩码，以及ask_curve_ends()和ask_curve_close_end()用于查询曲线的端点信息。
> 2. 主函数do_it()首先选择一条样条曲线，并提示用户输入平滑参数tolerance。
> 3. 使用UF_CURVE_create_simplified_curve()函数，根据输入的tolerance，将样条曲线简化成多段直线。
> 4. 查询简化后的直线段，确定它们的连接顺序。
> 5. 使用UF_CURVE_create_joined_curve()函数，将直线段连接成一条新的样条曲线。
> 6. 如果新样条曲线有尖角，则输出提示信息。
> 7. 替换原来的样条曲线为新样条曲线。
> 8. 循环上述步骤，直到用户选择退出。
>
> 该代码利用逼近算法对样条曲线进行平滑处理，实现了NX的二次开发功能。
>
