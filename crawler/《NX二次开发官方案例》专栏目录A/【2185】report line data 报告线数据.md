### 【2185】report line data 报告线数据

#### 代码

```cpp
    static void report_line_data(tag_t line)  
    {  
        double  
            length;  
        UF_CURVE_line_t  
            line_data;  
        UF_CALL(UF_CURVE_ask_line_data(line, &line_data));  
        WRITE3F(line_data.start_point);  
        WRITE3F(line_data.end_point);  
        UF_VEC3_distance(line_data.start_point, line_data.end_point, &length);  
        WRITE_F(length);  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API的二次开发代码，主要功能是报告给定线段的数据。代码的主要步骤如下：
>
> 1. 定义了一个静态函数report_line_data，用于报告线段数据。
> 2. 在函数内部，首先定义了变量length，用于存储线段的长度。
> 3. 定义了结构体变量line_data，用于存储线段数据。
> 4. 调用UF_CURVE_ask_line_data函数，获取传入参数line所表示线段的数据，并存储到line_data变量中。
> 5. 调用WRITE3F宏，打印出线段的起点坐标。
> 6. 调用WRITE3F宏，打印出线段的终点坐标。
> 7. 调用UF_VEC3_distance函数，计算线段的长度，并存储到length变量中。
> 8. 调用WRITE_F宏，打印出线段的长度。
>
> 综上所述，这段代码通过获取线段数据，计算线段长度，并打印出线段起点、终点坐标以及长度，实现了报告线段数据的功能。
>
