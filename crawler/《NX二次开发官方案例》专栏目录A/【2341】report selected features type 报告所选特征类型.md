### 【2341】report selected features type 报告所选特征类型

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_FEATURES_TYPE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的信息，可以总结如下：

1. UF_print_syslog 是 V18 版本新增的函数。

2. 对于使用 V18 版本的用户，可以使用 UF_print_syslog 函数来打印系统日志信息。

3. 对于使用 V18 之前版本的用户，该函数不可用，需要使用其他方式来打印系统日志信息。

4. UF_print_syslog 函数的具体参数和用法需要参考相关文档。

5. 该函数的引入为用户提供了更方便的方式来打印系统日志信息。

以上就是对 UF_print_syslog 函数的简要总结，希望对您有所帮助。 */  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_feature_type(tag_t feature)  
    {  
        char  
            *type;  
        WRITE_D(feature);  
        UF_CALL(UF_MODL_ask_feat_type(feature, &type));  
        WRITE_S(type);  
        UF_free(type);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Report type")) != NULL_TAG)  
            report_feature_type(feat);  
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

> 这是NX二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在发生错误时打印错误信息到系统日志和列表窗口。
> 2. 定义了一个选择特征函数select_a_feature，用于让用户选择一个特征，并返回特征tag。
> 3. 定义了两个用于在列表窗口打印整型和字符串的辅助函数write_integer_to_listing_window和write_string_to_listing_window。
> 4. 定义了一个报告特征类型函数report_feature_type，用于查询一个特征的类型并打印到列表窗口。
> 5. 主函数do_it循环调用select_a_feature和report_feature_type，让用户选择特征并打印其类型。
> 6. 定义了ufusr函数，作为NX二次开发的入口点。在ufusr中初始化NX API，调用do_it函数，然后终止NX API。
> 7. 定义了ufusr_ask_unload函数，用于在NX关闭二次开发应用程序时立即卸载。
>
> 总体来说，这段代码实现了一个简单的NX二次开发应用程序，用于让用户选择特征并打印其类型，方便用户查看特征信息。代码结构清晰，功能明确，可以作为NX二次开发的一个参考示例。
>
