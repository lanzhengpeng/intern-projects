### 【2006】report banner options 报告横幅选项

#### 代码

```cpp
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_banner_options(UF_PLOT_banner_options_p_t banner_options)  
    {  
        WRITE_L(banner_options->show_banner);  
        if ((banner_options->show_banner) && (banner_options->message != NULL))  
        {  
            WRITE_S(banner_options->message);  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了在NX的列表窗口中输出变量值的功能。
>
> 主要特点如下：
>
> 1. 使用宏定义WRITE_L和WRITE_S，简化了在列表窗口输出逻辑值和字符串的代码。
> 2. write_logical_to_listing_window函数负责输出逻辑值，通过判断变量值是TRUE还是FALSE，并格式化输出。
> 3. write_string_to_listing_window函数负责输出字符串，同样通过格式化输出字符串。
> 4. report_banner_options函数通过WRITE_L和WRITE_S宏，输出banner_options结构体的show_banner和message字段。
> 5. 使用了NX提供的UI函数UF_UI_open_listing_window和UF_UI_write_listing_window，实现了与NX列表窗口的交互。
> 6. 通过宏定义和函数封装，使得在列表窗口输出变量变得更加简单和可维护。
> 7. 该代码片段展示了NX二次开发中与UI交互的基本方法。
>
> 总的来说，这段代码实现了在NX的列表窗口中输出变量值的功能，采用了宏定义和函数封装，使得代码更加简洁和可维护。
>
