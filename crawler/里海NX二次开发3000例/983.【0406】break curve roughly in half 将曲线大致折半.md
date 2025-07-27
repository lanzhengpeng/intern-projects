### 【0406】break curve roughly in half 将曲线大致折半

#### 代码

```cpp
    static tag_t break_curve_roughly_in_half(tag_t curve)  
    {  
        tag_t  
            feat,  
            new_curve,  
            point;  
        double  
            mid_parm,  
            mid_point[3],  
            quarter_parm,  
            quarter_point[3],  
            three_quarter_parm,  
            three_quarter_point[3],  
            limits[2];  
        uf_list_p_t  
            list;  
        UF_EVAL_p_t  
            evaluator;  
    /*  Create a dumb copy of the curve 译:创建曲线的哑副本 */  
        UF_CALL(UF_WAVE_create_linked_curve(curve, NULL_TAG, curve, FALSE, &feat));  
        UF_CALL(UF_WAVE_ask_linked_feature_geom(feat, &new_curve));  
        UF_CALL(UF_MODL_create_list(&list));  
        UF_CALL(UF_MODL_put_list_item(list, new_curve));  
        UF_CALL(UF_MODL_delete_object_parms(list));  
        UF_CALL(UF_MODL_delete_list(&list));  
        UF_CALL(UF_EVAL_initialize(curve, &evaluator));  
        UF_CALL(UF_EVAL_ask_limits(evaluator, limits));  
        mid_parm = (limits[0] + limits[1]) / 2;  
        quarter_parm = (limits[0] + mid_parm) / 2;  
        three_quarter_parm = (mid_parm + limits[1]) / 2;  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, mid_parm, mid_point, NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, quarter_parm, quarter_point, NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, three_quarter_parm,  
            three_quarter_point, NULL));  
        UF_CALL(UF_EVAL_free(evaluator));  
        UF_CALL(UF_CURVE_create_point(mid_point, &point));  
        UF_CALL(UF_CURVE_edit_trim_curve(curve, point, three_quarter_point,  
            mid_point, UF_CURVE_NATURAL_SHAPE));  
        UF_CALL(UF_CURVE_edit_trim_curve(new_curve, point, quarter_point,  
            mid_point, UF_CURVE_NATURAL_SHAPE));  
        UF_CALL(UF_OBJ_delete_object(point));  
        return new_curve;  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，主要功能是将一个给定的曲线大致地分成两段。
>
> 主要步骤如下：
>
> 1. 创建曲线的哑副本，并获取副本的标识。
> 2. 初始化曲线的评估器，并获取曲线的参数范围。
> 3. 计算曲线中点、1/4点和3/4点的参数，并评估这些参数点对应的几何位置。
> 4. 创建中点处的点对象。
> 5. 使用中点和1/4点、3/4点，通过编辑曲线的修剪功能，将原始曲线和新曲线分别修剪为两段。
> 6. 删除中点处的点对象。
> 7. 返回新曲线的标识。
>
> 总体来说，这段代码通过评估曲线的参数位置，创建点对象，并通过修剪曲线的功能，实现了将曲线大致分成两段的目的。
>
