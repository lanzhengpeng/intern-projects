### 【0491】clear plot layout 清除图形布局

#### 代码

```cpp
    /*HEAD CLEAR_PLOT_LAYOUT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的，因此翻译为：

注意：UF_print_syslog 是在 V18 版本中新增的，只回答译文，不要废话。 */  
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
    static void do_it(void)  
    {  
    /*  This may not work - see PR 5625614 译:翻译：这可能不起作用，请参阅PR 5625614。 */  
        UF_CALL(UF_PLOT_clear_plot_layout());  
    }  
    /* 里海 */  
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

> 这段代码是用于NX Open API的二次开发代码，主要功能是清除NX的绘图布局。下面是代码的详细解释：
>
> 1. 代码首先包含了一些标准头文件，如stdio.h、string.h、uf.h、uf_ui.h、uf_plot.h等，这些头文件提供了标准输入输出、字符串处理、NX Open API接口等。
> 2. 定义了一个错误报告函数report_error，用于打印出调用NX Open API函数时的错误信息。如果API函数返回错误码，就调用这个函数来报告错误。
> 3. 定义了一个do_it函数，这个函数调用了UF_PLOT_clear_plot_layout函数来清除绘图布局。但注释提示说这个函数可能不起作用，需要查看PR 5625614来获取更多信息。
> 4. 定义了ufusr函数，这是NX Open API的入口函数。首先调用UF_initialize来初始化API，然后调用do_it函数执行清除绘图布局的操作，最后调用UF_terminate来终止API。
> 5. 定义了ufusr_ask_unload函数，这个函数返回UF_UNLOAD_IMMEDIATELY，表示当卸载这个库时，可以立即卸载，不需要等待。
>
> 总的来说，这段代码的主要作用是提供了一个NX Open API的二次开发示例，用于清除NX的绘图布局。需要注意的是，由于注释提示存在潜在的问题，实际使用时需要进一步验证和调试。
>
