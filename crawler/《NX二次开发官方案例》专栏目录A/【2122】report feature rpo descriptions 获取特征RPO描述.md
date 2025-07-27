### 【2122】report feature rpo descriptions 获取特征RPO描述

#### 代码

```cpp
    /*HEAD REPORT_FEATURE_RPO_DESCRIPTIONS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    static void report_feature_rpo_descriptions(tag_t feat)  
    {  
        int  
            ii,  
            n_exps;  
        tag_t  
            *exps;  
        char  
            **descriptions;  
        report_feature_name(feat);  
        UF_CALL(UF_MODL_ask_rpo_desc_of_feat(feat, &n_exps, &descriptions, &exps));  
        for (ii = 0; ii < n_exps; ii++)  
            printf("  %d.  %d = %s\n", ii+1, exps[ii], descriptions[ii]);  
        UF_free(exps);  
        UF_free_string_array(n_exps, descriptions);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Select feature")) != NULL_TAG)  
            report_feature_rpo_descriptions(feat);  
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

> 根据代码内容，这是一段用于NX的二次开发代码，主要功能是选择一个特征并报告其相关的RPO描述。具体介绍如下：
>
> 1. 包含必要的头文件，如stdio.h、string.h、uf.h等。
> 2. 定义了UF_CALL宏，用于调用UF函数，并在调用失败时记录错误信息。
> 3. 实现了report_error函数，用于输出错误信息到系统日志和列表窗口。
> 4. 实现了select_a_feature函数，用于选择一个特征，并返回其tag。
> 5. 实现了report_feature_name函数，用于报告特征名称。
> 6. 实现了report_feature_rpo_descriptions函数，用于报告特征相关的RPO描述。
> 7. 实现了do_it函数，循环调用select_a_feature和report_feature_rpo_descriptions，直到用户取消选择特征。
> 8. 实现了ufusr函数，这是NX二次开发的入口函数。初始化后调用do_it，然后终止。
> 9. 实现了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码通过NX的二次开发接口，实现了选择特征并查看其RPO描述的功能。
>
