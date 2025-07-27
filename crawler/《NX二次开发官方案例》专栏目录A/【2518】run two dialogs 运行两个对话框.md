### 【2518】run two dialogs 运行两个对话框

#### 代码

```cpp
    /*HEAD RUN_TWO_DIALOGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的信息，UF_print_syslog是V18版本新增的函数，主要用于打印系统日志。在文档中提到UF_print_syslog函数是V18版本新增的，因此针对这个问题的翻译应该是：“UF_print_syslog是V18版本新增的函数，主要用于打印系统日志。” */  
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
        int  
            resp_a,  
            resp_b;  
        while (  
            !UF_CALL(UF_STYLER_create_dialog("sample_a.dlg",NULL,0,NULL,&resp_a))  
            && (resp_a != UF_UI_CANCEL) &&  
            !UF_CALL(UF_STYLER_create_dialog("sample_b.dlg",NULL,0,NULL,&resp_b))  
            && (resp_b != UF_UI_CANCEL))  
        {}  
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

> 这段NX二次开发代码的功能是运行两个对话框sample_a.dlg和sample_b.dlg，并能够循环执行直到用户取消其中一个对话框。
>
> 主要步骤包括：
>
> 1. 包含所需的头文件，如stdio.h、string.h、uf.h、uf_ui.h和uf_styler.h。
> 2. 定义一个错误报告函数report_error，用于在函数执行出错时打印错误信息。
> 3. 定义一个do_it函数，用于循环创建并运行两个对话框sample_a.dlg和sample_b.dlg，直到用户取消其中一个。
> 4. 定义ufusr函数，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 5. 定义ufusr_ask_unload函数，用于卸载用户自定义函数。
> 6. 使用UF_CALL宏包装NX API调用，以检查返回码并打印错误信息。
> 7. 通过UF_STYLER_create_dialog函数创建并运行对话框，直到用户取消。
> 8. 使用UF_print_syslog和UF_UI_open_listing_window函数打印日志信息到系统日志和控制台窗口。
> 9. 使用UF_terminate函数终止NX环境。
>
> 这段代码通过循环调用两个对话框，实现了NX中多对话框的交互，为用户提供了一种灵活的二次开发交互方式。
>
