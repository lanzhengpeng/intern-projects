### 【0235】ask helix origin and direction 询问螺旋的原始点和方向

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
    static void ask_curve_midpoint(tag_t curve, double midpoint[3])  
    {  
        tag_t  
            last,  
            offset,  
            start;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.0, &start));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
            curve, start, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_invisible));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.5, &offset));  
        UF_CALL(UF_SO_create_point_along_curve(curve, UF_SO_update_after_modeling,  
            curve, last, offset, UF_SO_point_along_curve_percent, FALSE, &last));  
        UF_CALL(UF_CURVE_ask_point_data(last, midpoint));  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
    }  
    static logical ask_helix_origin_and_direction(tag_t helix_feature,  
        double origin[3], double direction[3])  
    {  
        int  
            ii,  
            n_curves,  
            n_exps,  
            whole_turns;  
        tag_t  
            *curves,  
            *exps,  
            plane,  
            point,  
            radius_exp = NULL_TAG,  
            turns_exp = NULL_TAG;  
        double  
            center[3],  
            dist,  
            end[3],  
            junk[3],  
            midpoint[3],  
            n_turns,  
            start[3];  
        char  
            **descs,  
            *lhs,  
            new_exp[UF_MAX_EXP_LENGTH+1],  
            *rhs,  
            *string;  
        UF_UNDO_mark_id_t  
            mark_id;  
        if (UF_CALL(UF_MODL_ask_exp_desc_of_feat(helix_feature, &n_exps, &descs,  
            &exps))) return FALSE;  
        for (ii = 0; ii < n_exps; ii++)  
        {  
            if (!strcmp(descs[ii], "Radius")) radius_exp = exps[ii];  
            if (!strcmp(descs[ii], "Number of Turns")) turns_exp = exps[ii];  
        }  
        if (n_exps > 0)  
        {  
            UF_free(exps);  
            UF_free_string_array(n_exps, descs);  
        }  
        if ((radius_exp == NULL_TAG) || (turns_exp == NULL_TAG)) return FALSE;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_MODL_ask_exp_tag_value(turns_exp, &n_turns));  
        whole_turns = (int) n_turns;  
        if (n_turns != whole_turns)  
        {  
            if (whole_turns < 1) whole_turns = 1;  
            UF_CALL(UF_MODL_ask_exp_tag_string(turns_exp, &string));  
            UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs,  
                &turns_exp));  
            sprintf(new_exp, "%s=%d", lhs, whole_turns);  
            UF_CALL(UF_MODL_edit_exp(new_exp));  
            UF_CALL(UF_MODL_update());  
            UF_free(string);  
            UF_free(lhs);  
            UF_free(rhs);  
        }  
        UF_CALL(UF_MODL_ask_feat_object(helix_feature, &n_curves, &curves));  
        ask_curve_ends(curves[0], start, end);  
        ask_curve_midpoint(curves[0], midpoint);  
        UF_free(curves);  
        UF_VEC3_sub(end, start, direction);  
        UF_VEC3_midpt(start, midpoint, center);  
        UF_CALL(UF_CURVE_create_point(center, &point));  
        UF_CALL(UF_MODL_create_plane(start, direction, &plane));  
        UF_CALL(UF_MODL_ask_minimum_dist(plane, point, FALSE, NULL, FALSE,  
            NULL, &dist, origin, junk));  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
        return TRUE;  
    }

```

#### 代码解析

> 这段代码主要用于提取螺旋特征的关键参数，包括螺旋的起始点、方向、半径和圈数。其主要功能包括：
>
> 1. 提取螺旋的起始点和终止点，通过计算曲线的两个端点来获取。
> 2. 计算螺旋的中点，通过创建点沿曲线的功能来实现。
> 3. 确定螺旋的半径和圈数，通过查找表达式来获取。
> 4. 计算螺旋的轴线方向，通过起始点和终止点来计算。
> 5. 确定螺旋的中心点，通过计算起始点和中点之间的中点来获取。
> 6. 计算螺旋的起始点，通过创建一个平面，并计算点到平面的最短距离来获取。
> 7. 使用UNDO标记来撤销临时创建的对象。
> 8. 返回螺旋的起始点和方向。
>
> 总的来说，这段代码通过NX提供的API，实现了对螺旋特征参数的提取，为后续的螺旋建模或其他处理提供了基础。
>
