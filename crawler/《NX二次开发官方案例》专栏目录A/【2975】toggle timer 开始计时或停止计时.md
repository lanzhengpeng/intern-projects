### 【2975】toggle timer 开始计时或停止计时

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static UF_timer_t the_timer;  
    static logical running = FALSE;  
    static void do_it(void)  
    {  
        UF_timer_values_t  
            values;  
        double  
            elapsed = 0;  
        if (running)  
        {  
            UF_CALL(UF_end_timer(the_timer, &values));  
            running = FALSE;  
            ECHO("Total time = %.2f minutes\n", values.real_time/60);  
        }  
        else  
        {  
            UF_CALL(UF_begin_timer(&the_timer));  
            running = TRUE;  
        }  
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
        return (UF_UNLOAD_SEL_DIALOG);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C API的二次开发示例，用于测量代码段的运行时间。以下是代码的主要功能：
>
> 1. 定义了一个ECHO宏，用于输出信息到NX的日志窗口和系统日志。
> 2. 定义了一个UF_CALL宏，用于调用NX API函数，并捕获错误。如果调用失败，它会输出错误代码、错误信息和调用语句。
> 3. 定义了一个全局变量the_timer，用于存储定时器句柄。
> 4. 定义了一个全局变量running，用于标记定时器是否正在运行。
> 5. 定义了一个do_it函数，用于开始或结束定时。如果定时器已经在运行，它会结束定时并输出运行时间；否则，它会开始定时。
> 6. 定义了ufusr函数，作为NX二次开发的入口函数。在这个函数中，它初始化NX Open，调用do_it函数，然后终止NX Open。
> 7. 定义了ufusr_ask_unload函数，返回一个标志，表示在卸载二次开发时是否显示确认对话框。
>
> 总的来说，这段代码通过NX Open C API实现了对代码段运行时间的测量，并且具有错误处理和日志输出功能。
>
