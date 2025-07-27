### 【2163】report is feature body 报告是否为特征体

#### 代码

```cpp
    /*HEAD REPORT_IS_FEATURE_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释内容，UF_print_syslog 是 V18 版本新增的功能。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    /* qq3123197280 */  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
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
    static void do_it(void)  
    {  
        tag_t  
            feat = NULL_TAG;  
        logical  
            is_feat = FALSE;  
        while((feat=select_a_feature("Select Feature:"))!=NULL_TAG)  
        {  
            UF_CALL( UF_MODL_is_body_feature( feat, &is_feat));  
            WRITE_L( is_feat );  
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

> 这段代码是一个NX Open C++的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于输出函数调用失败的信息，包括文件名、行号、错误码和错误消息。
> 2. 定义了一个逻辑值输出函数write_logical_to_listing_window，用于在列表窗口中输出逻辑值的真或假。
> 3. 定义了一个选择特征函数select_a_feature，使用UI函数让用户选择一个特征，并返回特征tag。
> 4. 定义了一个主体函数do_it，循环让用户选择特征，然后判断特征是否为body特征，并将结果输出到列表窗口。
> 5. 定义了ufusr函数，初始化NX环境，调用do_it函数执行主体逻辑，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码通过NX Open C++的UI和建模接口，实现了让用户选择特征，判断特征是否为body特征，并将结果输出到列表窗口的功能。代码结构清晰，使用错误处理机制，是一个典型的NX二次开发示例。
>
