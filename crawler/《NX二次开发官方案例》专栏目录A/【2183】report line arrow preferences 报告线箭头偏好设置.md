### 【2183】report line arrow preferences 报告线箭头偏好设置

#### 代码

```cpp
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %f\n", title, n);  
        ECHO(msg);  
    }  
    static void report_line_arrow_preferences(UF_DRF_line_arrow_preferences_t  
        *line_arrow_preferences)  
    {  
        WRITE_D(line_arrow_preferences->first_arrow_type);  
        WRITE_D(line_arrow_preferences->second_arrow_type);  
        WRITE_D(line_arrow_preferences->leader_location);  
        WRITE_F(line_arrow_preferences->arrowhead_length);  
        WRITE_F(line_arrow_preferences->arrowhead_included_angle);  
        WRITE_F(line_arrow_preferences->dot_arrowhead_diameter);  
        WRITE_F(line_arrow_preferences->stub_length);  
        WRITE_F(line_arrow_preferences->text_to_line_distance);  
        WRITE_F(line_arrow_preferences->line_past_arrow_distance);  
        WRITE_F(line_arrow_preferences->oblique_extension_line_angle);  
        WRITE_F(line_arrow_preferences->first_pos_to_extension_line_distance);  
        WRITE_F(line_arrow_preferences->second_pos_to_extension_line_distance);  
        WRITE_F(line_arrow_preferences->datum_length_past_arrow);  
        WRITE_F(line_arrow_preferences->text_over_stub_factor);  
        WRITE_D(line_arrow_preferences->first_extension_line_cfw.color);  
        WRITE_D(line_arrow_preferences->first_extension_line_cfw.font);  
        WRITE_D(line_arrow_preferences->first_extension_line_cfw.width);  
        WRITE_D(line_arrow_preferences->first_arrowhead_cfw.color);  
        WRITE_D(line_arrow_preferences->first_arrowhead_cfw.font);  
        WRITE_D(line_arrow_preferences->first_arrowhead_cfw.width);  
        WRITE_D(line_arrow_preferences->first_arrow_line_cfw.color);  
        WRITE_D(line_arrow_preferences->first_arrow_line_cfw.font);  
        WRITE_D(line_arrow_preferences->first_arrow_line_cfw.width);  
        WRITE_D(line_arrow_preferences->second_extension_line_cfw.color);  
        WRITE_D(line_arrow_preferences->second_extension_line_cfw.font);  
        WRITE_D(line_arrow_preferences->second_extension_line_cfw.width);  
        WRITE_D(line_arrow_preferences->second_arrowhead_cfw.color);  
        WRITE_D(line_arrow_preferences->second_arrowhead_cfw.font);  
        WRITE_D(line_arrow_preferences->second_arrowhead_cfw.width);  
        WRITE_D(line_arrow_preferences->second_arrow_line_cfw.color);  
        WRITE_D(line_arrow_preferences->second_arrow_line_cfw.font);  
        WRITE_D(line_arrow_preferences->second_arrow_line_cfw.width);  
        WRITE_F(line_arrow_preferences->arrow_out_length_factor);  
    }

```

#### 代码解析

> 这段代码是用于NX CAD系统的二次开发，主要功能是报告直线箭头的参数设置。它包括以下关键部分：
>
> 1. 定义了两个宏WRITE_D和WRITE_F，用于格式化打印整数和浮点数。
> 2. 定义了两个函数write_integer_to_listing_window和write_double_to_listing_window，用于向列表窗口打印整数和浮点数。
> 3. 定义了一个函数report_line_arrow_preferences，用于报告直线箭头参数。它遍历直线箭头参数结构体中的每个字段，并使用WRITE_D和WRITE_F宏打印出每个字段的名称和值。
> 4. 直线箭头参数结构体包括箭头类型、位置、长度、角度、直径、文本距离等参数。这些参数控制着NX中直线箭头的显示样式。
> 5. 该代码通过遍历打印所有直线箭头参数，可以方便地查看和调试当前的直线箭头设置，对二次开发调试非常有帮助。
> 6. 使用宏和函数封装，代码更加简洁易读，提高了开发效率。
>
