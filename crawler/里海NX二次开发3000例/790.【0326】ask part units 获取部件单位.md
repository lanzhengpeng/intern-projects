### 【0326】ask part units 获取部件单位

#### 代码

```cpp
    /*HEAD ASK_PART_UNITS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是在 V18 版本中新增的功能。 */  
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
            units;  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_PART_ask_units(part, &units));  
        ECHO(units);  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 包含了必要的头文件，例如uf.h、uf_ui.h等，以便使用NX提供的函数库。
> 2. 定义了宏ECHO和UF_CALL，用于打印变量值和调用NX函数，并在调用失败时打印错误信息。
> 3. 实现了report_error函数，用于在NX函数调用失败时报告错误，包括文件名、行号、错误码和错误信息。
> 4. 定义了do_it函数，用于获取当前显示部件的单元数，并打印出来。
> 5. 实现了ufusr函数，这是NX二次开发的入口函数。在这里调用do_it函数以执行获取单元数的操作。
> 6. 实现了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示立即卸载NX二次开发模块。
> 7. 该代码主要用于演示如何调用NX的UF_PART_ask_units函数来获取当前显示部件的单元数，并打印出来。同时，也展示了如何处理NX函数调用可能出现的错误。
> 8. 代码结构清晰，使用了宏来简化错误处理，是一个典型的NX二次开发示例。
>
