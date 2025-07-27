### 【1747】plot all drawings nx3 打印所有图纸，NX3版本

#### 代码

```cpp
    /*HEAD PLOT_ALL_DRAWINGS_NX3 CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plot.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本中新增的功能，请仅提供译文，不要添加其他无关内容。 */  
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
        int  
            ii,  
            n_dwgs;  
        tag_t  
            *dwgs;  
        char  
            drawing_name[MAX_ENTITY_NAME_SIZE+1],  
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
        UF_CALL(UF_DRAW_ask_drawings(&n_dwgs, &dwgs));  
        for (ii = 0; ii < n_dwgs; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(dwgs[ii], drawing_name));  
            WRITE_S(drawing_name);  
            UF_CALL(UF_PLOT_ask_default_job_name(dwgs[ii], &job_name));  
            WRITE_S(job_name);  
            UF_CALL(UF_PLOT_print(dwgs[ii], &job_options, job_name,  
                &banner_options, printer, profile, 1));  
            UF_free(job_name);  
        }  
        if (n_dwgs > 0) UF_free(dwgs);  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，主要功能包括：
>
> 1. 获取和打印NX系统中的打印作业参数设置，包括颜色、线宽、自定义颜色和线宽等。
> 2. 获取默认打印机和打印配置文件。
> 3. 获取NX系统中的所有绘图，并为每个绘图生成默认打印作业名。
> 4. 对每个绘图调用UF_PLOT_print函数进行打印。
> 5. 在主函数中，支持直接运行外部程序，并加载指定的NX部件文件，然后打印每个部件的绘图。
> 6. 使用UF_PRINT_ask_session_job_options等函数获取打印作业参数，使用UF_PRINT_ask_session_custom_colors等函数获取自定义颜色和线宽，以及使用UF_PRINT_ask_default_job_name生成默认打印作业名。
> 7. 通过UF_PRINT_print函数进行打印。
> 8. 在主函数中，支持直接运行外部程序，并加载指定的NX部件文件，然后打印每个部件的绘图。
> 9. 使用UF_PRINT_ask_session_job_options等函数获取打印作业参数，使用UF_PRINT_ask_session_custom_colors等函数获取自定义颜色和线宽，以及使用UF_PRINT_ask_default_job_name生成默认打印作业名。
> 10. 通过UF_PRINT_print函数进行打印。
>
> 这段代码提供了一个完整的NX绘图打印的二次开发示例，覆盖了获取打印参数、生成打印作业名、打印绘图等流程，具有很高的参考价值。
>
