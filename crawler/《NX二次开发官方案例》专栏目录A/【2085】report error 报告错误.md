### 【2085】report error 报告错误

#### 代码

```cpp
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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

```

#### 代码解析

> 这段代码是用于NX二次开发中的错误报告功能。
>
> 主要功能包括：
>
> 1. 定义了一个宏UF_CALL，用于调用NX API函数，并在调用前后记录文件名、行号、函数名和返回码。如果返回码不为0，则调用report_error函数报告错误。
> 2. report_error函数用于报告错误。它首先检查返回码是否为0，不为0则调用UF_get_fail_message获取错误信息，然后通过ECHO宏打印错误信息到NX日志窗口和系统日志。
> 3. ECHO宏用于格式化输出信息到NX日志窗口和系统日志。它通过vsnprintf_s函数格式化字符串，并调用UF_UI_open_listing_window和UF_UI_write_listing_window输出到NX日志窗口，同时调用UF_print_syslog输出到系统日志。
> 4. UF_UI_open_listing_window用于打开NX日志窗口。
> 5. UF_UI_write_listing_window用于向NX日志窗口写入信息。
> 6. UF_print_syslog用于向系统日志写入信息。
> 7. UF_get_fail_message用于获取NX API函数的错误信息。
>
> 总体而言，这段代码通过宏和函数封装了错误报告的流程，方便在NX二次开发中快速定位和报告API调用错误。
>
