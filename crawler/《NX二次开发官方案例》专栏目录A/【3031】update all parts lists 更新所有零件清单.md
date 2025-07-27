### 【3031】update all parts lists 更新所有零件清单

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plist.h>  
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
        UF_CALL(UF_PLIST_update_all_plists());  
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

> 这段代码是一段NX Open的二次开发代码，实现了以下功能：
>
> 1. 包含了NX Open的头文件，如uf.h、uf_ui.h、uf_plist.h，用于调用NX Open API。
> 2. 定义了一个ECHO宏，用于在列表窗口和系统日志中输出信息。
> 3. 定义了一个报告错误的函数report_error，用于在API调用失败时输出错误信息。
> 4. 定义了一个do_it函数，其中调用了UF_PLIST_update_all_plists()函数，用于更新NX中的所有列表。
> 5. 实现了ufusr函数，这是NX二次开发的标准入口函数。在ufusr中，首先调用UF_initialize()初始化NX Open环境，然后调用do_it()执行更新列表的操作，最后调用UF_terminate()终止NX Open环境。
> 6. 实现了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示可以立即卸载该二次开发程序。
> 7. 使用了#define UF_CALL(X)预处理指令定义了一个宏，用于调用NX Open API函数，并捕获错误。
>
> 总体来说，这段代码实现了在NX中更新所有列表的功能，并包含了错误处理机制。
>
