### 【0172】ask curve close endpoint 询问曲线靠近的端点

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
    static void ask_curve_close_endpoint(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            ends[6];  
        ask_curve_ends(curve, ends);  
        UF_VEC3_distance(pos, ends, &dist[0]);  
        UF_VEC3_distance(pos, &ends[3], &dist[1]);  
        if (dist[0] < dist[1]) UF_VEC3_copy(ends, pos);  
        else UF_VEC3_copy(&ends[3], pos);  
    }

```

#### 代码解析

> 这段代码用于NX Open的二次开发，主要实现了以下功能：
>
> 1. ask_curve_ends函数：获取曲线的起点和终点坐标。首先通过UF_EVAL_initialize初始化曲线的评估器，然后通过UF_EVAL_ask_limits获取曲线的参数范围，接着使用UF_EVAL_evaluate计算曲线起点和终点的坐标，并将结果存储在start和end数组中。最后，释放评估器。
> 2. ask_curve_close_endpoint函数：获取曲线距离给定点最近的端点坐标。首先调用ask_curve_ends获取曲线的起点和终点坐标，然后通过UF_VEC3_distance计算给定点与起点和终点的距离。最后，将距离最近的端点坐标复制到pos数组中。
> 3. 通过UF_CALL宏，封装了UF函数调用，实现了错误处理，并在函数最后释放了评估器资源。
>
> 总体来说，这段代码通过NX提供的评估器API，实现了曲线端点的获取以及距离给定点最近的端点坐标的获取。
>
