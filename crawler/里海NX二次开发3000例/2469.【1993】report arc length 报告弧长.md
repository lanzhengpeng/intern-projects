### 【1993】report arc length 报告弧长

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

> 这段代码是NX二次开发中的函数，用于计算和报告给定曲线的长度。以下是代码的主要内容和功能：
>
> 1. 定义了宏WRITE，用于打开NX的列表窗口并写入信息。
> 2. 定义了静态函数report_arc_length，接收一个曲线的tag作为参数。
> 3. 在函数内部，声明了变量length用于存储曲线长度，以及变量msg用于存储信息。
> 4. 调用UF_CURVE_ask_arc_length函数计算给定曲线在参数区间[0,1]内的长度，单位为零件单位。
> 5. 使用sprintf格式化字符串，将曲线的tag和长度拼接到信息字符串msg中。
> 6. 调用WRITE宏将信息写入NX的列表窗口。
> 7. UF_CALL宏用于处理函数调用，它会检查函数返回值，并在失败时输出错误信息。
> 8. UF_UI_open_listing_window和UF_UI_write_listing_window是NX的UI函数，用于打开和写入列表窗口。
> 9. UF_CURVE_ask_arc_length是NX的几何函数，用于计算曲线的长度。
>
> 综上所述，这段代码的主要功能是计算给定曲线的长度，并将结果报告在NX的列表窗口中。
>
