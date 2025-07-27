### 【1816】read and set customer area 读取和设置客户区域

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。该函数的作用是打印系统日志。 */  
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

> 这段代码是NX的二次开发代码，实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用NX API函数失败时，打印错误信息。
> 2. 定义了一个提示输入文本的函数prompt_for_text，用于向用户提示输入文本，并获取用户输入。
> 3. 定义了一个执行主体函数do_it，用于获取当前显示的部件，读取并修改其客户区域文本，然后提示用户输入新文本并更新客户区域文本。
> 4. 定义了ufusr函数，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载二次开发的标志。
>
> 总体来说，这段代码实现了在NX中读取和修改部件的客户区域文本的功能，并通过提示框获取用户输入来更新文本。代码结构清晰，使用了错误处理和内存管理，是NX二次开发的一个典型示例。
>
