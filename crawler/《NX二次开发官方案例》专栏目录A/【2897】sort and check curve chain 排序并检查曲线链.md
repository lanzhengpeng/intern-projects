### 【2897】sort and check curve chain 排序并检查曲线链

#### 代码

```cpp
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
    /* UF_MODL_put_list_item adds to the end.  This adds at the start. 译:翻译为：UF_MODL_put_list_item 添加到列表的末尾。这会将项目添加到列表的开头。 */  
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
    /*  this loops until both ends are dead ends 译:这个循环会一直持续，直到两端都成为死路为止。 */  
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
    /* return: 0 = good closed chain, 1 = good open chain, 2 = doesn't chain 译:返回值：0表示良好的闭环，1表示良好的开环，2表示没有闭环 */  
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

```

#### 代码解析

> 这是段NX二次开发代码，主要实现了以下功能：
>
> 1. 排序曲线：根据曲线端点的距离进行排序，以形成顺序链。此功能通过函数sort_end_to_end()实现。
> 2. 检查曲线链：检查排序后的曲线链是否形成一个闭环或开环，并找出链中的缺口。此功能通过函数sort_and_check_curve_chain()实现。
> 3. 临时显示文本和点：在曲线端点显示文本编号，以及在缺口处显示临时点。这些功能通过display_temporary_text()和display_temporary_point()函数实现。
> 4. 获取曲线端点信息：通过函数ask_curve_ends()获取曲线的起点和终点坐标。
> 5. 创建和操作列表：通过函数push_list_item()、UF_MODL_create_list()等操作曲线的列表。
> 6. 计算距离：通过函数UF_VEC3_distance()计算两点之间的距离。
> 7. 比较坐标是否相等：通过函数UF_VEC3_is_equal()判断两点的坐标是否在容差范围内相等。
> 8. 调用NX的显示函数：如UF_DISP_display_temporary_text()和UF_DISP_display_temporary_point()等，用于在图形窗口中显示文本和点。
> 9. 获取曲线的最小距离：通过函数UF_MODL_ask_minimum_dist_3()获取两曲线之间的最小距离。
> 10. 删除和插入列表项：通过函数UF_MODL_delete_list_item()和UF_MODL_put_list_item()在列表中删除和插入曲线项。
>
> 综合来看，这段代码主要用于处理和排序NX中的曲线，检查曲线是否形成闭环或开环，并在图形窗口中显示相关的信息。
>
