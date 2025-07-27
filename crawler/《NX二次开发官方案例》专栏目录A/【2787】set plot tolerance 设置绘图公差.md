### 【2787】set plot tolerance 设置绘图公差

#### 代码

```cpp
    /*HEAD SET_PLOT_TOLERANCE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_plot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 版本新增的功能，用于打印系统日志。 */  
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
        double  
            toler = 0.0001;  
        UF_CALL(UF_PLOT_set_tolerance(toler));  
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

> 这段代码是NX的二次开发代码，主要功能是设置NX的绘图公差。
>
> 主要特点如下：
>
> 1. 头文件包含：代码中包含了必要的NX头文件，如uf.h、uf_plot.h、uf_ui.h等。
> 2. 错误处理宏：定义了UF_CALL宏，用于在函数调用前后加上错误处理。
> 3. 错误处理函数：report_error函数用于在出错时打印错误信息，包含文件名、行号、调用函数名和错误码。
> 4. 设置绘图公差函数：do_it函数用于调用UF_PLOT_set_tolerance函数设置绘图公差。
> 5. 主函数：ufusr函数是NX二次开发的入口函数，其中调用do_it函数设置绘图公差。
> 6. 卸载函数：ufusr_ask_unload函数返回立即卸载标志，表示二次开发模块可以立即被NX卸载。
>
> 总体来说，这段代码实现了在NX中设置绘图公差的功能，并包含了基本的错误处理和二次开发模块管理。
>
