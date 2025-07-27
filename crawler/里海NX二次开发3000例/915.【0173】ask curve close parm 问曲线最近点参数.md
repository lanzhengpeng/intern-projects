### 【0173】ask curve close parm 问曲线最近点参数

#### 代码

```cpp
    static double ask_curve_close_parm(tag_t curve, double cp[3])  
    {  
        double  
            parm;  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(curve, &eval));  
        UF_CALL(UF_EVAL_evaluate_closest_point(eval, cp, &parm, cp));  
        UF_CALL(UF_EVAL_free(eval));  
        return parm;  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是获取曲线上与给定点最近的点的参数值。
>
> 具体步骤如下：
>
> 1. 定义函数ask_curve_close_parm，该函数接收曲线标签、参数数组cp[3]（存储给定点的坐标）以及返回参数值。
> 2. 在函数内部，首先定义并初始化UF_EVAL结构体，用于存储曲线的评估信息。
> 3. 调用UF_EVAL_evaluate_closest_point函数，计算与参数数组cp[3]中点坐标最接近的曲线上的点的参数值，并将其存储在参数数组cp中。
> 4. 返回计算得到的参数值。
> 5. 最后，释放UF_EVAL结构体占用的资源。
>
> 总的来说，这段代码通过UF_EVAL相关的函数，实现了在NX中获取曲线上与给定点最近的点的参数值的功能。
>
