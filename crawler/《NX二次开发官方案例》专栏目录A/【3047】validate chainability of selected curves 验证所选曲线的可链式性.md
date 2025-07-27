### 【3047】validate chainability of selected curves 验证所选曲线的可链式性

#### 代码

```cpp
    /*HEAD VALIDATE_CHAINABILITY_OF_SELECTED_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_eval.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    /* qq3123197280 */  
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
    /* UF_MODL_put_list_item adds to the end.  This adds at the start. 里海译:UF_MODL_put_list_item 在列表末尾添加项。而这是在列表开始处添加项。 */  
    static uf_list_p_t push_list_item(uf_list_p_t old_list, tag_t new_start)  
    {  
        uf_list_p_t  
            new_list;  
        UF_CALL(UF_MODL_create_list(&new_list));  
        new_list->eid = new_start;  
        new_list->next = old_list;  
        return new_list;  
    }  
    static void find_any_matches(uf_list_p_t *todo, uf_list_p_t *done,  
        double free_start[3], double free_end[3], double tol)  
    {  
        int  
            match;  
        double  
            this_start[3],  
            this_end[3];  
        uf_list_p_t  
            tmp;  
    /*  this loops until both ends are dead ends 里海译:这个循环直到两端都是死胡同为止。 */  
        for (tmp = *todo; tmp != NULL; )  
        {  
            ask_curve_ends(tmp->eid, this_start, this_end);  
            UF_VEC3_is_equal(this_start, free_start, tol, &match);  
            if (match)  
            {  
                *done = push_list_item(*done, tmp->eid);  
                UF_CALL(UF_MODL_delete_list_item(todo, tmp->eid));  
                UF_VEC3_copy(this_end, free_start);  
                tmp = *todo;  
                continue;  
            }  
            UF_VEC3_is_equal(this_end, free_start, tol, &match);  
            if (match)  
            {  
                *done = push_list_item(*done, tmp->eid);  
                UF_CALL(UF_MODL_delete_list_item(todo, tmp->eid));  
                UF_VEC3_copy(this_start, free_start);  
                tmp = *todo;  
                continue;  
            }  
            UF_VEC3_is_equal(this_start, free_end, tol, &match);  
            if (match)  
            {  
                UF_CALL(UF_MODL_put_list_item(*done, tmp->eid));  
                UF_CALL(UF_MODL_delete_list_item(todo, tmp->eid));  
                UF_VEC3_copy(this_end, free_end);  
                tmp = *todo;  
                continue;  
            }  
            UF_VEC3_is_equal(this_end, free_end, tol, &match);  
            if (match)  
            {  
                UF_CALL(UF_MODL_put_list_item(*done, tmp->eid));  
                UF_CALL(UF_MODL_delete_list_item(todo, tmp->eid));  
                UF_VEC3_copy(this_start, free_end);  
                tmp = *todo;  
                continue;  
            }  
            tmp = tmp->next;  
        }  
    }  
    static int sort_end_to_end(tag_t *curves, int n_curves, double tol)  
    {  
        int  
            ii;  
        tag_t  
            found_end,  
            found_start;  
        double  
            free_start[3],  
            free_end[3],  
            gap,  
            gap_end,  
            gap_start,  
            length,  
            new_start[3],  
            new_end[3],  
            this_start[3],  
            this_end[3];  
        uf_list_p_t  
            sorted = NULL,  
            tmp,  
            unsorted;  
        UF_CALL(UF_MODL_create_list(&unsorted));  
        for (ii = 0; ii < n_curves; ii++)  
        {  
            UF_CALL(UF_CURVE_ask_arc_length(curves[ii], 0.0, 1.0,  
                UF_MODL_UNITS_PART, &length));  
            if (length < tol)  
            {  
                UF_CALL(UF_DISP_set_highlight(curves[ii], TRUE));  
                ECHO("(Length = %f) < (tol = %f) dropped curve highlighted\n",  
                    length, tol);  
            }  
            else  
            {  
                if (!sorted)  
                {  
                    UF_CALL(UF_MODL_create_list(&sorted));  
                    UF_CALL(UF_MODL_put_list_item(sorted, curves[ii]));  
                }  
                else  
                    UF_CALL(UF_MODL_put_list_item(unsorted, curves[ii]));  
            }  
        }  
        if (!sorted) return 0;  
        ask_curve_ends(sorted->eid, free_start, free_end);  
        while (unsorted)  
        {  
            find_any_matches(&unsorted, &sorted, free_start, free_end, tol);  
            if (unsorted == NULL) break;  
            gap_start = HUGE_VAL;  
            gap_end = HUGE_VAL;  
            for (tmp = unsorted; tmp != NULL; tmp = tmp->next)  
            {  
                ask_curve_ends(tmp->eid, this_start, this_end);  
                UF_VEC3_distance(this_start, free_start, &gap);  
                if (gap < gap_start)  
                {  
                    found_start = tmp->eid;  
                    gap_start = gap;  
                    UF_VEC3_copy(this_end, new_start);  
                }  
                UF_VEC3_distance(this_end, free_start, &gap);  
                if (gap < gap_start)  
                {  
                    found_start = tmp->eid;  
                    gap_start = gap;  
                    UF_VEC3_copy(this_start, new_start);  
                }  
                UF_VEC3_distance(this_start, free_end, &gap);  
                if (gap < gap_end)  
                {  
                    found_end = tmp->eid;  
                    gap_end = gap;  
                    UF_VEC3_copy(this_end, new_end);  
                }  
                UF_VEC3_distance(this_end, free_end, &gap);  
                if (gap < gap_end)  
                {  
                    found_end = tmp->eid;  
                    gap_end = gap;  
                    UF_VEC3_copy(this_start, new_end);  
                }  
            }  
            if (found_end == found_start)  
            {  
                if (gap_start < gap_end)  
                    found_end = NULL_TAG;  
                else  
                    found_start = NULL_TAG;  
            }  
            if (found_start != NULL_TAG)  
            {  
                sorted = push_list_item(sorted, found_start);  
                UF_CALL(UF_MODL_delete_list_item(&unsorted, found_start));  
                UF_VEC3_copy(new_start, free_start);  
            }  
            if (found_end != NULL_TAG)  
            {  
                UF_CALL(UF_MODL_put_list_item(sorted, found_end));  
                UF_CALL(UF_MODL_delete_list_item(&unsorted, found_end));  
                UF_VEC3_copy(new_end, free_end);  
            }  
        }  
        for (ii = 0; sorted != NULL; ii++, sorted = sorted->next)  
            curves[ii] = sorted->eid;  
        UF_CALL(UF_MODL_delete_list(&sorted));  
        return ii;  
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
        UF_DISP_refresh();  
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
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_ASTERISK));  
    }  
    /* return: 0 = good closed chain, 1 = good open chain, 2 = doesn't chain 里海译:翻译return: 0 = good closed chain, 1 = good open chain, 2 = doesn't chain */  
    static int sort_and_check_curve_chain(tag_t *curves, int n_curves,  
        double tol)  
    {  
        int  
            ii,  
            flag = 0,  
            next;  
        double  
            accuracy,  
            dist,  
            cp_this[3],  
            cp_next[3],  
            gap,  
            next_start[3],  
            next_end[3],  
            on_this[3],  
            on_next[3],  
            pt[3],  
            small_gap,  
            this_start[3],  
            this_end[3];  
        n_curves = sort_end_to_end(curves, n_curves, tol);  
        number_objects(curves, n_curves);  
        ask_curve_ends(curves[0], this_start, this_end);  
        for (ii = 0; ii < n_curves; ii++)  
        {  
            if (ii == (n_curves - 1))  
                next = 0;  
            else  
                next = ii + 1;  
            ask_curve_ends(curves[next], next_start, next_end);  
            small_gap = HUGE_VAL;  
            UF_VEC3_distance(this_start, next_start, &gap);  
            if (gap < small_gap)  
            {  
                small_gap = gap;  
                UF_VEC3_copy(this_start, cp_this);  
                UF_VEC3_copy(next_start, cp_next);  
            }  
            UF_VEC3_distance(this_start, next_end, &gap);  
            if (gap < small_gap)  
            {  
                small_gap = gap;  
                UF_VEC3_copy(this_start, cp_this);  
                UF_VEC3_copy(next_end, cp_next);  
            }  
            UF_VEC3_distance(this_end, next_start, &gap);  
            if (gap < small_gap)  
            {  
                small_gap = gap;  
                UF_VEC3_copy(this_end, cp_this);  
                UF_VEC3_copy(next_start, cp_next);  
            }  
            UF_VEC3_distance(this_end, next_end, &gap);  
            if (gap < small_gap)  
            {  
                small_gap = gap;  
                UF_VEC3_copy(this_end, cp_this);  
                UF_VEC3_copy(next_end, cp_next);  
            }  
            if (small_gap > tol)  
            {  
                if ((next == 0) && (flag == 0)) flag = 1;  
                if (next == 0) continue;  
                flag = 2;  
                display_temporary_point(cp_this);  
                display_temporary_point(cp_next);  
                UF_CALL(UF_MODL_ask_minimum_dist_3(2, curves[ii],  
                    curves[next], FALSE, NULL, FALSE, NULL, &dist,  
                    on_this, on_next, &accuracy));  
                if (dist > 0)  
                {  
                    ECHO("gap between %d and %d = %.16f\n", ii+1, next+1, dist);  
                }  
                else  
                {  
                    UF_CALL(UF_MODL_ask_minimum_dist_3(2, NULL_TAG,  
                        curves[next], TRUE, cp_this, FALSE, NULL, &dist,  
                        pt, on_next, &accuracy));  
                    if (dist > 0)  
                        ECHO("intersection at %d and %d\n", ii+1, next+1);  
                    else  
                        ECHO("overlap at %d and %d = %.16f\n", ii+1, next+1,  
                            small_gap);  
                }  
            }  
            UF_VEC3_copy(next_start, this_start);  
            UF_VEC3_copy(next_end, this_end);  
        }  
        return flag;  
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
            flag,  
            n_curves;  
        tag_t  
            *curves;  
        double  
            tol;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while ((n_curves = select_curves("Check curve chain", &curves)) > 0)  
        {  
            while (prompt_for_a_number("Check curve chain", "Tolerance", &tol))  
            {  
                flag = sort_and_check_curve_chain(curves, n_curves, tol);  
                switch (flag)  
                {  
                    case 0:  
                        ECHO("Good Closed Chain\n");  
                        break;  
                    case 1:  
                        ECHO("Good Open Chain\n");  
                        break;  
                    case 2:  
                        ECHO("Curves do not chain\n");  
                }  
            }  
            UF_free(curves);  
            UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
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

> 这段代码是一个用于验证曲线链的NX二次开发应用程序。主要功能如下：
>
> 1. 用户可以通过对话框选择要检查的曲线。
> 2. 程序会按照曲线端点的距离进行排序，形成链。
> 3. 程序检查曲线链是否闭合或开放，并输出结果。
> 4. 如果曲线链不闭合，会高亮显示断开处，并计算间隙距离。
> 5. 用户可以设置公差值，程序会根据公差值来判断曲线端点是否匹配。
> 6. 程序使用了NX提供的各种API函数，如UF_EVAL、UF_CURVE、UF_VEC等，来获取曲线信息、计算距离等。
> 7. 使用UF_DISP临时显示文本和点，用于显示序号和间隙位置。
> 8. 在释放内存和清理高亮显示方面，程序使用UF_PART_cleanup进行清理。
> 9. 通过UF_MODL_ask_distance_tolerance获取系统默认的公差值。
> 10. 使用UF_CALL宏来检查API调用是否成功，并在出错时打印错误信息。
> 11. 使用UF_print_syslog在日志窗口打印信息。
> 12. 定义了多个辅助函数，如push_list_item、find_any_matches等，用于实现链排序和匹配算法。
> 13. 使用UF_OBJ_is_displayable检查对象是否可显示，以避免在不可显示对象上显示文本。
> 14. 使用UF_OBJ_ask_name_origin获取对象名称的显示位置。
> 15. 使用UF_DISP_display_temporary_text在对象名称位置显示临时文本。
> 16. 使用UF_DISP_display_temporary_point在间隙位置显示临时点。
> 17. 使用uc1609对话框让用户输入公差值。
> 18. 定义了ufusr和ufusr_ask_unload函数，以便NX调用。
>
> 综上所述，这段代码实现了根据曲线端点距离进行链排序、闭合性检查和显示间隙的功能，为曲线链的检查提供了便利。
>
