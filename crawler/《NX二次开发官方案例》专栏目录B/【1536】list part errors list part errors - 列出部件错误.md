### 【1536】list part errors list part errors - 列出部件错误

#### 代码

```cpp
    /*HEAD LIST_PART_ERRORS CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <error_bases.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是在V18版本中新增的功能。 */  
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
            ii;  
        char  
            err[133];  
        for (ii=ERROR_PART_base; ii<ERROR_OCC_base; ii++)  
            if(!UF_get_fail_message(ii, err)) printf("%d - %s\n", ii, err);  
    }  
    void main( void )  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于错误处理和打印错误信息。其主要特点如下：
>
> 1. 包含了UF错误处理函数的头文件，用于调用UF函数时报告错误。
> 2. 定义了宏UF_CALL，用于调用UF函数，并在发生错误时报告错误信息。
> 3. 定义了report_error函数，用于打印错误信息和调用信息。
> 4. do_it函数遍历所有零件相关的错误码，并打印错误信息。
> 5. main函数初始化和终止NX API，并调用do_it函数。
> 6. 使用UF_print_syslog在系统日志中打印错误信息。
> 7. 使用UF_UI函数在NX图形界面中显示错误信息。
> 8. 该代码主要用于测试和打印NX提供的错误码信息，以便开发者了解每个错误码的含义。
>
> 总体来说，这段代码通过宏和函数封装了UF的错误处理，便于在开发NX二次开发应用程序时使用。
>
