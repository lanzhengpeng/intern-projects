### 【1190】execute clone log file 执行克隆日志文件

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_clone.h>  
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
        UF_CLONE_ex_log_opts_t  
            options = { TRUE, TRUE };  
        UF_CALL(UF_CLONE_execute_log_file(UF_CLONE_clone_operation,  
            "clone.txt", &options));  
    /*  This is required to use UF_CLONE_execute_log_file twice, see PR 4255670 里海译:这是必需的，以便两次使用 UF_CLONE_execute_log_file，请参阅PR 4255670。 */  
        UF_CALL(UF_CLONE_terminate());  
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
    int main (void)  
    {  
        if (UF_CALL(UF_initialize())) exit;  
        do_it();  
        UF_terminate();  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 包含了必要的头文件，并定义了ECHO函数，用于在列表窗口和系统日志中输出信息。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并在发生错误时输出错误信息。
> 3. 定义了do_it函数，其中执行了UF_CLONE_execute_log_file函数，用于克隆模型，并设置了日志选项。
> 4. 在ufusr函数中，初始化NX系统，调用do_it函数执行模型克隆，然后终止NX系统。
> 5. 在main函数中，同样初始化NX系统，执行模型克隆，然后终止NX系统。
> 6. ufusr_ask_unload函数用于卸载NX系统。
>
> 综合来看，这段代码通过调用NX的UF_CLONE模块，实现了对模型进行克隆的功能，并使用了日志选项进行调试。代码结构清晰，错误处理完善，注释简洁，是一个典型的NX二次开发程序。
>
