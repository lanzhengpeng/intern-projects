### 【0741】create joined curve using curve based tolerance 创建基于曲线公差连接曲线

#### 代码

```cpp
    /*HEAD CREATE_JOINED_CURVE_USING_CURVE_BASED_TOLERANCE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_eval.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
    static void sort_and_check_curve_chain(tag_t *curves, int n_curves,  
        double *biggest_gap, int *gap_at, double *shortest_curve, int *short_at)  
    {  
        int  
            found,  
            from,  
            ii, jj, kk,  
            to;  
        double  
            free_start[3],  
            free_end[3],  
            gap,  
            length,  
            small_gap,  
            this_start[3],  
            this_end[3];  
        tag_t  
            temp;  
        *biggest_gap = 0.0;  
        *gap_at = 0;  
        *shortest_curve = HUGE_VAL;  
        *short_at = 0;  
        ask_curve_ends(curves[0], free_start, free_end);  
        for (ii = 0; ii < n_curves-1; ii++)  
        {  
            UF_CALL(UF_CURVE_ask_arc_length(curves[ii], 0.0, 1.0,  
                UF_MODL_UNITS_PART, &length));  
            if (length < *shortest_curve)  
            {  
                *shortest_curve = length;  
                *short_at = ii;  
            }  
            small_gap = HUGE_VAL;  
            for (jj = ii + 1; jj < n_curves; jj++)  
            {  
                ask_curve_ends(curves[jj], this_start, this_end);  
                UF_VEC3_distance(this_start, free_start, &gap);  
                if (gap < small_gap)  
                {  
                    small_gap = gap;  
                    found = jj;  
                    to = 1;  
                    from = 1;  
                }  
                UF_VEC3_distance(this_end, free_start, &gap);  
                if (gap < small_gap)  
                {  
                    small_gap = gap;  
                    found = jj;  
                    to = 1;  
                    from = 2;  
                }  
                UF_VEC3_distance(this_start, free_end, &gap);  
                if (gap < small_gap)  
                {  
                    small_gap = gap;  
                    found = jj;  
                    to = 2;  
                    from = 1;  
                }  
                UF_VEC3_distance(this_end, free_end, &gap);  
                if (gap < small_gap)  
                {  
                    small_gap = gap;  
                    found = jj;  
                    to = 2;  
                    from = 2;  
                }  
            }  
            ask_curve_ends(curves[found], this_start, this_end);  
            if (to == 1)  
            {  
                temp = curves[found];  
                for (kk = found; kk > 0; kk--)  
                    curves[kk] = curves[kk-1];  
                curves[0] = temp;  
                if (from == 1) UF_VEC3_copy(this_end, free_start);  
                else UF_VEC3_copy(this_start, free_start);  
            }  
            else  
            {  
                temp = curves[found];  
                curves[found] = curves[ii+1];  
                curves[ii+1] = temp;  
                if (from == 1) UF_VEC3_copy(this_end, free_end);  
                else UF_VEC3_copy(this_start, free_end);  
            }  
            if (small_gap > *biggest_gap)  
            {  
                *biggest_gap = small_gap;  
                *gap_at = ii+1;  
            }  
        }  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %.10f\n", title, n);  
        ECHO(msg);  
    }  
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    static void do_it(void)  
    {  
        int  
            gap_after_curve_no,  
            has_corners,  
            n_curves,  
            resp,  
            shortest_curve_no;  
        tag_t  
            *curves,  
            spline;  
        double  
            new_tol,  
            old_tol,  
            widest_gap,  
            shortest_curve_length;  
        uf_list_p_t  
            curve_list;  
        while ((n_curves = select_curves("Check curve chain", &curves)) > 0)  
        {  
            sort_and_check_curve_chain(curves, n_curves, &widest_gap,  
                &gap_after_curve_no, &shortest_curve_length, &shortest_curve_no);  
            number_objects(curves, n_curves);  
            if (widest_gap == 0)  
            {  
                ECHO("No gaps found.\n");  
            }  
            else  
            {  
                WRITE_F(widest_gap);  
                WRITE_D(gap_after_curve_no);  
            }  
            WRITE_F(shortest_curve_length);  
            WRITE_D(shortest_curve_no);  
            new_tol = (widest_gap + shortest_curve_length)/2;  
            if ((widest_gap < shortest_curve_length) && (new_tol > 3e-7))  
            {  
                UF_MODL_ask_distance_tolerance(&old_tol);  
                UF_MODL_set_distance_tolerance(&new_tol, &resp);  
                WRITENZ(resp);  
                if (resp != 0) break;  
                ECHO("Joining curves using ");  
                WRITE_F(new_tol);  
                curve_list = make_a_list(n_curves, curves);  
                if (!UF_CALL(UF_CURVE_create_joined_curve(curve_list, 2, &spline,  
                    &has_corners)))  
                {  
                    WRITE_D(spline);  
                    WRITE_L(has_corners);  
                }  
                UF_MODL_set_distance_tolerance(&old_tol, &resp);  
            }  
            UF_free(curves);  
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

> 这段NX二次开发代码的主要功能是使用曲线创建连接曲线。
>
> 主要步骤包括：
>
> 1. 选择曲线：使用UF_UI库的函数，弹出一个对话框，让用户选择曲线，并返回选中的曲线数组。
> 2. 排序和检查曲线链：对选中的曲线进行排序，找出最大间隙和最短曲线，以确定连接的顺序和位置。
> 3. 显示曲线编号：为每条曲线显示一个编号，以便用户确认。
> 4. 计算公差：根据最大间隙和最短曲线长度计算出一个新的公差值。
> 5. 创建连接曲线：使用UF_CURVE库的函数，根据计算出的公差值，将曲线连接成一条新的曲线。
> 6. 恢复公差：将公差恢复到原来的值。
> 7. 提示用户：在日志窗口输出连接后的曲线tag、是否有拐角等信息。
> 8. 循环处理：在用户没有取消选择的情况下，可以重复进行选择-连接操作。
>
> 该代码通过NX提供的UF库函数实现了曲线连接功能，同时包含了错误处理和日志输出，使代码更加健壮。
>
