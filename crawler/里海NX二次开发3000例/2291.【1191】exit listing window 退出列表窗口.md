### 【1191】exit listing window 退出列表窗口

#### 代码

```cpp
    /*HEAD EXIT_LISTING_WINDOW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。翻译为：UF_print_syslog是V18版本新增的函数。 */  
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
        logical  
            open;  
        UF_CALL(UF_UI_is_listing_window_open(&open));  
        if (open) UF_CALL(UF_UI_exit_listing_window());  
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

> 这段代码是用于NX二次开发的UF函数，主要功能包括错误报告和退出列表窗口。具体来说：
>
> 1. 定义了错误报告函数report_error，用于在UF函数调用失败时打印错误信息。
> 2. 定义了do_it函数，用于检查列表窗口是否打开，如果打开则退出列表窗口。
> 3. ufusr函数是NX二次开发的主入口函数。首先初始化UF环境，然后调用do_it函数，最后终止UF环境。
> 4. ufusr_ask_unload函数用于指示NX在卸载该UF时是否需要立即卸载。
> 5. 在错误报告函数中，使用了UF_print_syslog函数打印错误信息到系统日志。这是NX 18版本新增的函数。
> 6. 整个代码通过宏UF_CALL来简化UF函数的调用，并在调用失败时自动报告错误。
> 7. 该代码的主要作用是检查并退出NX的列表窗口。
>
