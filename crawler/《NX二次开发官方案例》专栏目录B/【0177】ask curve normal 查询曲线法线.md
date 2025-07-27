### 【0177】ask curve normal 查询曲线法线

#### 代码

```cpp
    static void ask_curve_normal(tag_t curve, double loc[3], double normal[3])  
    {  
        double  
            binormal[3],  
            parm,  
            point[3],  
            tangent[3];  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(curve, &eval));  
        UF_CALL(UF_EVAL_evaluate_closest_point(eval, loc, &parm, point));  
        UF_CALL(UF_EVAL_evaluate_unit_vectors(eval, parm, point, tangent,  
            normal, binormal));  
        UF_CALL(UF_EVAL_free(eval));  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，用于计算曲线上某一点的法向量。以下是该代码的主要功能和步骤：
>
> 1. 定义一个静态函数ask_curve_normal，接受曲线标签、点的坐标和输出的法向量。
> 2. 定义局部变量，包括切向量、参数值、点的坐标和副法向量。
> 3. 初始化曲线的评估器eval。
> 4. 计算输入点在曲线上的最近点，并获取对应的参数值和点的坐标。
> 5. 计算曲线在指定参数值处的单位切向量、法向量和副法向量。
> 6. 释放评估器eval。
> 7. 输出法向量。
>
> 通过这段代码，可以在NX中实现根据曲线上的点坐标计算法向量的功能，为后续的曲面建模、计算等提供了基础支持。
>
