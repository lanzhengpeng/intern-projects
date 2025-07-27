### 【2019】report cgm default custom widths 报告CGM默认自定义宽度

#### 代码

```cpp
    /*HEAD REPORT_CGM_DEFAULT_CUSTOM_WIDTHS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cgm.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我翻译如下：

注意：UF_print_syslog 是 V18 版本新增的，请仅回答翻译，不要添加其他无关内容。 */  
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
            ii;  
        char  
            msg[133];  
        UF_CGM_custom_widths_t   
            cw;  
        cw.use=UF_CGM_width_by_width;  
        UF_CALL(UF_CGM_ask_default_custom_widths(&cw));  
        for (ii = 0; ii < 3; ii++)  
        {  
            sprintf(msg, "\"%s\"\tWidth: %f \n",  
                cw.custom[ii].name, cw.custom[ii].width);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
            }  
        }  
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

> 根据代码内容，这是一段用于NX Open的二次开发代码，其主要功能如下：
>
> 1. 包含必要的头文件，定义了UF_CALL宏，用于调用NX API并处理错误。
> 2. 定义了report_error函数，用于打印错误信息，包括文件名、行号、错误码和错误消息。
> 3. 定义了do_it函数，用于获取NX默认的自定义线宽设置，并打印每个线宽的名称和值。
> 4. 定义了ufusr函数，这是NX二次开发的入口函数，用于初始化NX Open，调用do_it函数，然后终止NX Open。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发应用可以立即卸载。
> 6. 通过UF_CALL宏，在调用NX API时能够自动处理错误，并通过report_error函数打印错误信息。
> 7. do_it函数通过UF_CGM_ask_default_custom_widths API获取NX默认的自定义线宽设置，并打印出来。
> 8. ufusr函数作为入口函数，用于调用业务逻辑函数do_it，实现获取默认线宽设置的功能。
> 9. 整个代码结构清晰，实现了简单的功能，并具有良好的错误处理机制。
>
