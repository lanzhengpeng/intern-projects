### 【2976】toggle view name display 切换视图名称显示

#### 代码

```cpp
    /*HEAD TOGGLE_VIEW_NAME_DISPLAY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的功能，主要用于打印系统日志信息。该函数的参数包括日志信息字符串和日志级别，其中日志级别为可选参数，默认值为0。

在V18版本之前，日志信息只能通过UF_print_log函数打印到标准错误输出，而UF_print_syslog则允许将日志信息发送到系统日志服务，从而在系统日志中记录信息。这一功能改进使得日志管理更加灵活，能够更好地满足用户需求。 */  
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
        UF_DISP_system_params_t  
            system_parameters;  
        UF_CALL(UF_DISP_ask_system_parameters(&system_parameters));  
        system_parameters.view_display = 3 - system_parameters.view_display;  
        UF_CALL(UF_DISP_set_system_parameters(&system_parameters));  
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

> 根据代码，这是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了UF_CALL宏，用于调用UF函数，并在函数执行失败时报告错误信息。
> 2. 定义了report_error函数，用于在UF函数执行失败时，打印错误信息到系统日志和错误列表窗口。
> 3. 定义了do_it函数，用于获取当前NX系统参数中的视图显示模式，将其切换为另一种模式(0变3, 1变2, 2变1, 3变0)，并设置回系统参数。
> 4. 定义了ufusr函数，这是NX二次开发的入口函数。在该函数中，初始化NX环境，调用do_it函数以切换视图显示模式，然后结束NX环境。
> 5. 定义了ufusr_ask_unload函数，用于设置二次开发模块卸载模式为立即卸载。
> 6. 整个代码的主要功能是切换NX中的视图显示模式，以实现不同视图显示效果之间的切换。该功能对于调试和演示NX的不同视图显示功能非常有用。
>
