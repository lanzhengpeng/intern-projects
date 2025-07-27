### 【1362】hide pulldown button 隐藏下拉按钮

#### 代码

```cpp
    /*HEAD HIDE_PULLDOWN_BUTTON CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_mb.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我理解UF_print_syslog是在V18版本中新增的功能。 */  
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
        int button_id;  
        UF_CALL( UF_MB_ask_button_id( "UG_VIEW_FIT", &button_id));  
        UF_CALL( UF_MB_set_button_sensitivity( button_id, UF_MB_OFF));  
        /* After this is run, the View -> Fit button will   *  
         * be greyed inside Unigraphics                     */  
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

> 这段代码是NX Open API的二次开发代码，主要功能是禁用NX中的"View -> Fit"按钮。具体代码逻辑如下：
>
> 1. 包含必要的头文件。
> 2. 定义了一个错误报告函数report_error，用于打印函数调用失败时的错误代码和消息。
> 3. 定义了主函数do_it，主要逻辑如下：3.1 使用UF_MB_ask_button_id获取"View -> Fit"按钮的ID。3.2 使用UF_MB_set_button_sensitivity将按钮的可用性设置为UF_MB_OFF，即不可用状态。
> 4. 定义了ufusr函数，这是NX二次开发的入口函数，在这里调用do_it函数。
> 5. 定义了ufusr_ask_unload函数，用于在模块卸载时立即卸载，而不是等待当前操作完成。
>
> 总的来说，这段代码通过调用NX Open API，实现了禁用"View -> Fit"按钮的功能。
>
