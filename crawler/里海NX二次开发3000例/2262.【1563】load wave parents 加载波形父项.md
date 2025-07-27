### 【1563】load wave parents 加载波形父项

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_wave.h>  
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
    static void load_all_wave_parents(tag_t part)  
    {  
        int  
            ii,  
            n_parts,  
            *statuses;  
        char  
            **file_names,  
            msg[133];  
        UF_CALL(UF_WAVE_load_parents(part, UF_ASSEM_all, &n_parts, &file_names,  
            &statuses));  
    //  See PR 7125521 - failure information is no longer returned  
        ECHO("no. failures = %d\n", n_parts);  
        if (n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < n_parts; ii++)  
            {  
                UF_get_fail_message(statuses[ii], msg);  
                ECHO("  %s - %s\n", file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_free(statuses);  
            UF_free_string_array(n_parts, file_names);  
        }  
    }  
    static void do_it(void)  
    {  
        load_all_wave_parents(UF_PART_ask_display_part());  
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

> 根据代码，这是一个NX的二次开发UF程序，主要功能是加载NX部件的波前(wavefront)父级。
>
> 1. 包含了NX的UF库头文件，如uf.h、uf_part.h、uf_assem.h、uf_wave.h等。
> 2. 定义了一个ECHO宏，用于在UF系统的日志窗口和系统日志中输出信息。
> 3. 定义了一个报告错误的report_error函数，用于在函数调用失败时输出错误信息。
> 4. 定义了一个load_all_wave_parents函数，用于加载指定部件的所有波前父级。它调用UF_WAVE_load_parents函数来实现，并输出加载成功或失败的部件文件名和状态。
> 5. 定义了一个do_it函数，用于加载当前显示的部件的所有波前父级。
> 6. 定义了ufusr函数，这是程序的入口点。它初始化UF库，调用do_it函数，然后终止UF库。
> 7. 定义了ufusr_ask_unload函数，用于卸载程序，返回立即卸载。
>
> 总的来说，这是一个简单的NX二次开发程序，实现了加载当前显示部件的波前父级的功能。
>
