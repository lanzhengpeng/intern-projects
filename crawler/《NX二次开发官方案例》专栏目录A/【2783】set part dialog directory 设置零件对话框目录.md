### 【2783】set part dialog directory 设置零件对话框目录

#### 代码

```cpp
    /*HEAD SET_PART_DIALOG_DIRECTORY CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译如下：

翻译：

注意：UF_print_syslog是V18版本新增的功能。

 */  
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
    static void do_it(void)  
    {  
        char  
            n_dir[MAX_FSPEC_SIZE+1],  
            *p_dir;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        strcpy(n_dir, p_dir);  
        while (prompt_for_text("Enter directory for part dialogs", n_dir)  
            && UF_CALL(UF_UI_set_dialog_directory(UF_UI_PART_DIR, n_dir)));  
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

> 这段代码是一个NX的二次开发示例，其主要功能是设置NX部件对话框的默认目录。
>
> 代码的主要逻辑如下：
>
> 1. 定义了一个报告错误函数report_error，用于在UF函数调用失败时输出错误信息。
> 2. 定义了一个提示输入文本的函数prompt_for_text，用于提示用户输入并获取文本。
> 3. 定义了一个执行设置目录的函数do_it：调用UF_UI_ask_dialog_directory获取当前部件对话框目录，并复制到一个字符串n_dir中。循环提示用户输入新的目录，并调用UF_UI_set_dialog_directory设置目录，直到用户输入为空结束循环。
> 4. 调用UF_UI_ask_dialog_directory获取当前部件对话框目录，并复制到一个字符串n_dir中。
> 5. 循环提示用户输入新的目录，并调用UF_UI_set_dialog_directory设置目录，直到用户输入为空结束循环。
> 6. ufusr函数是NX调用的主函数，初始化UF库，调用do_it函数，然后终止UF库。
> 7. ufusr_ask_unload函数返回立即卸载标志，表示二次开发模块可以立即被NX卸载。
>
> 总体来说，这段代码通过二次开发实现了设置NX部件对话框默认目录的功能，采用了UF库提供的对话框目录相关函数，并使用了简单的循环和用户输入来实现目录的设置。
>
