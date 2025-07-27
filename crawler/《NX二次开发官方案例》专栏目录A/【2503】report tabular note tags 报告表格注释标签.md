### 【2503】report tabular note tags 报告表格注释标签

#### 代码

```cpp
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_tabular_note_tags(tag_t tabnote)  
    {  
        int  
            ii,  
            nm_columns,  
            nm_header_rows,  
            nm_sections,  
            nm_rows;  
        tag_t  
            column,  
            header_row,  
            row,  
            section;  
        WRITE_D(tabnote);  
        UF_CALL(UF_TABNOT_ask_nm_rows(tabnote, &nm_rows));  
        WRITE_D(nm_rows);  
        for (ii = 0; ii < nm_rows; ii++)  
        {  
            UF_CALL(UF_TABNOT_ask_nth_row(tabnote, ii, &row));  
            WRITE_D(row);  
        }  
        UF_CALL(UF_TABNOT_ask_nm_columns(tabnote, &nm_columns));  
        WRITE_D(nm_columns);  
        for (ii = 0; ii < nm_columns; ii++)  
        {  
            UF_CALL(UF_TABNOT_ask_nth_column(tabnote, ii, &column));  
            WRITE_D(column);  
        }  
        UF_CALL(UF_TABNOT_ask_nm_sections(tabnote, &nm_sections));  
        WRITE_D(nm_sections);  
        for (ii = 0; ii < nm_sections; ii++)  
        {  
            UF_CALL(UF_TABNOT_ask_nth_section(tabnote, ii, &section));  
            WRITE_D(section);  
        }  
        UF_CALL(UF_TABNOT_ask_nm_header_rows(tabnote, &nm_header_rows));  
        WRITE_D(nm_header_rows);  
        for (ii = 0; ii < nm_header_rows; ii++)  
        {  
            UF_CALL(UF_TABNOT_ask_nth_header_row(tabnote, ii, &header_row));  
            WRITE_D(header_row);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open API中的二次开发代码，用于报告表格注释的标签信息。主要步骤如下：
>
> 1. 定义宏WRITE_D，用于将变量名和值输出到列表窗口。
> 2. 定义函数write_integer_to_listing_window，用于将变量名和整数值输出到列表窗口。
> 3. 定义函数report_tabular_note_tags，用于报告表格注释的标签信息。
> 4. 在函数内部，首先获取表格注释的行数，然后遍历每一行，获取行标签并输出。
> 5. 接着获取表格注释的列数，遍历每一列，获取列标签并输出。
> 6. 获取表格注释的区段数，遍历每一个区段，获取区段标签并输出。
> 7. 获取表格注释的表头行数，遍历每一行，获取表头行标签并输出。
> 8. 通过调用NX Open API中的函数，获取表格注释的行、列、区段和表头行信息。
> 9. 通过WRITE_D宏，将获取的标签信息输出到列表窗口。
> 10. 通过UF_UI_open_listing_window和UF_UI_write_listing_window函数，打开列表窗口并写入信息。
>
> 这段代码的主要作用是遍历表格注释的行、列、区段和表头行，并将它们的标签信息输出到列表窗口，以供调试和查看。
>
