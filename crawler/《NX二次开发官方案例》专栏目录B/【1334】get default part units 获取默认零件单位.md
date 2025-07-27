### 【1334】get default part units 获取默认零件单位

#### 代码

```cpp
    /*HEAD GET_DEFAULT_PART_UNITS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本新增的函数。因此，我的回答是：UF_print_syslog 是 V18 版本新增的函数，不要添加任何废话，只回答译文。 */  
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
        char  
            *default_value;  
        UF_CALL( UF_get_customer_default( "UG_initialUnits", 0, &default_value));  
        printf("Initials Units: %s\n", default_value);  
        UF_free( default_value);  
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

> 这段代码是用于NX二次开发的用户函数示例，其主要功能是获取NX的默认单位设置。具体代码介绍如下：
>
> 1. 头文件引入：引入了必要的头文件，如stdio.h、string.h、uf.h和uf_ui.h，其中uf.h和uf_ui.h是NX二次开发中常用的头文件。
> 2. 宏定义UF_CALL：该宏用于调用NX API函数，并打印错误信息。它通过调用report_error函数来实现错误报告。
> 3. report_error函数：用于报告NX API调用错误。它会获取错误码、错误信息，并打印到系统日志和列表窗口中。
> 4. do_it函数：该函数是主函数，用于调用UF_get_customer_default函数获取默认单位设置，并打印输出。
> 5. ufusr函数：这是NX二次开发的入口函数，用于初始化NX环境，调用do_it函数执行主要逻辑，并在结束时终止NX环境。
> 6. ufusr_ask_unload函数：该函数用于设置NX卸载模式，返回UF_UNLOAD_IMMEDIATELY表示立即卸载二次开发应用程序。
>
> 总体来说，这段代码通过调用NX API获取默认单位设置，并打印输出。它使用了NX二次开发常用的错误处理机制，并提供了完整的示例代码框架。
>
