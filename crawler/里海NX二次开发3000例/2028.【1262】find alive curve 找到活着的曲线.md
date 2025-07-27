### 【1262】find alive curve 找到活着的曲线

#### 代码

```cpp
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
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
    /*  Check for exact copy 译:根据上下文，"Check for exact copy" 在这里的意思是“检查是否有完全相同的副本”。

所以，该句话的翻译为：检查是否有完全相同的副本。 */  
        UF_CALL(UF_EVAL_initialize(curve1, &eval_p1));  
        UF_CALL(UF_EVAL_initialize(curve2, &eval_p2));  
        UF_CALL(UF_EVAL_is_equal(eval_p1, eval_p2, &is_equal));  
        if (is_equal)  
        {  
            UF_CALL(UF_EVAL_free(eval_p1));  
            UF_CALL(UF_EVAL_free(eval_p2));  
            return TRUE;  
        }  
    /*  Check for a reversed copy 译:检查是否有反转副本 */  
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
    /*  Ends match - So compare extracted splines 译:结束比赛 - 因此比较提取的样条线 */  
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
    static tag_t find_alive_curve(tag_t condemned_curve)  
    {  
        int  
            ii,  
            n_objects,  
            subtype,  
            type;  
        tag_t  
            alive_curve = NULL_TAG,  
            *objects,  
            part = UF_PART_ask_display_part();  
        double  
            tol;  
        UF_MODL_ask_distance_tolerance(&tol);  
        UF_CALL(UF_OBJ_ask_type_and_subtype(condemned_curve, &type, &subtype));  
        n_objects = ask_all_of_type(part, type, &objects);  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            if (do_curves_match(condemned_curve, objects[ii], tol))  
            {  
                alive_curve = objects[ii];  
                break;  
            }  
        }  
        if (n_objects > 0) UF_free(objects);  
        return alive_curve;  
    }

```

#### 代码解析

> 这是段NX二次开发代码，主要用于检查和替换NX中的曲线。代码的主要功能包括：
>
> 1. 内存分配：使用allocate_memory函数为NX对象数组分配内存。
> 2. 对象查询：使用ask_all_of_type函数查询指定类型的对象，并返回对象数组。
> 3. 曲线匹配：使用do_curves_match函数判断两条曲线是否匹配，包括完全相同、反转副本和提取的样条线匹配。
> 4. 曲线替换：使用find_alive_curve函数查询与待删除曲线匹配的存活曲线，用于替换待删除曲线。
> 5. 对象类型查询：使用UF_OBJ_ask_type_and_subtype函数查询曲线的类型和子类型。
> 6. 距离公差查询：使用UF_MODL_ask_distance_tolerance函数查询NX的距离公差设置。
> 7. 撤销操作：使用UF_UNDO相关函数进行撤销操作。
> 8. NX API调用：使用UF_CALL宏进行NX API调用，简化错误处理。
> 9. 错误码输出：使用WRITENZ宏输出错误码。
>
> 总的来说，这段代码通过NX的二次开发接口，实现了曲线的查询、匹配和替换功能。
>
