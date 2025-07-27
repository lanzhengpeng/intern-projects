### 【2394】report tomorrows date 明日日期报告

#### 代码

```cpp
    /*HEAD REPORT_TOMORROWS_DATE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以得出以下关键信息：

1. UF_print_syslog是V18版本新增的函数。
2. 该函数用于打印系统日志信息。
3. 使用该函数时，需要传递日志信息的级别、文件名、行号和格式化字符串。
4. 该函数可以灵活控制日志信息的输出级别，并可以根据日志级别决定是否需要输出信息。
5. 该函数定义在unifield/src/printer.cpp文件中。

综上所述，UF_print_syslog是一个用于在V18版本中打印系统日志信息的新增函数，能够根据日志级别来控制日志信息的输出。 */  
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
            tomorrow[2],  
            now[2] = { -1, -1 };  
        int  
            datetime[2] = { 0, 0 };  
        char  
            dstr[21],  
            msg[133],  
            tstr[21];  
        UF_CALL(uc4582(now, 2, dstr, tstr));  
        sprintf(msg, "Now: %s %s", dstr, tstr);  
        uc1601(msg, TRUE);  
        UF_CALL(uc4583("", "", datetime));  
        sprintf(msg, "Numerically:  %d days, %6.3f hours", datetime[0], (double)((datetime[1]) / 600.0));  
        uc1601(msg, TRUE);  
        tomorrow[0] = datetime[0] + 1;  
        tomorrow[1] = datetime[1];  
        UF_CALL(uc4582(tomorrow, 2, dstr, tstr));  
        sprintf(msg, "In 24 hours: %s %s", dstr, tstr);  
        uc1601(msg, TRUE);  
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

> 这段代码是一个NX Open C++程序，用于获取当前日期和时间，并计算24小时后的日期和时间，然后输出这些信息。关键点如下：
>
> 1. 引入了UF框架的头文件，用于调用UF函数。
> 2. 定义了report_error函数，用于报告UF函数调用的错误。
> 3. 定义了do_it函数，用于获取当前日期和时间，计算24小时后的日期和时间，并输出这些信息。
> 4. ufusr函数是程序的入口点，用于初始化和调用do_it函数。
> 5. ufusr_ask_unload函数用于设置程序的卸载方式。
> 6. do_it函数中调用了uc4582函数获取日期和时间，uc4583函数获取数值表示的日期和时间，并使用sprintf函数格式化输出。
> 7. 输出包括当前日期和时间、数值表示的当前日期和时间、以及24小时后的日期和时间。
>
> 总的来说，这段代码通过UF函数获取和计算日期时间，实现了基本的日期时间运算和输出功能。
>
