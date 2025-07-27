### 【2958】test internal execution 测试内部执行

#### 代码

```cpp
    /*HEAD TEST_INTERNAL_EXECUTION CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：请注意，UF_print_syslog 是 V18 中的新功能，请只提供译文，不要添加任何废话。 */  
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
        uc1601("It works!", TRUE);  
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

> 这段代码是一个NX Open C++的二次开发示例，实现了在NX中显示文本的功能。下面是对代码的简要介绍：
>
> 1. 头文件包含：引入了标准库头文件、NX Open C++ API头文件、NX UI API头文件等。
> 2. 宏定义UF_CALL：用于封装NX API调用，并捕获错误码，实现错误报告。
> 3. report_error函数：用于打印错误信息到syslog和listing window。
> 4. do_it函数：调用NX API函数uc1601显示文本"It works!"。
> 5. ufusr函数：NX二次开发程序的入口函数。初始化NX环境后，调用do_it函数，然后终止NX环境。
> 6. ufusr_ask_unload函数：用于设置二次开发程序的卸载方式，这里设置为立即卸载。
>
> 这段代码通过NX Open C++ API实现了在NX中显示文本的功能，并通过错误处理和日志打印提高了程序的健壮性。
>
