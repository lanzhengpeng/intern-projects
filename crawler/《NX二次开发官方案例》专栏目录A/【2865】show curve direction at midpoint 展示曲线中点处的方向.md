### 【2865】show curve direction at midpoint 展示曲线中点处的方向

#### 代码

```cpp
    static void display_colored_conehead(double origin[3], double vec[3], int color)  
    {  
        UF_DISP_conehead_attrb_s  
            attrb;  
        UF_DISP_get_conehead_attrb(&attrb);  
        attrb.color = color;  
        UF_DISP_set_conehead_attrb(&attrb);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_DISP_reset_conehead_attrb();  
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
    static void ask_curve_direction(tag_t curve, double loc[3], double dir[3])  
    {  
        double  
            binormal[3],  
            normal[3],  
            parm,  
            point[3];  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(curve, &eval));  
        UF_CALL(UF_EVAL_evaluate_closest_point(eval, loc, &parm, point));  
        UF_CALL(UF_EVAL_evaluate_unit_vectors(eval, parm, point, dir, normal,  
            binormal));  
        UF_CALL(UF_EVAL_free(eval));  
    }  
    static void show_curve_direction_at_midpoint(tag_t curve)  
    {  
        double  
            start[3],  
            tangent[3];  
        ask_curve_midpoint(curve, start);  
        ask_curve_direction(curve, start, tangent);  
        display_colored_conehead(start, tangent, UF_OBJ_BLUE);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要用于NX的二次开发。
>
> 代码主要包含以下几个函数：
>
> 1. display_colored_conehead()：用于在NX图形窗口中显示一个带有颜色属性的小圆锥体，以表示曲线的方向。
> 2. ask_curve_midpoint()：用于计算给定曲线的中点坐标。
> 3. ask_curve_direction()：用于计算曲线在指定点处的切线方向。
> 4. show_curve_direction_at_midpoint()：综合使用上述函数，计算曲线的中点坐标和在该点处的切线方向，并在图形窗口中显示表示该切线方向的蓝色小圆锥体。
>
> 通过使用这些函数，可以直观地显示曲线的切线方向，对理解曲线形状和走向非常有帮助。
>
