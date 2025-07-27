### 【0175】ask curve length 问曲线长度

#### 代码

```cpp
    static double ask_curve_length(tag_t curve)  
    {  
        double  
            length;  
        UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, 1.0, UF_MODL_UNITS_PART,  
            &length));  
        return length;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发函数，其功能是计算给定曲线的长度。
>
> 函数名为ask_curve_length，参数为curve，代表需要计算长度的曲线对象。函数使用UF_CURVE_ask_arc_length函数来计算曲线长度，其中参数0.0和1.0分别代表曲线的起始和终止位置参数。UF_MODL_UNITS_PART参数表示使用零件的单位制。计算出的长度值存储在length变量中，并最终返回该值。
>
> 总体来说，该函数通过NX的UF_CURVE_ask_arc_length函数实现了计算曲线长度的功能。它封装了曲线长度计算的复杂逻辑，并返回了长度值，便于在NX二次开发中使用。
>
