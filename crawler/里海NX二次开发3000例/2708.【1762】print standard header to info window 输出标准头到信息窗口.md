### 【1762】print standard header to info window 输出标准头到信息窗口

#### 代码

```cpp
    /*HEAD PRINT_STANDARD_HEADER_TO_INFO_WINDOW CCC UFUN */  
    /* There is no Open C API command which will output the standard header to  
        the information window.  The only way to accomplish this from ufunc  
        is to use UF_call_grip to call a simple GRIP program which uses the  
        command PRINT.  The header info is always output the first time any  
        GRIP program uses the PRINT command.  
        Here is the corresponding GRIP code:  
        PRINT/&NULSTR  
        HALT  
    */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是在 V18 版本中新增的函数。 */  
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
        UF_CALL(UF_call_grip("grip/print_header.grx", 0, NULL));  
        UF_CALL(UF_UI_write_listing_window("Scroll up to see header from GRIP\n"));  
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

> 这段代码是一个用于在NX中输出标准信息窗口头的二次开发代码。
>
> 代码的主要功能包括：
>
> 1. 定义了一个report_error函数，用于打印错误信息。
> 2. 定义了一个do_it函数，通过UF_call_grip调用一个简单的GRIP程序，该程序只执行了PRINT命令，从而输出标准信息窗口头。
> 3. ufusr函数是二次开发的入口函数，其中初始化了NX，然后调用do_it函数，最后终止NX。
> 4. ufusr_ask_unload函数用于设置二次开发模块卸载方式为立即卸载。
> 5. 整个代码通过调用GRIP程序来实现输出标准信息窗口头，因为NX的Open C API没有直接提供相关功能。
> 6. report_error函数用于打印错误信息到系统日志和列表窗口，增加了代码的健壮性。
> 7. do_it函数通过UF_UI_write_listing_window函数在列表窗口输出提示信息，提示用户向上滚动查看GRIP输出的头信息。
> 8. 整个代码结构清晰，功能明确，实现了在NX中输出标准信息窗口头的需求。
>
