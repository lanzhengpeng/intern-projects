### 【1751】plot current drawing nx3 NX3 当前绘图打印

#### 代码

```cpp
    /*HEAD PLOT_CURRENT_DRAWING_NX3 CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。 */  
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
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
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
    static void report_custom_colors(UF_PLOT_custom_colors_p_t custom_colors)  
    {  
        int ii;  
        for (ii = 0; ii < UF_CGM_MAX_COLORS; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(custom_colors->colors[ii].clr_index);  
            WRITE_S(custom_colors->colors[ii].clr_name);  
            WRITE3F(custom_colors->colors[ii].clr_vals);  
            if (custom_colors->colors[ii].favorite_index != UF_CGM_NO_FAVORITE)  
            {  
                WRITE_D(custom_colors->colors[ii].favorite_index);  
            }  
        }  
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
        logical  
            drawing_sheet_has_colors,  
            drawing_sheet_has_widths;  
        tag_t  
            dwg;  
        char  
            *job_name,  
            *printer,  
            *profile;  
        UF_PLOT_job_options_t  
            job_options;  
        UF_PLOT_custom_widths_t  
            custom_widths;  
        UF_PLOT_custom_colors_t  
            custom_colors;  
        UF_PLOT_banner_options_t  
            banner_options;  
        UF_CALL(UF_PLOT_ask_session_job_options(&job_options));  
        report_job_options(&job_options);  
        if ((!job_options.use_drawing_sheet_colors) &&  
            (job_options.colors == UF_PLOT_CUSTOM_PALETTE_COLORS))  
        {  
            UF_CALL(UF_PLOT_ask_session_custom_colors(&custom_colors));  
            report_custom_colors(&custom_colors);  
        }  
        if ((!job_options.use_drawing_sheet_widths) &&  
            (job_options.widths != UF_PLOT_STANDARD_WIDTHS))  
        {  
            UF_CALL(UF_PLOT_ask_session_custom_widths(&custom_widths));  
            report_custom_widths(&custom_widths);  
        }  
        UF_CALL(UF_PLOT_ask_session_banner_options(&banner_options));  
        report_banner_options(&banner_options);  
        UF_CALL(UF_PLOT_ask_default_printer_and_profile( &printer, &profile));  
        WRITE_S(printer);  
        WRITE_S(profile);  
        if (!UF_CALL(UF_DRAW_ask_current_drawing(&dwg)) && (dwg != NULL_TAG))  
        {  
            UF_CALL(UF_PLOT_ask_default_job_name(dwg, &job_name));  
            WRITE_S(job_name);  
            if (job_options.use_drawing_sheet_colors)  
            {  
                UF_CALL(UF_PLOT_has_drawing_sheet_colors(dwg,  
                    &drawing_sheet_has_colors));  
                WRITE_L(drawing_sheet_has_colors);  
                if (drawing_sheet_has_colors)  
                {  
                    UF_CALL(UF_PLOT_ask_drawing_sheet_colors(dwg, &custom_colors));  
                    report_custom_colors(&custom_colors);  
                }  
            }  
            if (job_options.use_drawing_sheet_widths)  
            {  
                UF_CALL(UF_PLOT_has_drawing_sheet_widths(dwg,  
                    &drawing_sheet_has_widths));  
                WRITE_L(drawing_sheet_has_widths);  
                if (drawing_sheet_has_widths)  
                {  
                    UF_CALL(UF_PLOT_ask_drawing_sheet_widths(dwg, &custom_widths));  
                    report_custom_widths(&custom_widths);  
                }  
            }  
            UF_CALL(UF_PLOT_print(dwg, &job_options, job_name, &banner_options,  
                printer, profile, 1));  
            UF_free(job_name);  
        }  
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

> 根据代码内容，这是一个用于在NX中获取和设置打印配置的二次开发程序。具体功能如下：
>
> 1. 获取当前的打印作业选项，包括颜色、线宽等设置，并输出到日志窗口。
> 2. 如果使用了自定义调色板颜色，获取自定义颜色设置并输出。
> 3. 如果使用了自定义线宽设置，获取自定义线宽设置并输出。
> 4. 获取横幅打印选项并输出。
> 5. 获取默认打印机名称和打印配置文件名称。
> 6. 如果当前存在打开的图纸，获取默认打印作业名称。
> 7. 如果图纸使用了图纸颜色，获取图纸颜色设置并输出。
> 8. 如果图纸使用了图纸线宽，获取图纸线宽设置并输出。
> 9. 使用当前的打印设置打印图纸。
> 10. 在打印之前，先初始化NX环境，打印完成后终止NX环境。
> 11. 提供卸载函数。
>
> 通过该程序，用户可以查看和设置NX的打印配置，并且进行打印操作。
>
