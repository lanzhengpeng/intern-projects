### 【2049】report custom widths 报告自定义宽度

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_custom_widths(UF_PLOT_custom_widths_p_t custom_widths)  
    {  
        int ii;  
        switch (custom_widths->units)  
        {  
            case UF_PART_METRIC:  
                WRITE("custom_widths->units = UF_PART_METRIC\n");  
                break;  
            case UF_PART_ENGLISH:  
                WRITE("custom_widths->units = UF_PART_ENGLISH\n");  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(custom_widths->units);  
                break;  
        }  
        switch (custom_widths->use)  
        {  
            case UF_CGM_width_single:  
                WRITE("custom_widths->use = UF_CGM_width_single\n");  
                switch (custom_widths->single)  
                {  
                    case UF_CGM_width_std:  
                        WRITE("custom_widths->single = UF_CGM_width_std\n");  
                        break;  
                    case UF_CGM_width_custom:  
                        WRITE("custom_widths->single = UF_CGM_width_custom\n");  
                        break;  
                    case UF_CGM_width_user:  
                        WRITE("custom_widths->single = UF_CGM_width_user\n");  
                        break;  
                    default:  
                        WRITE("Unknown ");  
                        WRITE_D(custom_widths->single);  
                        break;  
                }  
                WRITE_F(custom_widths->single_width);  
                WRITE_D(custom_widths->single_source);  
                break;  
            case UF_CGM_width_by_width:  
                WRITE("custom_widths->use = UF_CGM_width_by_width\n");  
                for (ii = 0; ii < UF_CGM_MAX_WIDTHS; ii++)  
                {  
                    WRITE_D(ii);  
                    WRITE_F(custom_widths->custom[ii].width);  
                    WRITE_S(custom_widths->custom[ii].name);  
                }  
                break;  
            case UF_CGM_width_by_color:  
                WRITE("custom_widths->use = UF_CGM_width_by_color\n");  
                for (ii = 0; ii < UF_CGM_MAX_COLORS; ii++)  
                {  
                    WRITE_D(ii);  
                    WRITE_F(custom_widths->color[ii].width);  
                    WRITE_D(custom_widths->color[ii].width_source);  
                }  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(custom_widths->use);  
                break;  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是输出绘图设置的自定义线宽信息。
>
> 1. 首先，定义了几个宏，用于输出不同类型的信息到NX的日志窗口。例如，WRITE用于输出字符串，WRITE_D用于输出整数，WRITE_F用于输出浮点数，WRITE_S用于输出字符串。
> 2. 接着，定义了几个函数，用于根据不同类型的数据，格式化输出信息到日志窗口。这些函数分别用于整数、浮点数和字符串。
> 3. 最后，定义了report_custom_widths函数，该函数接收一个自定义线宽结构体，并输出该结构体的所有信息到日志窗口。
>
> 具体来说，该函数会输出单位制信息，线宽使用方式，如果是单一线宽，则输出线宽值和来源；如果是按宽度设置，则输出每个宽度的值和名称；如果是按颜色设置，则输出每个颜色的线宽值和来源。
>
> 总之，这段代码实现了读取和输出NX绘图设置中的自定义线宽信息的功能，方便用户查看当前的自定义线宽设置。
>
