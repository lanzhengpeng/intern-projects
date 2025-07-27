### 【2960】test platform for windows 测试平台适用于Windows

#### 代码

```cpp
    /*HEAD TEST_PLATFORM_FOR_WINDOWS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能，它用于打印系统日志。 */  
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
    #ifdef _WIN32  
        uc1601("You ARE in Windows", TRUE);  
    #else  
        uc1601("You ARE NOT in Windows", TRUE);  
    #endif  
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

> 这是一段用于NX Open C++的二次开发代码，主要功能是检查当前运行环境是否为Windows系统。
>
> 代码的主要部分包括：
>
> 1. 头文件包含：包含了必要的头文件，如stdio.h、stdlib.h、string.h等，以及NX Open C++的UF.h和UF_UI.h头文件。
> 2. 错误报告函数：定义了一个report_error函数，用于在发生错误时打印错误信息到系统日志和列表窗口。
> 3. 实际执行函数：定义了一个do_it函数，使用宏定义来判断当前环境是否为Windows，并打印相应的提示信息。
> 4. ufusr函数：这是NX二次开发的入口函数，用于初始化NX环境，调用do_it函数执行实际操作，并在结束时关闭NX环境。
> 5. ufusr_ask_unload函数：用于设置二次开发模块卸载时的策略。
>
> 这段代码的主要功能是检查当前环境是否为Windows系统，并通过打印提示信息来通知用户。通过NX Open C++提供的API，实现了NX二次开发的常见功能。
>
