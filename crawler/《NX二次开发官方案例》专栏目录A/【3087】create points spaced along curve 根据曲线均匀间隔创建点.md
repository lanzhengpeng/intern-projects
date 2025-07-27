### 【3087】create points spaced along curve 根据曲线均匀间隔创建点

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
    static int create_points_spaced_along_curve(tag_t curve, double increment,  
        UF_SO_point_along_curve_option_t dist_vs_pct, tag_t **points)  
    {  
        int  
            n_points;  
        tag_t  
            last,  
            offset,  
            start;  
        double  
            step,  
            total_length;  
        uf_list_p_t  
            point_list;  
        UF_CALL(UF_MODL_create_list(&point_list));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.0, &start));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
            curve, start, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
        UF_CALL(UF_MODL_put_list_item(point_list, last));  
        switch (dist_vs_pct)  
        {  
            case UF_SO_point_along_curve_distance:  
                UF_CALL(UF_SO_create_scalar_double(curve,  
                    UF_SO_update_after_modeling, increment, &offset));  
                UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, 1.0,  
                    UF_MODL_UNITS_PART, &total_length));  
                for (step = increment; step < total_length; step = step+increment)  
                {  
                    UF_CALL(UF_SO_create_point_along_curve(curve,  
                        UF_SO_update_after_modeling, curve, last, offset,  
                        UF_SO_point_along_curve_distance, FALSE, &last));  
                    UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
                    UF_CALL(UF_MODL_put_list_item(point_list, last));  
                }  
                break;  
            case UF_SO_point_along_curve_percent:  
                UF_CALL(UF_SO_create_scalar_double(curve,  
                    UF_SO_update_after_modeling, increment/100, &offset));  
                for (step = increment; step <= 100; step = step + increment)  
                {  
                    UF_CALL(UF_SO_create_point_along_curve(curve,  
                        UF_SO_update_after_modeling, curve, last, offset,  
                        UF_SO_point_along_curve_percent, FALSE, &last));  
                    UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
                    UF_CALL(UF_MODL_put_list_item(point_list, last));  
                }  
                break;  
            default:  
                break;  
        }  
        UF_CALL(UF_MODL_delete_object_parms(point_list));  
        return n_points = make_an_array(&point_list, points);  
    }

```

#### 代码解析

> 这段NX Open API代码用于在曲线上按一定距离或百分比间隔创建点。
>
> 主要步骤包括：
>
> 1. 初始化变量：创建一个点列表，并初始化起点为曲线的起始点。
> 2. 根据参数选择创建点的方式：若选择按距离创建点，则循环调用UF_SO_create_point_along_curve函数，每次按给定的距离增量在曲线上创建一个点，并添加到点列表中。若选择按百分比创建点，则循环调用UF_SO_create_point_along_curve函数，每次按给定的百分比增量在曲线上创建一个点，并添加到点列表中。
> 3. 若选择按距离创建点，则循环调用UF_SO_create_point_along_curve函数，每次按给定的距离增量在曲线上创建一个点，并添加到点列表中。
> 4. 若选择按百分比创建点，则循环调用UF_SO_create_point_along_curve函数，每次按给定的百分比增量在曲线上创建一个点，并添加到点列表中。
> 5. 处理点列表：删除点列表的参数，并将点列表转换为点数组。
> 6. 返回点数组的长度。
> 7. 释放点列表：在创建点数组后，释放点列表。
>
> 总的来说，这段代码实现了在曲线上按距离或百分比间隔创建点的功能，并返回了点数组。代码中使用了NX Open API中的建模和参数化建模函数。
>
