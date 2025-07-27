### 【1853】register tag event callback 注册标签事件回调

#### 代码

```cpp
    /*HEAD REGISTER_TAG_EVENT_CALLBACK CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的内容，UF_print_syslog是V18版本中的新功能。 */  
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
    /* qq3123197280 */  
    static void my_tag_event_cb(UF_TAG_event_t reason, tag_t tag, void *closure)  
    {  
        ECHO(reason);  
        ECHO(tag);  
    }  
    static void do_it(void)  
    {  
        int  
            cb_id;  
        UF_CALL(UF_TAG_register_event_cb(my_tag_event_cb, NULL, &cb_id));  
        ECHO(cb_id);  
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

> 这段代码是一个NX二次开发的示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在UF函数调用出错时输出错误信息。
> 2. 定义了一个标签事件回调函数my_tag_event_cb，用于处理标签事件。
> 3. 在do_it函数中，使用UF_TAG_register_event_cb注册了标签事件回调函数。
> 4. 在ufusr函数中，初始化UF库，调用do_it函数，然后终止UF库。
> 5. ufusr_ask_unload函数用于卸载该UF插件。
>
> 通过这个示例，我们可以看到如何使用UF库来注册标签事件回调，并在NX中处理标签相关的事件。
>
