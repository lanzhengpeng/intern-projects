### 【1976】report all tabular notes parameters and eval data 报告所有表格注释参数和评估数据

#### 代码

```cpp
    /*HEAD REPORT_ALL_TABULAR_NOTES_PARAMETERS_AND_EVAL_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数，用于在系统日志中打印信息。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
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
    static void report_tabular_note_eval_data(  
        UF_DRAW_tabnot_eval_data_p_t eval_data)  
    {  
        int ii;  
        WRITE_D(eval_data->table_start.row);  
        WRITE_D(eval_data->table_start.col);  
        WRITE_D(eval_data->table_end.row);  
        WRITE_D(eval_data->table_end.col);  
        WRITE_D(eval_data->nm_lines);  
        for (ii = 0; ii < eval_data->nm_lines; ii++)  
        {  
            WRITE_D(ii);  
            WRITE3F(eval_data->lines[ii].start);  
            WRITE3F(eval_data->lines[ii].end);  
        }  
    }  
    static tag_t ask_next_tabular_note(tag_t part, tag_t tnote)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type, &tnote))  
            && (tnote != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(tnote, &type, &subtype));  
            if (subtype == UF_tabular_note_subtype) return tnote;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            tnote = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_DRAW_tabnot_params_t  
            parms;  
        UF_DRAW_tabnot_eval_data_t  
            eval_data;  
        while ((tnote = ask_next_tabular_note(part, tnote)) != NULL_TAG)  
        {  
            WRITE_D(tnote);  
            UF_CALL(UF_DRAW_ask_tabular_note_params(tnote, &parms,  
                &eval_data));  
            report_tabular_note_parameters(&parms);  
            report_tabular_note_eval_data(&eval_data);  
        }  
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

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是遍历当前显示部件中的所有表格注释，并打印出它们的参数和评估数据。
>
> 代码首先包含必要的头文件，定义了一些宏来简化错误处理和打印变量。主要功能由以下函数实现：
>
> 1. report_error()：用于打印错误信息到系统日志和列表窗口。
> 2. write_integer_to_listing_window()：用于将整数值打印到列表窗口。
> 3. write_double_to_listing_window()：用于将浮点数打印到列表窗口。
> 4. write_double_array_to_listing_window()：用于将浮点数组打印到列表窗口。
> 5. write_logical_to_listing_window()：用于将布尔值打印到列表窗口。
> 6. report_tabular_note_parameters()：用于打印表格注释的参数。
> 7. report_tabular_note_eval_data()：用于打印表格注释的评估数据。
> 8. ask_next_tabular_note()：用于遍历当前部件中的下一个表格注释。
> 9. do_it()：主函数，用于遍历所有表格注释并打印信息。
> 10. ufusr()：NX Open提供的标准二次开发函数入口点。
> 11. ufusr_ask_unload()：用于卸载二次开发程序。
>
> 通过调用UF_DRAW_ask_tabular_note_params()函数，可以获取表格注释的参数和评估数据，然后利用自定义的打印函数将它们输出。该代码提供了遍历和打印NX部件中表格注释的一个示例。
>
