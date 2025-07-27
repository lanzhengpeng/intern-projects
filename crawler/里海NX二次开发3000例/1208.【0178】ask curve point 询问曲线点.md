### 【0178】ask curve point 询问曲线点

#### 代码

```cpp
    static void ask_curve_point(tag_t curve, double parm, double point[3])  
    {  
        double  
            limits[2];  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(curve, &eval));  
        UF_CALL(UF_EVAL_ask_limits(eval, limits));  
        UF_CALL(UF_EVAL_evaluate(eval, 0,  
            limits[0] + ((limits[1] - limits[0]) * parm), point, NULL));  
        UF_CALL(UF_EVAL_free(eval));  
    }

```

#### 代码解析

> 这段代码实现了在NX中根据曲线参数化表达式和给定参数值，求出曲线上的点的功能。
>
> 具体步骤如下：
>
> 1. 首先调用UF_EVAL_initialize()函数，初始化评估器，参数为曲线对象和评估器对象。
> 2. 然后调用UF_EVAL_ask_limits()函数，获取曲线的参数化范围，结果存储在limits数组中。
> 3. 接着调用UF_EVAL_evaluate()函数，根据给定的参数值(在参数化范围内按比例插值)，计算曲线上的点的坐标，结果存储在point数组中。
> 4. 最后调用UF_EVAL_free()函数，释放评估器对象。
>
> 整体上，该段代码利用NX提供的曲线评估函数，实现了根据参数值求取曲线上的点的功能。
>
