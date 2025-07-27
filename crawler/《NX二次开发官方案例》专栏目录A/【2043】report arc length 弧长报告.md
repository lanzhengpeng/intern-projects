### 【2043】report arc length 弧长报告

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); (UF_UI_write_listing_window(X))  
    static void report_arc_length(tag_t curve)  
    {  
        double  
            length;  
        char  
            msg[133];  
        UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, 1.0, UF_MODL_UNITS_PART,  
            &length));  
        sprintf(msg, "Curve tag %d has length: %f\n", curve, length);  
        WRITE(msg);  
    }

```

#### 代码解析

> 这段代码是NX Open的二次开发代码，其主要功能是计算并报告指定弧线的长度。
>
> 具体步骤如下：
>
> 1. 定义了一个宏WRITE，用于打开NX的列表窗口并写入信息。
> 2. 定义了一个静态函数report_arc_length，用于计算并报告指定弧线的长度。
> 3. 在report_arc_length函数中，首先调用UF_CURVE_ask_arc_length函数计算弧线在0到1参数范围内的长度，单位为零件单位。
> 4. 将计算得到的长度和弧线标签拼接成字符串，并通过WRITE宏写入NX的列表窗口。
> 5. 用户可以调用report_arc_length函数来报告指定弧线的长度。
>
> 总体来说，这段代码实现了弧线长度的计算和报告功能，适用于NX的二次开发。
>
