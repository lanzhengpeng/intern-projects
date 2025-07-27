### 【0459】change file ownership 更改文件所有权

#### 代码

```cpp
    /*HEAD CHANGE_FILE_OWNERSHIP CCC UFUN */  
    /* Windows O/S does not support this functionality 译:Windows操作系统不支持此功能。 */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_csys.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog 是 V18 中的新功能，用于打印系统日志。文档中提到了这个新功能，请直接翻译这个句子，不要添加任何其他内容。 */  
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
        int  
            resp;  
        char  
            new_owner[17] = "webstera",  
            filter[MAX_FSPEC_SIZE+1] = { "*.txt" },  
            fspec[MAX_FSPEC_SIZE+1] = { "" };  
        while (!UF_UI_create_filebox("Choose file for ownership change", "Change Owner",  
            filter, "", fspec, &resp) && (resp != UF_UI_CANCEL)&&  
            prompt_for_text("New Owner", new_owner ))  
        {  
            UF_CALL(uc4613(fspec, 0, new_owner));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是用于NX的二次开发，其主要功能是更改文件的拥有者。代码的主要部分包括：
>
> 1. 错误报告函数：用于在发生UF函数调用错误时，记录错误代码、文件名、行号等信息，并通过UF_print_syslog打印到系统日志。
> 2. 提示输入文本函数：用于提示用户输入新文件拥有者的名字。
> 3. 主要逻辑函数：使用UF_UI_create_filebox让用户选择要更改拥有者的文件，并调用UF函数uc4613来更改文件的拥有者。
> 4. NX启动和结束函数：在NX启动和结束阶段分别调用UF_initialize和UF_terminate。
> 5. NX卸载函数：用于在卸载NX时立即卸载二次开发代码。
>
> 总的来说，这段代码通过二次开发实现了更改NX文件拥有者的功能，用户可以方便地选择文件并输入新的拥有者名字，从而完成文件拥有者的更改。代码结构清晰，使用了错误处理和提示输入等辅助功能，提高了用户体验。
>
