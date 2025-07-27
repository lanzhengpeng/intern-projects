### 【1974】report all tabnote cells 报告所有表格注释单元格

#### 代码

```cpp
    /*HEAD REPORT_ALL_TABNOTE_CELLS CCC UFUN */  
    /* This sample uses some new functions declared in uf_tabnote.h 里海译:这个样本使用了一些在uf_tabnote.h中声明的新函数。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_tabnot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的功能。翻译如下：

注意：UF_print_syslog是V18版本新增的功能。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnS(n,X) (write_string_array_to_listing_window(#X, X, n))  
    void write_string_array_to_listing_window(char *title, char **strings,  
        int n_strings)  
    {  
        int  
            ii;  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            sprintf(msg, "%s[%d] = %s\n", title, ii, strings[ii]);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    static tag_t ask_next_tabular_note(tag_t part, tag_t tnote)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type,  
            &tnote)) && (tnote != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(tnote, &type, &subtype));  
            if (subtype == UF_tabular_note_subtype) return tnote;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            nm_rows,  
            nm_columns;  
        tag_t  
            tnote = NULL_TAG,  
            cell = NULL_TAG,  
            row = NULL_TAG,  
            column = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            *cell_text;  
        UF_TABNOT_cell_prefs_t  
            cell_prefs;  
        while ((tnote = ask_next_tabular_note(part, tnote)) != NULL_TAG)  
        {  
            WRITE_D(tnote);  
            UF_CALL( UF_TABNOT_ask_nm_rows( tnote, &nm_rows) );  
            WRITE_D(nm_rows);  
            UF_CALL( UF_TABNOT_ask_nm_columns( tnote, &nm_columns) );  
            WRITE_D(nm_columns);  
            for( ii = 0; ii < nm_columns; ii++)  
            {  
                for( jj = 0; jj < nm_rows; jj++)  
                {  
                    UF_TABNOT_ask_nth_row( tnote, jj, &row);  
                    UF_TABNOT_ask_nth_column( tnote, ii, &column);  
                    UF_CALL( UF_TABNOT_ask_cell_at_row_col( row, column, &cell) );  
                    WRITE_D(cell);  
                    UF_CALL( UF_TABNOT_ask_cell_text( cell, &cell_text) );  
                    WRITE_S(cell_text);  
                    UF_CALL( UF_TABNOT_ask_cell_prefs( cell, &cell_prefs) );  
                }  
            }  
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

> 这段代码是一个NX的二次开发示例，主要功能是遍历当前部件中的所有表格注释中的所有单元格，并打印出每个单元格的文本内容。主要特点如下：
>
> 1. 包含错误报告宏和日志打印函数，用于在出现错误时打印错误信息。
> 2. 包含整数和字符串的日志打印函数，用于打印变量的值。
> 3. 包含一个获取下一个表格注释的函数，用于循环遍历当前部件中的所有表格注释。
> 4. 主函数do_it()遍历每个表格注释的行和列，获取每个单元格，并打印单元格的文本内容。
> 5. 包含NX启动和退出的函数，以及立即卸载的声明。
> 6. 使用了UF_TABNOT相关的函数，用于操作表格注释中的单元格。
> 7. 整体代码结构清晰，注释详细，具有良好的可读性。
>
> 这段代码的目的是展示如何遍历NX中的表格注释，获取单元格内容，对于学习NX二次开发具有一定的参考价值。
>
