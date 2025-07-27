### 【1446】report curve length 报告曲线长度

#### 代码

```cpp
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_curve_length(tag_t curve)  
    {  
        double  
            length;  
        UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, 1.0, UF_MODL_UNITS_PART,  
            &length));  
        WRITE_F(length);  
    }

```

#### 代码解析

> 这段代码是用于NX（Siemens NX）的二次开发代码，主要功能是计算并输出给定曲线的长度。
>
> 1. 定义了一个宏WRITE_F(X)，用于将变量X的值和名称输出到NX的listing窗口。这个宏通过调用write_double_to_listing_window函数来实现，该函数接受变量名和变量值，并输出到listing窗口。
> 2. write_double_to_listing_window函数用于将字符串标题和双精度浮点数变量值输出到NX的listing窗口。它首先调用UF_UI_open_listing_window()打开listing窗口，然后使用sprintf格式化字符串，最后调用UF_UI_write_listing_window()将字符串写入窗口。
> 3. report_curve_length函数用于计算并输出给定曲线的长度。它首先调用UF_CURVE_ask_arc_length()函数计算曲线的长度，然后调用WRITE_F宏将长度输出到listing窗口。其中，UF_CURVE_ask_arc_length()函数接受曲线标签、起始参数、结束参数、单位系统和长度变量地址。
>
> 总体来说，这段代码通过自定义宏和函数，实现了在NX中计算曲线长度并输出到listing窗口的功能。
>
