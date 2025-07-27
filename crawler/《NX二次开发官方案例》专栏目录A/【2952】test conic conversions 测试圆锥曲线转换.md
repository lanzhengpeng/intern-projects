### 【2952】test conic conversions 测试圆锥曲线转换

#### 代码

```cpp
    /*HEAD TEST_CONIC_CONVERSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_eval.h>  
    #include <uf_undo.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的功能。这个功能允许用户通过统一的接口打印系统日志。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE6F(X)  (write_double_array_to_listing_window(#X, X, 1, 6))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
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
    static int mask_for_conics(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ELLIPTICAL_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_conic(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a conic", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_conics, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical do_curves_match(tag_t curve1, tag_t curve2, double dist_tol)  
    {  
        logical  
            is_equal;  
        int  
            forward = 1,  
            irc,  
            reverse = -1;  
        tag_t  
            forward_bcurve,  
            reverse_bcurve;  
        double  
            end1[3],  
            end2[3],  
            limits1[2],  
            limits2[2],  
            dist,  
            start1[3],  
            start2[3];  
        UF_EVAL_p_t  
            eval_p1,  
            eval_p2;  
        UF_UNDO_mark_id_t  
            mark_id;  
    /*  Check for exact copy 里海译:检查是否存在精确复制 */  
        UF_CALL(UF_EVAL_initialize(curve1, &eval_p1));  
        UF_CALL(UF_EVAL_initialize(curve2, &eval_p2));  
        UF_CALL(UF_EVAL_is_equal(eval_p1, eval_p2, &is_equal));  
        if (is_equal)  
        {  
            UF_CALL(UF_EVAL_free(eval_p1));  
            UF_CALL(UF_EVAL_free(eval_p2));  
            return TRUE;  
        }  
    /*  Check for a reversed copy 里海译:检查是否有反转副本 */  
        UF_CALL(UF_EVAL_ask_limits(eval_p1, limits1));  
        UF_CALL(UF_EVAL_ask_limits(eval_p2, limits2));  
        UF_CALL(UF_EVAL_evaluate(eval_p1, 0, limits1[0], start1, NULL));  
        UF_CALL(UF_EVAL_evaluate(eval_p2, 0, limits2[1], end2, NULL));  
        UF_VEC3_distance(start1, end2, &dist);  
        if (dist > dist_tol)  
        {  
            UF_CALL(UF_EVAL_free(eval_p1));  
            UF_CALL(UF_EVAL_free(eval_p2));  
            return FALSE;  
        }  
        UF_CALL(UF_EVAL_evaluate(eval_p1, 0, limits1[1], end1, NULL));  
        UF_CALL(UF_EVAL_evaluate(eval_p2, 0, limits2[0], start2, NULL));  
        UF_VEC3_distance(start2, end1, &dist);  
        UF_CALL(UF_EVAL_free(eval_p1));  
        UF_CALL(UF_EVAL_free(eval_p2));  
        if (dist > dist_tol) return FALSE;  
    /*  Ends match - So compare extracted splines 里海译:结束比赛 - 因此比较提取的样条线 */  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        FTN(uf5466)(&curve1, &forward, &forward_bcurve, &irc);  
        WRITENZ(irc);  
        FTN(uf5466)(&curve2, &reverse, &reverse_bcurve, &irc);  
        WRITENZ(irc);  
        UF_CALL(UF_EVAL_initialize(forward_bcurve, &eval_p1));  
        UF_CALL(UF_EVAL_initialize(reverse_bcurve, &eval_p2));  
        UF_CALL(UF_EVAL_is_equal(eval_p1, eval_p2, &is_equal));  
        UF_CALL(UF_EVAL_free(eval_p1));  
        UF_CALL(UF_EVAL_free(eval_p2));  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
        if (is_equal) return TRUE;  
        return FALSE;  
    }  
    static void report_conic_data(UF_CURVE_conic_t *conic_data)  
    {  
        WRITE_D(conic_data->matrix_tag);  
        WRITE_D(conic_data->conic_type);  
        WRITE_F(conic_data->rotation_angle);  
        WRITE_F(conic_data->start_param);  
        WRITE_F(conic_data->end_param);  
        WRITE3F(conic_data->center);  
        WRITE_F(conic_data->k1);  
        WRITE_F(conic_data->k2);  
    }  
    static void init_conic_data(UF_CURVE_conic_t *conic_data)  
    {  
        conic_data->matrix_tag = NULL_TAG;  
        conic_data->conic_type = 0;  
        conic_data->rotation_angle = 0;  
        conic_data->start_param = 0;  
        conic_data->end_param = 0;  
        conic_data->center[0] = 0;  
        conic_data->center[1] = 0;  
        conic_data->center[2] = 0;  
        conic_data->k1 = 0;  
        conic_data->k2 = 0;  
    }  
    static void init_genconic_data(UF_CURVE_genconic_t *genconic_data)  
    {  
        genconic_data->matrix_tag = NULL_TAG;  
        genconic_data->coefficients[0] = 0;  
        genconic_data->coefficients[1] = 0;  
        genconic_data->coefficients[2] = 0;  
        genconic_data->coefficients[3] = 0;  
        genconic_data->coefficients[4] = 0;  
        genconic_data->coefficients[5] = 0;  
        genconic_data->start_pt[0] = 0;  
        genconic_data->start_pt[1] = 0;  
        genconic_data->start_pt[2] = 0;  
        genconic_data->end_pt[0] = 0;  
        genconic_data->end_pt[1] = 0;  
        genconic_data->end_pt[2] = 0;  
    }  
    static void report_genconic_data(UF_CURVE_genconic_t *genconic_data)  
    {  
        WRITE_D(genconic_data->matrix_tag);  
        WRITE6F(genconic_data->coefficients);  
        WRITE3F(genconic_data->start_pt);  
        WRITE3F(genconic_data->end_pt);  
    }  
    static void do_it(void)  
    {  
        logical  
            sense;  
        double  
            tol;  
        tag_t  
            orig_conic,  
            new_conic;  
        UF_CURVE_conic_t  
            conic_data;  
        UF_CURVE_genconic_t  
            genconic_data;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while ((orig_conic = select_a_conic("Conversion test")) != NULL_TAG)  
        {  
            init_conic_data(&conic_data);  
            UF_CALL(UF_CURVE_ask_conic_data(orig_conic, &conic_data));  
            report_conic_data(&conic_data);  
            init_genconic_data(&genconic_data);  
            UF_CALL(UF_CURVE_convert_conic_to_gen(&conic_data, &genconic_data));  
            report_genconic_data(&genconic_data);  
            init_conic_data(&conic_data);  
            UF_CALL(UF_CURVE_convert_conic_to_std(&genconic_data, &conic_data, &sense));  
            report_conic_data(&conic_data);  
            WRITE_L(sense);  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
            if (!UF_CALL(UF_CURVE_create_conic(&conic_data, &new_conic)))  
            {  
                WRITE_L(do_curves_match(orig_conic, new_conic, tol));  
            }  
            UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
            UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
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

> 这段代码是用于在NX中进行圆锥曲线转换的示例代码，主要实现了以下功能：
>
> 1. 选择圆锥曲线：通过自定义的UI选择对话框，实现了在NX中选取圆锥曲线的功能。
> 2. 查询圆锥曲线参数：通过调用UF_CURVE_ask_conic_data函数，可以查询到所选圆锥曲线的参数，如中心、旋转角度等，并输出这些参数。
> 3. 圆锥曲线转换为通用参数形式：调用UF_CURVE_convert_conic_to_gen函数，将圆锥曲线转换为通用参数形式，并输出通用参数。
> 4. 通用参数形式转换为标准形式：调用UF_CURVE_convert_conic_to_std函数，将通用参数形式的圆锥曲线转换回标准形式，并输出转换后的参数。
> 5. 创建新的圆锥曲线：根据转换后的标准参数，调用UF_CURVE_create_conic函数创建新的圆锥曲线。
> 6. 比较新旧圆锥曲线：通过比较新旧圆锥曲线的参数，检查转换前后曲线是否一致。
> 7. 错误处理和日志打印：利用UF_CALL宏，实现了统一的错误处理和日志打印功能。
> 8. 资源释放：在创建新曲线前，先设置撤销标记，以便在出错时可以撤销操作。
> 9. 主函数和卸载处理：提供了ufusr主函数和ufusr_ask_unload卸载处理函数。
>
> 总的来说，这段代码展示了如何使用NX的二次开发接口，实现了圆锥曲线的查询、转换和创建功能，并具有完善的错误处理机制。
>
