### 【0176】ask curve midpoint 求曲线中点

#### 代码

```cpp
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

```

#### 代码解析

> 这段代码用于计算NX曲线的中点，其关键步骤包括：
>
> 1. 使用UF_UNDO_set_mark函数创建一个不可见的撤销标记。
> 2. 使用UF_SO_create_scalar_double和UF_SO_create_point_on_curve创建曲线上的第一个点。
> 3. 使用UF_SO_set_visibility_option将这个点设置为不可见。
> 4. 使用UF_SO_create_scalar_double和UF_SO_create_point_along_curve创建曲线上的第二个点，该点位于曲线长度的50%处。
> 5. 使用UF_CURVE_ask_point_data获取这个点的坐标。
> 6. 使用UF_UNDO_undo_to_mark删除这个点，并恢复到撤销标记的状态。
> 7. 最后使用UF_UNDO_delete_mark删除撤销标记。
>
> 通过以上步骤，该代码成功地计算出曲线的中点坐标，并实现了撤销操作，以保持模型的原始状态。
>
