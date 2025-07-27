### 【1544】list search directories 列出搜索目录

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_ui.h>  
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
    static void report_assem_search_directories(void)  
    {  
        char  
            **dir_list;  
        int  
            ii,  
            num_dir = 0;  
        logical  
            *subs;  
        UF_CALL(UF_ASSEM_ask_search_directories(&num_dir, &dir_list, &subs));  
        for (ii = 0 ; ii < num_dir; ii ++)  
        {  
            ECHO("%s", dir_list[ii]);  
            if (subs[ii]) ECHO("...");  
            ECHO("\n");  
        }  
        UF_free_string_array(num_dir, dir_list);  
        UF_free(subs);  
    }  
    static void do_it(void)  
    {  
        report_assem_search_directories();  
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

> 这段代码是一段NX二次开发代码，主要功能是列出NX中装配搜索目录。以下是代码的主要部分：
>
> 1. 定义了一个ECHO宏，用于输出信息到NX的日志窗口和系统日志。
> 2. 定义了一个report_error函数，用于在NX函数调用失败时报告错误信息，包括错误码、文件名、行号、错误消息和调用语句。
> 3. 定义了一个report_assem_search_directories函数，用于获取并打印NX的装配搜索目录。它首先调用UF_ASSEM_ask_search_directories函数获取目录列表，然后遍历目录列表并打印每个目录名，如果是子目录则打印省略号。最后释放内存。
> 4. 定义了一个do_it函数，用于调用report_assem_search_directories函数。
> 5. 定义了ufusr函数，这是NX二次开发程序的入口函数。它首先调用UF_initialize初始化NX，然后调用do_it执行主功能，最后调用UF_terminate终止NX。
> 6. 定义了ufusr_ask_unload函数，用于询问NX是否立即卸载二次开发程序，这里返回的是立即卸载。
>
> 总体来说，这段代码实现了查询并打印NX装配搜索目录的功能，通过标准的NX二次开发流程进行调用。
>
