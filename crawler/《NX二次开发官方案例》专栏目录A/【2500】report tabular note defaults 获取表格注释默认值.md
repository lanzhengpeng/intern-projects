### 【2500】report tabular note defaults 获取表格注释默认值

#### 代码

```cpp
    /*HEAD REPORT_TABULAR_NOTE_DEFAULTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能，用于将日志信息打印到系统日志中。 */  
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
    #define WRITE(X) (UF_UI_write_listing_window(X))  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
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
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
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
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_tabular_note_parameters(UF_DRAW_tabnot_params_p_t params)  
    {  
        WRITE3F(params->position);  
        WRITE_D(params->range_start.row);  
        WRITE_D(params->range_start.col);  
        WRITE_D(params->range_end.row);  
        WRITE_D(params->range_end.col);  
        WRITE_D(params->title_cell.row);  
        WRITE_D(params->title_cell.col);  
        WRITE_F(params->ug_aspect_ratio);  
        WRITE_D(params->border_type);  
        WRITE_F(params->border_width);  
        WRITE_L(params->use_title_cell);  
        WRITE_L(params->use_grid_lines);  
        WRITE_L(params->use_vert_grid_lines);  
        WRITE_L(params->use_horiz_grid_lines);  
        WRITE_L(params->use_row_hdr_grid_lines);  
        WRITE_L(params->use_col_hdr_grid_lines);  
        WRITE_L(params->auto_size_cells);  
    }  
    static void do_it(void)  
    {  
        UF_DRAW_tabnot_params_t  
            default_params;  
        UF_UI_open_listing_window();  
        WRITE("Default tabular note parameters:\n");  
        UF_CALL(UF_DRAW_ask_tabular_note_defaults(&default_params));  
        report_tabular_note_parameters(&default_params);  
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

> 这段代码是NX的二次开发代码，其主要功能是获取表格注释的默认参数并打印输出。
>
> 主要步骤包括：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时报告错误。
> 2. 定义了多个宏，用于将变量打印到日志窗口。
> 3. 定义了write_integer_to_listing_window等函数，用于打印整数、浮点数、逻辑值等。
> 4. 定义了report_tabular_note_parameters函数，用于打印表格注释的参数。
> 5. 在do_it函数中，调用UF_DRAW_ask_tabular_note_defaults获取表格注释的默认参数，并调用report_tabular_note_parameters打印输出。
> 6. 定义了ufusr函数，作为NX的二次开发入口函数，在其中调用do_it函数。
> 7. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 整体来看，这段代码实现了获取并打印输出NX表格注释的默认参数的功能，通过NX二次开发的API调用，实现了参数获取，并通过自定义的打印函数实现了输出。
>
