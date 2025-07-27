### 【2165】report job options 报告作业选项

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
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
    static void report_job_options(UF_PLOT_job_options_p_t job_options)  
    {  
        switch (job_options->colors)  
        {  
            case UF_PLOT_AS_DISPLAYED_COLORS:  
                WRITE("job_options->colors= UF_PLOT_AS_DISPLAYED_COLORS\n");  
                break;  
            case UF_PLOT_PART_COLORS:  
                WRITE("job_options->colors= UF_PLOT_PART_COLORS\n");  
                break;  
            case UF_PLOT_CUSTOM_PALETTE_COLORS:  
                WRITE("job_options->colors= UF_PLOT_CUSTOM_PALETTE_COLORS\n");  
                break;  
            case UF_PLOT_BLACK_ON_WHITE:  
                WRITE("job_options->colors= UF_PLOT_BLACK_ON_WHITE\n");  
                break;  
            case UF_PLOT_LEGACY_COLORS:  
                WRITE("job_options->colors= UF_PLOT_LEGACY_COLORS\n");  
                break;  
            case UF_PLOT_COLOR_BY_WIDTH:  
                WRITE("job_options->colors= UF_PLOT_COLOR_BY_WIDTH\n");  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(job_options->colors);  
                break;  
        }  
        WRITE_L(job_options->use_drawing_sheet_colors);  
        switch (job_options->widths)  
        {  
            case UF_PLOT_STANDARD_WIDTHS:  
                WRITE("job_options->widths= UF_PLOT_STANDARD_WIDTHS\n");  
                break;  
            case UF_PLOT_SINGLE_WIDTH:  
                WRITE("job_options->widths= UF_PLOT_SINGLE_WIDTH\n");  
                break;  
            case UF_PLOT_CUSTOM_3_WIDTHS:  
                WRITE("job_options->widths= UF_PLOT_CUSTOM_3_WIDTHS\n");  
                break;  
            case UF_PLOT_CUSTOM_PALETTE_WIDTHS:  
                WRITE("job_options->widths= UF_PLOT_CUSTOM_PALETTE_WIDTHS\n");  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(job_options->widths);  
                break;  
        }  
        WRITE_L(job_options->use_drawing_sheet_widths);  
        WRITE_F(job_options->tolerance);  
    }

```

#### 代码解析

> 这段代码是用于NX Open二次开发，目的是向列表窗口输出绘图作业选项。主要功能如下：
>
> 1. 定义了三个宏WRITE、WRITE_L和WRITE_D，用于向列表窗口输出不同类型的信息。
> 2. 定义了三个静态函数，分别用于向列表窗口输出逻辑值、整数和浮点数。
> 3. 定义了一个函数report_job_options，用于向列表窗口输出绘图作业选项的详细信息。
> 4. report_job_options函数根据作业选项的colors和widths字段，使用switch语句输出不同选项的文本描述，对于未知选项，输出"Unknown"和字段的整数值。
> 5. 对于逻辑字段use_drawing_sheet_colors和use_drawing_sheet_widths，直接输出其值。
> 6. 对于浮点字段tolerance，输出其值。
> 7. 在函数中大量使用UF_CALL宏来调用NX API函数，以确保异常安全。
> 8. 使用宏简化了输出不同类型信息的代码。
>
> 总的来说，这段代码实现了根据NX绘图作业选项结构体，输出选项信息到列表窗口的功能。通过宏和函数封装，使输出代码更简洁和易于维护。
>
