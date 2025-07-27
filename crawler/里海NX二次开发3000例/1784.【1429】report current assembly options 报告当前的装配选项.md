### 【1429】report current assembly options 报告当前的装配选项

#### 代码

```cpp
    /*HEAD REPORT_CURRENT_ASSEMBLY_OPTIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog 是 V18 中新增的，请只提供翻译，不要添加其他内容。

翻译：
注意：UF_print_syslog 是在 V18 版本中新增的。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_assem_options(void)  
    {  
        UF_ASSEM_options_t  
            assem_opts;  
        UF_CALL(UF_ASSEM_ask_assem_options(&assem_opts));  
        WRITE_D(assem_opts.load_options);  
        WRITE_D(assem_opts.parts_list);  
        WRITE_D(assem_opts.update);  
        WRITE_D(assem_opts.emphasize);  
        WRITE_D(assem_opts.emphasize_color);  
        WRITE_D(assem_opts.failure_action);  
        WRITE_D(assem_opts.maintain_work_part);  
        WRITE_D(assem_opts.load_latest);  
        WRITE_D(assem_opts.load_components);  
        WRITE_D(assem_opts.load_fully);  
        WRITE_D(assem_opts.load_substitution);  
        WRITE_D(assem_opts.apply_to_all_levels);  
        WRITE_D(assem_opts.load_wave_data);  
        WRITE_D(assem_opts.load_wave_parents);  
    }  
    static void do_it(void)  
    {  
        report_assem_options();  
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

> 这段代码是NX Open C++ API的示例代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于输出函数调用失败的错误信息，包括错误码、错误消息和调用语句。
> 2. 定义了一个整数输出函数write_integer_to_listing_window，用于将整数变量及其名称输出到列表窗口。
> 3. 定义了一个报告装配选项函数report_assem_options，使用UF_ASSEM_ask_assem_options函数获取当前的装配选项，并使用write_integer_to_listing_window函数输出各个选项的值。
> 4. 定义了一个执行函数do_it，用于调用report_assem_options函数。
> 5. 定义了ufusr函数，这是NX Open C++ API程序的入口函数。在初始化NX Open C++环境后，调用do_it函数，执行功能，最后终止NX Open C++环境。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示程序卸载时不需要进行交互确认。
>
> 总体来说，这段代码实现了获取并输出NX装配模块的当前选项值的功能，展示了NX Open C++ API的基本用法。
>
