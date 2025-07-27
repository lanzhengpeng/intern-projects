### 【2054】report default model reference set name 报告默认模型参考集名称

#### 代码

```cpp
    /*HEAD REPORT_DEFAULT_MODEL_REFERENCE_SET_NAME CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        char  
            *model_refset;  
        if (!UF_CALL(UF_get_customer_default("Assemblies_ModelReferenceSet", 0,  
                &model_refset)) && (model_refset))  
        {  
            WRITE_S(model_refset);  
            UF_free(model_refset);  
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

> 这段代码是一个NX二次开发示例，主要功能是获取NX系统中默认的模型引用集名称，并将其打印到日志窗口中。具体来说：
>
> 1. 函数和宏定义：代码定义了UF_CALL宏，用于调用NX API函数，并在出错时报告错误。同时，还定义了WRITE_S宏，用于将字符串写入日志窗口。此外，还定义了report_error函数用于报告错误，write_string_to_listing_window函数用于向日志窗口写入字符串。
> 2. 主要功能：do_it函数是主要功能实现，它调用NX API函数UF_get_customer_default获取默认的模型引用集名称，然后调用WRITE_S宏将其打印到日志窗口。如果获取成功，还需要调用UF_free释放内存。
> 3. 入口函数：ufusr是NX二次开发的入口函数，首先调用UF_initialize初始化NX API，然后调用do_it执行功能，最后调用UF_terminate终止NX API。此外，ufusr_ask_unload函数用于处理NX退出时的清理工作。
>
> 总的来说，这段代码展示了如何使用NX二次开发接口获取系统配置信息，并进行错误处理和日志输出。代码结构清晰，注释详细，可以作为NX二次开发的一个参考示例。
>
