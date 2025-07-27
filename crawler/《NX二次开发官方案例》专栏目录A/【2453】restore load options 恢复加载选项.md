### 【2453】restore load options 恢复加载选项

#### 代码

```cpp
    /*HEAD RESTORE_LOAD_OPTIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本中新增的功能。该功能的作用是打印系统日志信息。 */  
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
    static void report_assem_options(void)  
    {  
        UF_ASSEM_options_t  
            assem_opts;  
        UF_CALL(UF_ASSEM_ask_assem_options(&assem_opts));  
        ECHO(assem_opts.load_options);  
        ECHO(assem_opts.parts_list);  
        ECHO(assem_opts.update);  
        ECHO(assem_opts.emphasize);  
        ECHO(assem_opts.emphasize_color);  
        ECHO(assem_opts.failure_action);  
        ECHO(assem_opts.maintain_work_part);  
        ECHO(assem_opts.load_latest);  
        ECHO(assem_opts.load_components);  
        ECHO(assem_opts.load_fully);  
        ECHO(assem_opts.load_substitution);  
        ECHO(assem_opts.apply_to_all_levels);  
        ECHO(assem_opts.load_wave_data);  
        ECHO(assem_opts.load_wave_parents);  
    }  
    static void do_it(void)  
    {  
        report_assem_options();  
        UF_CALL(UF_ASSEM_restore_load_options("new_load_options.def"));  
        report_assem_options();  
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

> 这是段NX二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于输出调用UF函数时的错误信息。
> 2. 定义了一个报告装配选项的函数report_assem_options，用于输出当前的装配加载选项。
> 3. 定义了一个执行函数do_it，先输出当前的装配加载选项，然后调用UF_ASSEM_restore_load_options函数恢复新的加载选项，再输出更新后的选项。
> 4. 定义了ufusr函数，用于初始化NX和执行do_it函数，然后终止NX。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载标志。
> 6. 代码通过调用UF_ASSEM_ask_assem_options获取当前的装配加载选项，并输出。然后调用UF_ASSEM_restore_load_options恢复新的加载选项，再输出更新后的选项。通过这个示例展示了如何在NX中获取和设置装配加载选项。
>
