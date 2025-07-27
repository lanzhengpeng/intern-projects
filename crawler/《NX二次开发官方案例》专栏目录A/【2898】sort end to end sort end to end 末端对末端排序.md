### 【2898】sort end to end sort end to end 末端对末端排序

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
    /* UF_MODL_put_list_item adds to the end.  This adds at the start. 译:UF_MODL_put_list_item 添加到末尾。这添加到开头。 */  
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
    /*  this loops until both ends are dead ends 译:这句话的翻译是：这个循环直到两端都变成死胡同为止。 */  
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

```

#### 代码解析

> 这段代码是用于在NX中排序曲线的二次开发代码，其主要功能包括：
>
> 1. ask_curve_ends函数：获取给定曲线的开始和结束点坐标。
> 2. push_list_item函数：在列表的开始处添加新的曲线。
> 3. find_any_matches函数：在待排序的曲线列表中找到与已排序曲线两端匹配的曲线，并移到已排序的列表中。
> 4. sort_end_to_end函数：遍历待排序的曲线列表，通过找到两端与已排序曲线最接近的曲线，并按顺序添加到已排序的列表中，直到所有曲线都被排序。
> 5. 主要流程：首先，移除长度小于给定公差tol的短曲线；然后，将第一条曲线作为已排序曲线，其余作为待排序曲线；接着，在待排序曲线中查找与已排序曲线两端匹配的曲线，将其移到已排序曲线的末尾；最后，重复这个过程，直到所有曲线都被排序。
> 6. 输出：最终返回排序后的曲线列表。
>
> 综上所述，这段代码通过曲线的端点匹配实现了曲线的排序功能。
>
