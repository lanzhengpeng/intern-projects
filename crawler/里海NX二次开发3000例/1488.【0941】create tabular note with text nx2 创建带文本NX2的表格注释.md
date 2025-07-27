### 【0941】create tabular note with text nx2 创建带文本NX2的表格注释

#### 代码

```cpp
    /*HEAD CREATE_TABULAR_NOTE_WITH_TEXT_NX2 CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本新增的功能。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    void extern create_tabular_note(char *tabtext)  
    {  
        UF_TABNOT_section_prefs_t  
            sec_prefs;  
        double  
            loc[]= { 1,1,0 };  
        tag_t  
            row,  
            col,  
            cel,  
            tabnote;  
        int  
            indxr = 0,  
            indxc = 0;  
        UF_CALL(UF_TABNOT_ask_default_section_prefs(&sec_prefs));  
        UF_CALL(UF_TABNOT_create(&sec_prefs, loc, &tabnote));  
        UF_CALL(UF_TABNOT_ask_nth_row(tabnote, indxr, &row));  
        UF_CALL(UF_TABNOT_ask_nth_column(tabnote, indxc, &col));  
        UF_CALL(UF_TABNOT_ask_cell_at_row_col(row, col, &cel));  
        UF_CALL(UF_TABNOT_set_cell_text(cel, tabtext));   
    }  
    static void do_it(void)  
    {  
        char   
            tabtext[133] = "Text for first cell ";  
        prompt_for_text("Enter text for tab note", tabtext);  
        create_tabular_note(tabtext);     
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 创建一个表格注释，并将其放置在指定的位置(1,1,0)。
> 2. 为表格的第一行第一列的单元格设置指定的文本内容。
> 3. 用户可以输入要设置的文本内容。
> 4. 通过错误处理函数report_error()来打印错误信息。
> 5. 通过UF_print_syslog()函数将错误信息打印到系统日志。
> 6. 通过UF_UI模块将错误信息显示在NX的列表窗口中。
> 7. 定义了一个外部函数create_tabular_note()来创建表格注释，并设置单元格文本。
> 8. 在ufusr()函数中初始化NX环境，调用do_it()函数执行主要逻辑，最后清理NX环境。
> 9. 通过ufusr_ask_unload()函数设置在NX关闭时立即卸载该二次开发应用程序。
>
> 总体来说，这段代码通过NX的二次开发接口实现了创建表格注释并设置单元格文本的功能，同时包含了错误处理和日志打印功能。
>
