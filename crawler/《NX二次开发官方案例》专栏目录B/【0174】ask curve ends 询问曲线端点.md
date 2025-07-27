### 【0174】ask curve ends 询问曲线端点

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

```

#### 代码解析

> 这段代码实现了在NX中查询曲线端点坐标的功能。
>
> 主要步骤包括：
>
> 1. 初始化曲线的评估器UF_EVAL_p_t。
> 2. 调用UF_EVAL_ask_limits()获取曲线的参数范围limits[0]和limits[1]。
> 3. 调用UF_EVAL_evaluate()在limits[0]处计算曲线的起点坐标start。
> 4. 调用UF_EVAL_evaluate()在limits[1]处计算曲线的终点坐标end。
> 5. 释放评估器资源。
>
> 通过以上步骤，可以高效地获取给定曲线的起点和终点坐标。
>
