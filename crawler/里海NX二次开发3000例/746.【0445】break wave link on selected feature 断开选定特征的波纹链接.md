### 【0445】break wave link on selected feature 断开选定特征的波纹链接

#### 代码

```cpp
    /*HEAD BREAK_WAVE_LINK_ON_SELECTED_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_wave.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，我了解到UF_print_syslog是V18版本中新增的函数。 */  
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
        tag_t  
            feat;  
        while ((feat = select_a_feature("Break Link")) != NULL_TAG)  
        {  
            UF_CALL(UF_WAVE_set_link_data(feat, NULL_TAG, NULL_TAG, FALSE));  
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

> 这段代码是一个NX Open C++的二次开发程序，主要实现了选择一个特征并断开其链接的功能。
>
> 具体步骤如下：
>
> 1. 包含必要的NX Open头文件。
> 2. 定义一个错误报告函数report_error，用于在出错时输出错误信息。
> 3. 定义一个选择特征函数select_a_feature，用于提示用户选择一个特征，并返回特征标签。
> 4. 定义一个执行函数do_it，用于循环调用select_a_feature选择特征，然后调用UF_WAVE_set_link_data函数断开特征的链接。
> 5. 定义ufusr函数作为程序的入口，初始化NX环境，调用do_it执行主体功能，最后终止NX环境。
> 6. 定义ufusr_ask_unload函数，用于在程序卸载时立即卸载。
>
> 整体而言，这是一个比较简单的NX二次开发示例，实现了选择特征并断开链接的功能。
>
