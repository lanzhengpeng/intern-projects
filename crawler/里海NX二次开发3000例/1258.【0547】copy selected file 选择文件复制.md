### 【0547】copy selected file 选择文件复制

#### 代码

```cpp
    /*HEAD COPY_SELECTED_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的功能，主要用于在用户界面上打印系统日志信息。该功能使开发人员可以轻松地显示系统日志，而无需自己实现日志打印逻辑。 */  
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
            copy_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            filter[UF_CFI_MAX_PATH_NAME_SIZE] = { "" },  
            orig_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        while (!UF_CALL(UF_UI_create_filebox("Select file to Copy",  
            "Copy File Name", filter, "", orig_fspec, &resp)) &&  
            (resp != UF_UI_CANCEL))  
        {  
            strcpy(copy_fspec, orig_fspec);  
            if (prompt_for_text("Copy to", copy_fspec))  
                UF_CALL(uc4567(orig_fspec, copy_fspec, UF_CFI_COPY_ALWAYS_REPLACE,  
                    0, 0));  
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

> 这段代码是NX Open C++的一个示例，实现了以下功能：
>
> 1. 包含了UF.h、uf_ui.h等NX Open C++头文件，定义了与NX系统交互的接口函数。
> 2. 定义了一个错误报告函数report_error，用于在出错时打印错误信息和调用信息。
> 3. 定义了一个提示输入文本的函数prompt_for_text，用于在用户界面上提示用户输入文本。
> 4. 定义了一个主函数do_it，用于实现文件复制的功能。它使用文件选择框让用户选择要复制的文件，然后提示用户输入目标文件名，最后调用文件复制函数实现复制。
> 5. 定义了ufusr函数，这是NX二次开发的入口函数。在ufusr中，先调用UF_initialize初始化NX系统，然后调用do_it执行文件复制功能，最后调用UF_terminate终止NX系统。
> 6. 定义了ufusr_ask_unload函数，用于在卸载NX Open C++库时立即卸载。
>
> 总的来说，这段代码利用NX Open C++的接口，实现了一个简单的文件复制功能。
>
