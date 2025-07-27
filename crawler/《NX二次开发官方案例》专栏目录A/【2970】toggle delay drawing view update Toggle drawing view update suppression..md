### 【2970】toggle delay drawing view update Toggle drawing view update suppression.

#### 代码

```cpp
    /*HEAD TOGGLE_DELAY_DRAWING_VIEW_UPDATE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意，UF_print_syslog是V18版本中新增的。 */  
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
        logical  
            suppress_view_update;  
        UF_CALL(UF_DRAW_ask_suppress_view_updat(&suppress_view_update));  
        UF_CALL(UF_DRAW_set_suppress_view_updat(1-suppress_view_update));  
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

> 这段代码是一个NX二次开发程序，其主要功能是切换NX的视图更新。具体来说，它通过调用UF_DRAW_ask_suppress_view_updat函数来查询当前是否抑制视图更新，然后通过UF_DRAW_set_suppress_view_updat函数切换视图更新的状态。
>
> 在代码中，首先定义了一个错误报告函数report_error，用于在调用UF函数出错时打印错误信息。
>
> 主函数ufusr首先调用UF_initialize来初始化NX，然后调用do_it函数来切换视图更新状态，最后调用UF_terminate来终止NX。
>
> do_it函数通过UF_DRAW_ask_suppress_view_updat获取当前视图更新状态，然后调用UF_DRAW_set_suppress_view_updat切换状态。
>
> 此外，还定义了一个ufusr_ask_unload函数，返回值为UF_UNLOAD_IMMEDIATELY，表示立即卸载该程序。
>
> 总的来说，这段代码实现了在NX中切换视图更新的功能。
>
