### 【1750】plot current display nx3 NX3当前显示的绘图

#### 代码

```cpp
    /*HEAD PLOT_CURRENT_DISPLAY_NX3 CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plot.h>  
    #include <uf_draw.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数，用于打印系统日志信息。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
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
    static void report_banner_options(UF_PLOT_banner_options_p_t banner_options)  
    {  
        WRITE_L(banner_options->show_banner);  
        if ((banner_options->show_banner) && (banner_options->message != NULL))  
        {  
            WRITE_S(banner_options->message);  
        }  
    }  
    static void do_it(void)  
    {  
        char  
            *job_name,  
            *printer,  
            *profile;  
        UF_PLOT_job_options_t  
            job_options;  
        UF_PLOT_banner_options_t  
            banner_options;  
        UF_CALL(UF_PLOT_ask_session_job_options(&job_options));  
        report_job_options(&job_options);  
        UF_CALL(UF_PLOT_ask_session_banner_options(&banner_options));  
        report_banner_options(&banner_options);  
        UF_CALL(UF_PLOT_ask_default_printer_and_profile( &printer, &profile));  
        WRITE_S(printer);  
        WRITE_S(profile);  
        UF_CALL(UF_PLOT_ask_default_job_name(NULL_TAG, &job_name));  
        WRITE_S(job_name);  
        UF_CALL(UF_PLOT_print(NULL_TAG, &job_options, job_name, &banner_options,  
            printer, profile, 1));  
        UF_free(job_name);  
        UF_free(printer);  
        UF_free(profile);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息到系统日志和列表窗口。
> 2. 定义了一些宏，用于将变量值打印到列表窗口，如WRITE、WRITE_L、WRITE_D、WRITE_F、WRITE_S。
> 3. 定义了两个函数，report_job_options和report_banner_options，分别用于打印绘图作业选项和横幅选项。
> 4. 定义了主函数do_it，用于获取当前的绘图作业选项、横幅选项、默认打印机、默认打印配置文件、默认作业名，并将这些信息打印到列表窗口。然后调用UF_PLOT_print函数进行绘图。
> 5. 定义了ufusr函数，用于初始化和终止NX Open API，并调用do_it函数。
> 6. 定义了ufusr_ask_unload函数，用于卸载当前加载的库。
>
> 综上所述，这段代码的主要功能是获取当前NX的绘图设置，并将这些设置打印出来，然后进行一次绘图。通过这段代码，开发者可以了解如何使用NX Open API获取和设置绘图参数，以及进行绘图操作。
>
