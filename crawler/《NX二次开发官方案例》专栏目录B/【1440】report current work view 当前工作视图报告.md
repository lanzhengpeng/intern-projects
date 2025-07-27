### 【1440】report current work view 当前工作视图报告

#### 代码

```cpp
    /*HEAD REPORT_CURRENT_WORK_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的内容，可以得出以下翻译：

注意：UF_print_syslog 是在 V18 版本中新增的，请只提供翻译，不要添加其他无关内容。 */  
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
            work_view_tag;  
        char  
            msg[133],  
            work_view_name[MAX_ENTITY_NAME_SIZE+1];  
        /* This may be obseleted in the future, but works in V18   
        uc6448(&work_view_name);  
        so instead, use the next two lines */  
        UF_VIEW_ask_work_view(&work_view_tag);  
        UF_CALL(UF_OBJ_ask_name(work_view_tag, work_view_name));  
        sprintf(msg, "Work View: %s", work_view_name);  
        uc1601(msg,TRUE);  
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

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要实现的功能是获取当前的工作视图名称并输出。以下是代码的主要内容和流程：
>
> 1. 引入相关头文件，包括标准库、UF库、对象库、视图库和用户界面库。
> 2. 定义了UF_CALL宏，用于调用UF函数，并检查返回码。如果函数调用失败，会输出错误信息。
> 3. 定义了report_error函数，用于输出错误信息。
> 4. 定义了do_it函数，用于获取当前的工作视图名称并输出。
> 5. 在ufusr函数中，首先调用UF_initialize初始化UF环境，然后调用do_it函数获取并输出当前工作视图名称，最后调用UF_terminate终止UF环境。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示该二次开发代码可以被立即卸载。
>
> 总体来说，这段代码实现了获取当前工作视图名称的功能，并提供了错误处理机制。作为代码的作者，我希望这段代码能够清晰、高效地完成所需功能，并具有良好的可维护性。
>
