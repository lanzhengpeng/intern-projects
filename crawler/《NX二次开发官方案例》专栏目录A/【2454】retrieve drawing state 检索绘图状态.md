### 【2454】retrieve drawing state 检索绘图状态

#### 代码

```cpp
    /*HEAD RETRIEVE_DRAWING_STATE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #define SHOW(X)    (report_int_value(#X, (X)))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
    static void report_int_value(char *variable, int value)  
    {  
        char  
            messg[133];  
        sprintf(messg, "%s = %d", variable, value);  
        printf("%s\n", messg);  
        uc1601(messg, TRUE);  
    }  
    static void do_it(void)  
    {  
        SHOW(uc6477());  
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

> 这段代码是用于NX Open C++ API的二次开发代码，其主要功能是获取当前的绘制状态。
>
> 代码的具体功能和结构如下：
>
> 1. 包含必要的头文件，如stdio.h、string.h、uf.h、uf_ui.h和uf_draw.h。
> 2. 定义宏SHOW用于打印变量名和值，以及宏UF_CALL用于调用UF函数并报告错误。
> 3. 定义静态函数report_error，用于报告UF函数调用错误。
> 4. 定义静态函数report_int_value，用于打印变量名和整数值。
> 5. 定义静态函数do_it，其中调用SHOW宏打印当前绘制状态的值。
> 6. 定义ufusr函数，是NX二次开发的入口函数。在该函数中，首先调用UF_initialize进行初始化，然后调用do_it执行主要功能，最后调用UF_terminate进行清理。
> 7. 定义ufusr_ask_unload函数，返回立即卸载标志，用于在NX关闭时卸载二次开发程序。
>
> 总的来说，这段代码的主要功能是调用UF函数获取当前的绘制状态，并打印出来。通过使用NX Open C++ API进行二次开发，实现了获取NX系统当前绘制状态的功能。
>
