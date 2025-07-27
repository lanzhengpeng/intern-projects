### 【0867】create revolved feature 创建旋转特征

#### 代码

```cpp
    /*HEAD CREATE_REVOLVED_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本新增的函数，用于打印系统日志。该函数的具体功能包括：

- 打印系统日志消息，可接受消息字符串作为参数。
- 根据配置的日志级别，决定是否打印消息。
- 消息内容会包含时间戳、日志级别、来源文件和行号等信息。
- 如果启用了远程日志功能，消息还会被发送到远程日志服务器。

总之，UF_print_syslog是V18版本新增的日志打印函数，提供了便捷的系统日志打印功能。 */  
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
    static void sort_end_to_end(tag_t *curves, int n_curves)  
    {  
        logical  
            found;  
        int  
            ii,  
            jj,  
            kk,  
            n_strings,  
            start;  
        double  
            free_start[3],  
            free_end[3],  
            gap,  
            this_start[3],  
            this_end[3],  
            tol;  
        tag_t  
            temp;  
        UF_MODL_ask_distance_tolerance(&tol);  
        n_strings = 0;  
        ask_curve_ends(curves[0], free_start, free_end);  
        start = 0;  
        for (ii = 0; ii < n_curves; ii++)  
        {  
            found = FALSE;  
            for (jj = ii + 1; jj < n_curves; jj++)  
            {  
                ask_curve_ends(curves[jj], this_start, this_end);  
                UF_VEC3_distance(this_start, free_start, &gap);  
                if (gap <= tol)  
                {  
                    temp = curves[jj];  
                    for (kk = jj; kk > start; kk--)  
                        curves[kk] = curves[kk-1];  
                    curves[start] = temp;  
                    UF_VEC3_copy(this_end, free_start);  
                    found = TRUE;  
                    break;  
                }  
                UF_VEC3_distance(this_end, free_start, &gap);  
                if (gap <= tol)  
                {  
                    temp = curves[jj];  
                    for (kk = jj; kk > start; kk--)  
                        curves[kk] = curves[kk-1];  
                    curves[start] = temp;  
                    UF_VEC3_copy(this_start, free_start);  
                    found = TRUE;  
                    break;  
                }  
                UF_VEC3_distance(this_start, free_end, &gap);  
                if (gap <= tol)  
                {  
                    temp = curves[jj];  
                    curves[jj] = curves[ii+1];  
                    curves[ii+1] = temp;  
                    UF_VEC3_copy(this_end, free_end);  
                    found = TRUE;  
                    break;  
                }  
                UF_VEC3_distance(this_end, free_end, &gap);  
                if (gap <= tol)  
                {  
                    temp = curves[jj];  
                    curves[jj] = curves[ii+1];  
                    curves[ii+1] = temp;  
                    UF_VEC3_copy(this_start, free_end);  
                    found = TRUE;  
                    break;  
                }  
            }  
            if (found == FALSE)  
            {  
                n_strings++;  
                start = ii+1;  
                if (start < n_curves)  
                    ask_curve_ends(curves[ii+1], free_start, free_end);  
            }  
        }  
    }  
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
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
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_curves,  
            n_feats;  
        tag_t  
            *curves,  
            *feats;  
        uf_list_p_t  
            curve_list,  
            feat_list;  
        char  
            e_angle[20],  
            *limits[2],  
            *offsets[2] = { "0", "0" },  
            s_angle[20];  
        double  
            dir[3],  
            end = 360,  
            point[3],  
            start = 0;  
        limits[0] = s_angle;  
        limits[1] = e_angle;  
        while (((n_curves = select_curves("Select curves to revolve", &curves)) > 0)  
            && (specify_vector("Define revolution axis", dir, point))  
            && prompt_for_two_numbers("Limits", "Start Angle", "End Angle",  
                &start, &end))  
        {  
            sort_end_to_end(curves, n_curves); /* Work around for PR 679402 译:翻译如下：

为解决PR 679402的临时解决方案。 */  
            sprintf(s_angle, "%f", start);  
            sprintf(e_angle, "%f", end);  
            UF_CALL(UF_MODL_create_list(&curve_list));  
            for (ii = 0; ii < n_curves; ii++)  
                UF_CALL(UF_MODL_put_list_item(curve_list, curves[ii]));  
            if (!UF_CALL(UF_MODL_create_revolved(curve_list, limits, point, dir,  
                UF_NULLSIGN, &feat_list)))  
                UF_CALL(UF_MODL_delete_list(&feat_list));  
        /*  or  译:根据上下文，可以翻译为"或者"。 */  
            if (!UF_CALL(UF_MODL_create_revolution(curves, n_curves, NULL, limits,  
                offsets, point, FALSE, FALSE, point, dir, UF_NULLSIGN, &feats,  
                &n_feats)))  
                UF_free(feats);  
            UF_free(curves);  
            UF_CALL(UF_MODL_delete_list(&curve_list));  
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

> 这段NX二次开发代码的主要功能是创建旋转特征。具体来说，代码的主要步骤包括：
>
> 1. 包含了必要的NX Open API头文件。
> 2. 定义了错误报告函数report_error，用于在发生错误时输出错误信息。
> 3. 定义了ask_curve_ends函数，用于获取曲线的首尾点坐标。
> 4. 定义了sort_end_to_end函数，用于按首尾点对曲线进行排序，以解决PR 679402。
> 5. 定义了mask_for_curves函数，用于设置选择过滤，只选择线、圆、圆锥、样条线等曲线类型。
> 6. 定义了select_curves函数，用于让用户选择曲线。
> 7. 定义了specify_vector函数，用于让用户指定旋转轴。
> 8. 定义了prompt_for_two_numbers函数，用于让用户输入旋转的开始角度和结束角度。
> 9. 定义了do_it函数，用于实现旋转特征创建的主要逻辑，包括获取曲线、排序、创建旋转特征等。
> 10. 定义了ufusr和ufusr_ask_unload函数，作为NX的回调函数入口。
>
> 总的来说，这段代码通过交互方式让用户选择曲线，指定旋转轴和角度，最终创建出旋转特征。代码利用了NX的几何建模API来实现这一功能。
>
