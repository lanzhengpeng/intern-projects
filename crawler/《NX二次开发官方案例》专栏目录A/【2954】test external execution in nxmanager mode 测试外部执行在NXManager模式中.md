### 【2954】test external execution in nxmanager mode 测试外部执行在NXManager模式中

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
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
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    static void do_it(void)  
    {  
        logical  
            nxmanager_mode;  
        UF_CALL(UF_is_ugmanager_active(&nxmanager_mode));  
        WRITE_L(nxmanager_mode);  
    }  
    int main( int argc, char *argv[] )  
    {  
        if (UF_CALL(UF_UGMGR_initialize(argc, argv))) return 1;  
        do_it();  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 包含了NX二次开发常用的头文件，如uf.h、uf_ui.h、uf_ugmgr.h等。
> 2. 定义了一个ECHO宏，用于格式化输出信息到列表窗口和系统日志。
> 3. 定义了一个UF_CALL宏，用于调用NX的UF函数，并报告错误信息。
> 4. 定义了一个WRITE_L宏，用于输出一个逻辑变量到列表窗口。
> 5. 定义了一个do_it函数，用于获取NX Manager模式是否激活的状态，并输出到列表窗口。
> 6. 在main函数中，初始化NX二次开发环境，调用do_it函数，然后结束二次开发环境。
> 7. 使用了NX提供的UF函数，如UF_is_ugmanager_active判断NX Manager模式是否激活，UF_UGMGR_initialize初始化二次开发环境，UF_terminate结束二次开发环境等。
> 8. 通过宏和函数封装，简化了错误处理和输出，提高了代码的可读性和可维护性。
> 9. 实现了一个简单的功能，用于检测和输出NX Manager模式的状态，为NX二次开发提供了示例。
>
> 这段代码提供了一个NX二次开发的简单示例，通过封装UF函数调用，实现了获取NX Manager模式状态的功能，并输出了相关信息。
>
