### 【2871】show edge points 显示边线上的点

#### 代码

```cpp
    static void show_edge_points(tag_t edge, int n_pts, double sp, double ep)  
    {  
        int  
            ii;  
        double  
            limits[2],  
            p,  
            point[3],  
            tep,  
            tsp;  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(edge, &eval));  
        UF_CALL(UF_EVAL_ask_limits(eval, limits));  
        tsp = limits[0] + sp * (limits[1] - limits[0]);  
        tep = limits[0] + ep * (limits[1] - limits[0]);  
        for (ii = 0; ii < n_pts; ii++)  
        {  
            p = tsp + ii * ((tep - tsp)/(n_pts - 1));  
            UF_CALL(UF_EVAL_evaluate(eval, 0, p, point, NULL));  
            UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
                UF_DISP_USE_ACTIVE_PLUS, point, &attrib, UF_DISP_POINT));  
        }  
        UF_CALL(UF_EVAL_free(eval));  
    }

```

#### 代码解析

> 这段代码是用于在NX中显示曲线的特定点。其主要功能如下：
>
> 1. 初始化一个UF_EVAL结构，该结构用于存储曲线的相关信息。
> 2. 获取曲线的参数范围，并计算起点和终点参数。
> 3. 循环遍历指定数量的点，计算每个点的参数，并使用UF_EVAL_evaluate函数在曲线上计算对应的点坐标。
> 4. 使用UF_DISP_display_temporary_point函数在曲线上显示这些点，并设置点的显示属性。
> 5. 释放UF_EVAL结构。
> 6. 参数sp和ep指定了曲线参数的起始和结束百分比，用于确定显示点的范围。
> 7. 参数n_pts指定了显示点的数量。
> 8. 函数使用静态方法调用，可以直接调用。
>
> 总的来说，这段代码实现了在NX中根据参数范围和点数显示曲线上的点，可用于二次开发中的特定需求。
>
