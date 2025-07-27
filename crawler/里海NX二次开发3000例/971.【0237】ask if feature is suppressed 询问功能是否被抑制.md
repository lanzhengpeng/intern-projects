### 【0237】ask if feature is suppressed 询问功能是否被抑制

#### 代码

```cpp
    /*HEAD ASK_IF_FEATURE_IS_SUPPRESSED CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数。 */  
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
    static void do_it(void)  
    {  
        int  
            suppressed;  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Select Feature:")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_suppress_feature(feat, &suppressed));  
                if (suppressed)  
                    uc1601("Feature is Suppressed", TRUE);  
                else  
                    uc1601("Feature is NOT Suppressed", TRUE);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一段NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个名为report_error的函数，用于在调用NX API函数发生错误时输出错误信息。该函数会输出错误代码、错误消息以及调用失败的API函数名。
> 2. 特征选择函数：定义了一个名为select_a_feature的函数，用于提示用户选择一个特征。该函数会调用NX的UF_UI_select_feature API获取用户选择，并返回第一个特征。
> 3. 特征抑制询问函数：定义了一个名为do_it的函数，用于循环调用select_a_feature选择特征，然后调用NX的UF_MODL_ask_suppress_feature API询问该特征是否被抑制，并输出结果。
> 4. 用户自定义函数入口：定义了名为ufusr的函数，这是NX二次开发的入口函数。在该函数中，初始化NX环境，调用do_it函数实现功能，并在结束时终止NX环境。
> 5. 卸载询问函数：定义了名为ufusr_ask_unload的函数，用于在二次开发模块卸载时调用，返回立即卸载的标志。
>
> 通过这段代码，用户可以方便地选择特征并查询其抑制状态，实现了特征抑制查询的功能。代码结构清晰，采用了模块化设计，易于阅读和维护。
>
