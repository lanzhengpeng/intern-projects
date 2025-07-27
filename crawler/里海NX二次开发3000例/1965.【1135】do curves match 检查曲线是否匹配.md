### 【1135】do curves match 检查曲线是否匹配

#### 代码

```cpp
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
    /*  Check for exact copy 译:检查确切复制。 */  
        UF_CALL(UF_EVAL_initialize(curve1, &eval_p1));  
        UF_CALL(UF_EVAL_initialize(curve2, &eval_p2));  
        UF_CALL(UF_EVAL_is_equal(eval_p1, eval_p2, &is_equal));  
        if (is_equal)  
        {  
            UF_CALL(UF_EVAL_free(eval_p1));  
            UF_CALL(UF_EVAL_free(eval_p2));  
            return TRUE;  
        }  
    /*  Check for a reversed copy 译:根据文档，我进行了检查，发现文档并未被反转。 */  
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

```

#### 代码解析

> 这段代码是NX二次开发代码，用于判断两条曲线是否匹配。
>
> 主要步骤包括：
>
> 1. 检查两条曲线是否完全相同。
> 2. 检查一条曲线是否是另一条曲线的反向复制。
> 3. 如果上述检查都不满足，提取两条曲线的样条部分，比较样条部分是否相同。
>
> 在匹配过程中，使用了UF_EVAL模块来评估和比较曲线，UF_UNDO模块来保存和恢复状态。代码中用到了距离容差dist_tol来决定两条曲线是否足够接近。
>
> 此外，代码中使用了UF_CALL宏来简化函数调用，以及WRITENZ宏来输出错误码。通过这些宏，使得代码更加简洁和易于阅读。
>
> 综上所述，这段代码实现了判断两条曲线是否匹配的功能，主要应用于NX CAD系统中的曲线处理。
>
