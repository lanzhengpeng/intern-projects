### 【2214】report name display status 报告名称显示状态

#### 代码

```cpp
    /*HEAD REPORT_NAME_DISPLAY_STATUS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能，用于打印系统日志。 */  
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
        int  
            current_status;  
        UF_CALL(UF_DISP_ask_name_display_status(&current_status));  
        ECHO(current_status);  
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

> 这段代码是一个NX二次开发示例，实现了查询和输出当前名称显示状态的UF功能。
>
> 主要功能包括：
>
> 1. 定义了一个报告错误的函数report_error，用于在UF函数调用出错时输出错误信息。
> 2. 定义了主函数do_it，其中调用了UF_DISP_ask_name_display_status函数，以查询当前名称显示状态，并使用宏ECHO输出该状态。
> 3. 定义了ufusr函数，在NX启动时自动调用。在ufusr中，先调用UF_initialize初始化UF环境，然后调用do_it执行主功能，最后调用UF_terminate终止UF环境。
> 4. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示在NX关闭时立即卸载该UF。
>
> 该示例展示了如何利用UF函数获取NX系统的状态信息，并使用宏进行错误处理和日志输出。通过在NX启动时调用ufusr，可以实现自定义的二次开发功能。
>
