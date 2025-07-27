### 【3008】turn on wcs display 开启wcs显示

#### 代码

```cpp
    /*HEAD TURN_ON_WCS_DISPLAY CCC UFUN */  
    /* This program demonstrates using UF_call_grip to turn on the WCS display  
       This is the corresponding GRIP code turn_on_wcs_display.grs:  
       &WCSDRW = &YES  
       HALT  
       Be sure the path to turn_on_wcs_display.grx is correct in the UF_call_grip  
       call.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数。 */  
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
        UF_CALL(UF_call_grip("n:\\grip\\turn_on_wcs_display.grx", 0, NULL));  
    #else  
        UF_CALL(UF_call_grip("grip/turn_on_wcs_display.grx", 0, NULL));  
    #endif  
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

> 根据代码注释和上下文，这是一段用于在NX中通过Grip脚本打开WCS显示的二次开发代码。
>
> 主要功能包括：
>
> 1. 定义了一个report_error函数，用于打印错误信息。
> 2. 定义了一个do_it函数，根据不同平台调用不同路径的Grip脚本，用于打开WCS显示。
> 3. 定义了一个ufusr函数，在NX启动时调用do_it函数，实现功能。
> 4. 定义了一个ufusr_ask_unload函数，在NX卸载时返回立即卸载标志。
>
> 代码首先通过UF_initialize初始化NX环境，然后调用do_it函数执行Grip脚本，最后通过UF_terminate结束NX环境。如果出错，会通过report_error函数打印错误信息。
>
> 总体来说，这是一个简单的二次开发示例，用于调用Grip脚本来打开NX的WCS显示。
>
