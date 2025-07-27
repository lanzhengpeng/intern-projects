### 【0490】clear listing window 清除列表窗口

#### 代码

```cpp
    /*HEAD CLEAR_LISTING_WINDOW CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，可以得出以下翻译：

注意：UF_print_syslog是在V18版本中新增的函数。

希望这个翻译对您有帮助。 */  
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
        UF_CALL(UF_UI_exit_listing_window());  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_UI_write_listing_window("This is the only thing there."));  
    }  
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

> 这段代码是一个NX Open的UFUNC应用程序示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在UFUNC函数调用出错时，输出错误信息到系统日志和列表窗口。
> 2. 定义了一个函数do_it，用于关闭并重新打开NX的列表窗口，并写入文本"This is the only thing there."。
> 3. 定义了ufusr函数，作为程序的入口。在该函数中，首先调用UF_initialize()进行初始化，然后调用do_it()执行功能，最后调用UF_terminate()进行清理。
> 4. 定义了ufusr_ask_unload函数，用于指定程序卸载的方式为立即卸载。
>
> 总体来说，这段代码演示了如何使用UFUNC接口操作NX的列表窗口，并包含了一个简单的错误处理机制。通过这段代码，可以学习如何在NX Open中使用UFUNC接口进行二次开发。
>
