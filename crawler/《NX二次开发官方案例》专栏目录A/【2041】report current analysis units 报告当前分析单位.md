### 【2041】report current analysis units 报告当前分析单位

#### 代码

```cpp
    #include <stdio.h>  
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
    static void do_it(void)  
    {  
        int  
            units;  
        UF_CALL(UF_UI_ask_info_units(&units));  
        switch (units)  
        {  
            case UF_UI_POUNDS_INCHES:  
                ECHO("Analysis-> Units = lb - in\n");  
                break;  
            case UF_UI_POUNDS_FEET:  
                ECHO("Analysis-> Units = lb - ft\n");  
                break;  
            case UF_UI_GRAMS_MILLIMETERS:  
                ECHO("Analysis-> Units = g - mm\n");  
                break;  
            case UF_UI_GRAMS_CENTIMETERS:  
                ECHO("Analysis-> Units = g - cm\n");  
                break;  
            case UF_UI_KILOS_METERS:  
                ECHO("Analysis-> Units = kg - m\n");  
                break;  
    //  UF_UI_KILOS_MILLIMETERS is missing prior to NX7.5 - see PR 6291450  
    #ifdef UF_UI_KILOS_MILLIMETERS  
            case UF_UI_KILOS_MILLIMETERS:  
                ECHO("Analysis-> Units = kg - mm\n");  
                break;  
    #endif  
            default:  
                ECHO("Analysis-> Units = %d (unknown)\n", units);  
                break;  
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
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是用于NX（前身为Unigraphics）的二次开发代码。它主要实现了以下功能：
>
> 1. 定义了一个ECHO宏，用于输出信息到NX的日志窗口。
> 2. 定义了一个report_error函数，用于在函数调用失败时输出错误信息。
> 3. 定义了一个do_it函数，用于获取NX的当前单位设置，并输出到日志窗口。
> 4. 实现了ufusr函数，这是NX二次开发的入口函数。在ufusr中，首先调用UF_initialize初始化NX API，然后调用do_it执行主体功能，最后调用UF_terminate终止NX API。
> 5. 实现了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示该二次开发应用可以被立即卸载。
>
> 代码的整体逻辑是：通过NX API获取当前单位设置，并输出到日志窗口。其中，do_it函数通过UF_UI_ask_info_units函数获取单位，然后根据不同单位输出不同信息。在NX的不同版本中，可用的单位常量也不同，因此使用了条件编译来处理NX版本差异。
>
> 通过这段代码，用户可以在NX中快速查看当前的单位设置，有助于更好地理解NX模型和仿真。
>
