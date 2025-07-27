### 【0408】bring up internet explorer 启动Internet Explorer

#### 代码

```cpp
    /*HEAD BRING_UP_INTERNET_EXPLORER CCC UFUN */  
    /* obviously, this is only for the Windows platform 译:显然，这仅适用于Windows平台。 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog is new in V18，意思是UF_print_syslog是V18版本新增的。 */  
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
        system("start iexplore support.ugs.com");  
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

> 根据代码注释，这是一段用于在NX中启动IE浏览器的二次开发代码。主要功能包括：
>
> 1. 使用UF函数调用NX提供的API，实现错误报告功能。
> 2. 定义了一个do_it()函数，用于调用系统命令启动IE浏览器并打开指定网址。
> 3. 实现了ufusr()函数，用于初始化NX，调用do_it()启动IE浏览器，然后终止NX。
> 4. 实现了ufusr_ask_unload()函数，用于卸载该二次开发代码。
> 5. 代码使用了UF函数进行NX API的调用，通过UF_get_fail_message获取错误信息，并通过UF_print_syslog打印错误日志。
> 6. 通过UF_UI_open_listing_window和UF_UI_write_listing_window实现错误信息在NX界面的打印。
> 7. 使用了C语言标准库函数，如stdio.h、stdlib.h、string.h。
> 8. 代码仅适用于Windows平台，使用了Windows的system()函数来启动IE浏览器。
>
> 综上所述，这是一段用于在NX环境下调用IE浏览器并打开指定网址的二次开发代码。
>
