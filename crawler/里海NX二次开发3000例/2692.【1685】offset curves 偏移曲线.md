### 【1685】offset curves 偏移曲线

#### 代码

```cpp
    /*HEAD OFFSET_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_eval.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，UF_print_syslog 是 V18 新增的函数。 */  
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
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 }};  
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
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
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
    static void sort_into_strings(tag_t *curves, int n_curves, UF_STRING_p_t string)  
    {  
        logical  
            found;  
        int  
            dirs[100],  
            ii,  
            jj,  
            kk,  
            n_segs[100],  
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
        n_segs[n_strings] = 1;  
        dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
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
                    n_segs[n_strings]++;  
                    dirs[n_strings] = UF_MODL_CURVE_START_FROM_END;  
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
                    n_segs[n_strings]++;  
                    dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
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
                    n_segs[n_strings]++;  
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
                    n_segs[n_strings]++;  
                    UF_VEC3_copy(this_start, free_end);  
                    found = TRUE;  
                    break;  
                }  
            }  
            if (found == FALSE)  
            {  
                n_strings++;  
                n_segs[n_strings] = 1;  
                dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
                start = ii+1;  
                if (ii+1 < n_curves)  
                    ask_curve_ends(curves[ii+1], free_start, free_end);  
            }  
        }  
        UF_MODL_init_string_list(string);  
        UF_MODL_create_string_list(n_strings, n_curves, string);  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            string->string[ii] = n_segs[ii];  
            string->dir[ii] = dirs[ii];  
        }  
        for (ii = 0; ii < n_curves; ii++)  
            string->id[ii] = curves[ii];  
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
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves,  
            num_curves;  
        tag_t  
            *curves,  
            offset_object,  
            *offset_curves;  
        double  
            offset_vec[3],  
            distance = 1.0,  
            draft_vec[3],  
            base_pt[3];  
        char  
            dist_str[21];  
        UF_STRING_t  
            curvs;  
        UF_CURVE_offset_data_t  
            offset_data;  
        UF_CURVE_offset_distance_data_t  
            distance_data;  
        distance_data.distance = dist_str;  
        offset_data.approximation_tolerance = 0.001;  
        UF_MODL_ask_distance_tolerance(&offset_data.string_tolerance);  
        offset_data.offset_type = UF_CURVE_OFFSET_DISTANCE_TANGENT;  
        offset_data.offset_def.distance_type1 = &distance_data;  
        while (((n_curves = select_curves("Offset curves", &curves)) > 0) &&  
            prompt_for_a_number("Offset", "Distance", &distance))  
        {  
            sort_into_strings(curves, n_curves, &curvs);  
            UF_free(curves);  
            offset_data.input_curves = &curvs;  
            if (UF_CALL(UF_CURVE_ask_offset_direction(&curvs, offset_vec,  
                draft_vec, base_pt))) continue;  
            UF_DISP_refresh();  
            UF_DISP_conehead( 0, base_pt, offset_vec, 0);  
            if (ask_yes_or_no("Offset direction", "Flip?")) distance = -distance;  
            sprintf(dist_str, "%f", distance);  
            strip_trailing_zeros(dist_str);  
            UF_CALL(UF_CURVE_create_offset_object(&offset_data, &offset_object));  
            /* Or this way 里海译:翻译如下：

Or this way

或者这样 */  
            UF_CALL(UF_CURVE_create_offset_curve(&offset_data, &num_curves,  
                &offset_curves));  
            UF_MODL_free_string_list(&curvs);  
            UF_free(offset_curves);  
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

> 这是段用于NX的二次开发代码，其主要功能是创建曲线的偏移曲线。下面是该代码的简要介绍：
>
> 1. 函数和宏定义：包括错误报告宏、曲线选择掩码函数、选择曲线函数、获取曲线端点函数、将曲线排序到字符串中的函数、提示输入数字函数、提示选择是或否的函数，以及去除字符串尾随零的函数。
> 2. 主要功能：包括选择曲线、对选择的曲线进行排序、获取曲线偏移方向、提示是否翻转偏移方向、创建曲线偏移对象以及创建曲线偏移曲线。
> 3. 用户界面：通过对话框选择曲线，并提示输入偏移距离。在创建偏移曲线后，提示用户是否翻转偏移方向。
> 4. 代码结构：主函数do_it()中包含主要功能逻辑，首先选择曲线，然后创建偏移曲线，重复此过程直到用户取消选择。
> 5. 注释：代码中包含中文注释，解释了关键函数和宏的作用。
> 6. NX版本：代码使用了NX的UF函数进行二次开发，适用于NX 18版本。
> 7. 用户交互：通过NX的用户界面与用户交互，提示输入偏移距离和是否翻转偏移方向。
> 8. 错误处理：通过错误报告宏UF_CALL进行错误处理，并显示错误信息。
>
> 综上所述，该代码实现了创建曲线偏移曲线的功能，具有良好的用户交互界面，适用于NX的二次开发。
>
