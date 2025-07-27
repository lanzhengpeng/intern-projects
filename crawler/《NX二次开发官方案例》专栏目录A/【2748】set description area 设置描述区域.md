### 【2748】set description area 设置描述区域

#### 代码

```cpp
    /*HEAD SET_DESCRIPTION_AREA CCC UFUN */  
    #include <stdio.h>   
    #include <string.h>   
    #include <stdlib.h>   
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以总结如下：

1. UF_print_syslog 是 V18 新增的函数。

2. 文档提到 UF_print_syslog，这是 V18 新增的函数，用于打印系统日志。

3. 文档仅要求提供译文，无需添加任何额外内容。

综上所述，UF_print_syslog 是 V18 新增的用于打印系统日志的函数。 */  
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
        UF_CALL(UF_PART_set_description(part, "successfully added text"));   
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

> 这段代码是NX的二次开发代码，其主要功能是设置NX部件的描述信息。以下是代码的主要内容和步骤：
>
> 1. 包含必要的NX开发头文件，如uf.h、uf_ui.h、uf_part.h等。
> 2. 定义宏UF_CALL，用于调用NX API函数，并在调用失败时打印错误信息。
> 3. 定义report_error函数，用于在API调用失败时打印错误信息到系统日志和列表窗口。
> 4. 定义do_it函数，该函数获取当前显示的部件，并设置其描述信息为"successfully added text"。
> 5. 定义ufusr函数，该函数是NX二次开发的入口函数。在ufusr中初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. 定义ufusr_ask_unload函数，返回立即卸载标志，表示二次开发模块不需要在NX中持久化。
>
> 综上所述，这段代码的功能是设置当前显示的NX部件的描述信息为"successfully added text"，并在API调用失败时打印错误信息。这段代码是一个简单的NX二次开发示例。
>
