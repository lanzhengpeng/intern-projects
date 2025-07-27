### 【0429】ask part name 询问零件名称

#### 代码

```cpp
    /*HEAD ASK_PART_NAME CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog是V18版本新增的，只需回复翻译内容，不要添加其他无关内容。 */  
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
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_ask_part_name(part, part_name);  
        uc1601(part_name, TRUE);  
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

> 这段代码是NX Open API的二次开发示例代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印出错误信息，包括错误代码、错误发生位置、错误消息等。
> 2. 定义了一个函数do_it，用于获取当前显示的零件对象，并查询该零件的名称，然后输出到日志窗口。
> 3. 定义了ufusr函数，这是NX Open API的入口函数。在该函数中，先初始化NX Open API，然后调用do_it函数，最后结束NX Open API。
> 4. 定义了ufusr_ask_unload函数，返回卸载模式为立即卸载。
> 5. 使用UF_CALL宏来简化错误处理的代码，该宏会在函数调用失败时调用report_error函数报告错误。
> 6. 使用了NX提供的UF_PART_ask_display_part函数来获取当前显示的零件对象。
> 7. 使用了NX提供的UF_PART_ask_part_name函数来获取零件的名称。
> 8. 使用了NX提供的日志输出函数UF_print_syslog和UF_UI_write_listing_window来打印日志信息。
>
> 综上所述，该代码实现了查询当前显示零件名称并打印的功能，是NX二次开发的一个简单示例。
>
