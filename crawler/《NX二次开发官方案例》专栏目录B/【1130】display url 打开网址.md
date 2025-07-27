### 【1130】display url 打开网址

#### 代码

```cpp
    /*HEAD DISPLAY_URL CCC UFUN */  
    #include <stdio.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 新增的函数。 */  
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
    static void do_it(void)  
    {  
        UF_UI_display_url("https://blog.csdn.net/WangPaiFeiXingYuan");  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 引入了必要的头文件，包括stdio.h、string.h、uf.h和uf_ui.h。
> 2. 定义了一个宏UF_CALL，用于在函数调用后检查返回码，如果返回码非零则调用report_error函数输出错误信息。
> 3. report_error函数用于输出函数执行失败的错误信息，包括文件名、行号、调用函数名和错误码，同时打印到syslog和listing窗口。
> 4. 定义了一个静态函数do_it，该函数调用UF_UI_display_url显示一个URL。
> 5. ufusr函数是NX二次开发的标准入口函数，首先调用UF_initialize进行初始化，然后调用do_it函数，最后调用UF_terminate进行清理。
> 6. ufusr_ask_unload函数返回立即卸载标志，表示二次开发应用程序可以立即卸载。
>
> 综上所述，这段代码主要演示了NX二次开发中的错误处理、函数调用、初始化和清理等内容。
>
