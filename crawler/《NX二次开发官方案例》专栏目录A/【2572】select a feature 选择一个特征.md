### 【2572】select a feature 选择一个特征

#### 代码

```cpp
    /*HEAD SELECT_A_FEATURE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog是V18版本新增的，请只回答译文，不要添加无关内容。

原文：
UF_print_syslog is new in V18，只回答译文，不要废话。

翻译：
UF_print_syslog是V18版本新增的，请只回答译文，不要添加无关内容。 */  
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
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *string;  
        UF_CALL(UF_MODL_ask_feat_name(feat, &string));  
        printf("%d = %s\n", feat, string);  
        UF_free(string);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feature;  
        while ((feature = select_a_feature("Select feature")) != NULL_TAG)  
            report_feature_name(feature);  
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

> 这段代码是用于NX软件进行二次开发的一个示例，其主要功能包括：
>
> 1. 错误报告函数：用于在函数调用失败时，输出错误代码和错误信息。
> 2. 选择特征函数：通过用户界面提示用户选择一个特征，并返回特征标签。
> 3. 报告特征名称函数：根据特征标签，查询特征名称并输出。
> 4. 主函数：循环调用选择特征函数，直到用户不选择特征为止，然后调用报告特征名称函数输出特征名称。
> 5. NX初始化/终止函数：在NX软件启动和关闭时调用，用于初始化和清理NX环境。
> 6. 卸载请求函数：返回立即卸载标志，用于在插件卸载时调用。
>
> 整体来看，这段代码实现了通过用户界面交互选择NX模型中的特征，并输出特征名称的功能。
>
