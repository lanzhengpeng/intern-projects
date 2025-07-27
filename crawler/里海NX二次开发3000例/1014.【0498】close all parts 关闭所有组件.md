### 【0498】close all parts 关闭所有组件

#### 代码

```cpp
    /*HEAD CLOSE_ALL_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中新增的功能。

它允许用户将日志输出到系统日志。 */  
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
        UF_CALL(UF_PART_close_all());  
    }  
    /* 里海 */  
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

> 这段代码是NX Open API的一个示例，实现了在NX中关闭所有部件的功能。下面是代码的主要功能介绍：
>
> 1. 引入了必要的头文件，包括stdio.h、string.h、uf.h、uf_ui.h、uf_part.h。
> 2. 定义了一个宏UF_CALL，用于调用NX API函数，并在调用失败时报告错误信息。
> 3. 实现了一个静态函数report_error，用于打印错误信息到系统日志和窗口。
> 4. 实现了一个静态函数do_it，调用了UF_PART_close_all()函数，用于关闭所有打开的部件。
> 5. 定义了ufusr函数，这是NX Open API的入口函数。在该函数中初始化了NX环境，调用do_it函数关闭所有部件，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示在NX关闭时立即卸载该二次开发程序。
> 7. 代码结构清晰，包含错误处理，是一个完整的NX二次开发示例，实现了关闭所有部件的功能。
>
