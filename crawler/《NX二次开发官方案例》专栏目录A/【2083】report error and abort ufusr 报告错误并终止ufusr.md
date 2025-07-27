### 【2083】report error and abort ufusr 报告错误并终止ufusr

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <setjmp.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    jmp_buf env;  
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
            ECHO("Aborting application now\n");  
            longjmp(env, irc);  
        }  
        return(irc);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    static void do_it(void)  
    {  
        int  
            part_units;  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_PART_ask_units(part, &part_units));  
        WRITE_D(part_units);  /* if no part is open - this line will not execute 里海译:如果没有任何部分开启 - 这条线将不会执行 */  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        ECHO("Called UF_initialize...\n");  
        if (!setjmp(env)) do_it();  
        ECHO("Calling UF_terminate...\n");  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发的用户函数示例，主要功能包括：
>
> 1. 包含NX二次开发相关的头文件，定义了常用的二次开发接口。
> 2. 定义了一个宏 UF_CALL，用于调用NX的函数，并检查返回码，如果出错则打印错误信息并退出。
> 3. 定义了一个宏 WRITE_D，用于打印变量的值。
> 4. 定义了一个函数 do_it，其中调用 NX 的 UF_PART_ask_display_part 函数获取当前显示的部件，然后调用 UF_PART_ask_units 获取该部件的单位。
> 5. 定义了 ufusr 主函数，初始化 NX 环境，调用 do_it 函数，然后结束 NX 环境。
> 6. 定义了 ufusr_ask_unload 函数，返回立即卸载标志，用于卸载用户函数。
> 7. 使用 setjmp/longjmp 进行错误处理，出错时可以跳转到指定位置执行。
>
> 总体来说，这段代码实现了在 NX 中获取当前显示部件的单位信息，并进行了错误处理。
>
