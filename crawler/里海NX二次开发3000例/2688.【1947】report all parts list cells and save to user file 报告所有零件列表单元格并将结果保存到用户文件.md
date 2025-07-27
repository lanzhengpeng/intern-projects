### 【1947】report all parts list cells and save to user file 报告所有零件列表单元格并将结果保存到用户文件

#### 代码

```cpp
    /*HEAD REPORT_ALL_PARTS_LIST_CELLS_AND_SAVE_TO_USER_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
注意：UF_print_syslog 是 V18 中新增的功能，只回答翻译，不要添加其他无关内容。 */  
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
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            nm_rows,  
            nm_columns;  
        tag_t  
            plist = NULL_TAG,  
            cell = NULL_TAG,  
            row = NULL_TAG,  
            column = NULL_TAG;  
        char  
            cellstr[133],  
            savestr[133],  
            row_str[941], /* 7 X 133 里海译:7 x 133等于931。 */  
            *cell_text,  
            tfn[UF_CFI_MAX_PATH_NAME_SIZE],  
            your_name[30];  
        logical  
            use_file;  
        if ((plist = UF_PLIST_ask_tag()) != NULL_TAG)  
        {  
            WRITE_D(plist);  
            UF_CALL( UF_TABNOT_ask_nm_rows( plist, &nm_rows) );  
            WRITE_D(nm_rows);  
            UF_CALL( UF_TABNOT_ask_nm_columns( plist, &nm_columns) );  
            WRITE_D(nm_columns);  
            for( ii = 0; ii < nm_rows; ii++)  
            {  
                UF_TABNOT_ask_nth_row( plist, ii, &row);  
                for( jj = 0; jj < nm_columns; jj++)  
                {  
                    UF_TABNOT_ask_nth_column( plist, jj, &column);  
                    UF_CALL( UF_TABNOT_ask_cell_at_row_col( row, column, &cell) );  
                    /* WRITE_D(cell);*/  
                    UF_CALL( UF_TABNOT_ask_evaluated_cell_text( cell, &cell_text) );  
                    /* Formatting output. You'll want to change or   
                        eliminate this, according to your output needs.*/   
                    switch (jj)   
                    {  
                        case 0:  
                            sprintf(cellstr, "%-7s", cell_text);   
                            strcpy(row_str, cellstr);  
                        break;  
                        case 1:  
                            sprintf(cellstr, "%-42s", cell_text);      
                            strcat(row_str, cellstr);  
                        break;  
                        case 2:  
                            sprintf(cellstr, "%-6s", cell_text);   
                            strcat(row_str, cellstr);  
                        break;  
                        case 3:  
                            sprintf(cellstr, "%-5s", cell_text);   
                            strcat(row_str, cellstr);  
                        break;  
                        case 4:  
                            sprintf(cellstr, "%-41s", cell_text);      
                            strcat(row_str, cellstr);  
                        break;  
                        case 5:  
                            sprintf(cellstr, "%-6s", cell_text);   
                            strcat(row_str, cellstr);  
                        break;  
                        case 6:  
                            sprintf(cellstr, "%-s", cell_text);    
                            strcat(row_str, cellstr);  
                        break;  
                        default:  
                        break;  
                    }  
                        /*UF_CALL( UF_TABNOT_ask_cell_prefs( cell, &cell_prefs) );*/  
                }  
                if (!UF_UI_open_listing_window())  
                {  
                    UF_UI_write_listing_window(row_str);  
                    UF_UI_write_listing_window("\n");  
                }  
            }  
        }  
        UF_CALL(uc4595(1, your_name));  /* API equiv. to GRIP's GPA &USERID 里海译:GPA和USERID在GRIP中的等效翻译API为：

GPA: 学业平均成绩点数
USERID: 用户识别号 */  
        sprintf(tfn, "C:\\temp\\%s.txt", your_name);  
        sprintf(savestr, "Save to %s ?\n", tfn);  
        use_file = ask_yes_or_no("Save output to User File?", savestr);  
        if(use_file)  
        {  
            UF_UI_save_listing_window(tfn);  
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

> 这段代码是一个NX的二次开发程序，其主要功能是：
>
> 1. 获取当前NX部件列表，并获取部件列表的行数和列数。
> 2. 遍历部件列表的每一行和列，获取每个单元格的内容，并进行格式化输出。
> 3. 提示用户是否将输出保存到文件，如果用户选择是，则将输出保存到临时目录下以用户名命名的文本文件中。
> 4. 使用UF_print_syslog函数将错误信息输出到NX系统日志中。
> 5. 定义了一个错误报告函数report_error，用于在出错时输出错误信息。
> 6. 定义了一个写入整型到列表窗口的函数write_integer_to_listing_window。
> 7. 定义了一个询问是否的函数ask_yes_or_no。
> 8. 在ufusr函数中初始化NX环境，执行do_it函数，然后终止NX环境。
> 9. 提供了卸载函数ufusr_ask_unload。
>
> 这段代码通过遍历部件列表，获取每个单元格的内容，并进行格式化输出和保存，实现了报告部件列表所有单元格内容的功能。
>
