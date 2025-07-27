### 【1815】read and set customer area 更改客户区域文本

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 中的新功能。 */  
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
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            *read,  
            set[133];  
        UF_CALL(UF_PART_ask_customer_area(part, &read));  
        strncpy(set, read, 132);  
        UF_free(read);  
        if (prompt_for_text("Enter new customer area text", set))  
            UF_CALL(UF_PART_set_customer_area(part, set));  
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

> 这段代码是一个NX的二次开发程序示例，其主要功能是读取和修改当前打开零件的“客户区域”文本。以下是代码的简要介绍：
>
> 1. 包含了必要的NX API头文件，例如 uf.h、uf_ui.h、uf_part.h。
> 2. 定义了 UF_CALL 宏，用于调用NX API函数，并捕获错误代码。
> 3. report_error 函数用于报告错误，包括错误代码、文件名、行号等信息。
> 4. prompt_for_text 函数用于提示用户输入文本，并返回输入结果。
> 5. do_it 函数是主函数，首先获取当前打开零件的tag，然后读取其“客户区域”文本，提示用户输入新文本，并更新零件的“客户区域”。
> 6. ufusr 是NX二次开发程序的入口函数，用于初始化NX API，调用主函数，然后终止NX API。
> 7. ufusr_ask_unload 函数用于卸载二次开发程序。
>
> 该代码通过NX的UF_PART模块，实现了读取和修改零件属性的功能。通过UF_PRINT_SYSLOG函数输出错误信息，并提示用户输入新文本，实现了简单的用户交互。
>
