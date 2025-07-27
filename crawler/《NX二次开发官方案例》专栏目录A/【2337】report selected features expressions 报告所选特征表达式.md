### 【2337】report selected features expressions 报告所选特征表达式

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_FEATURES_EXPRESSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
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
    static void report_feature_name_and_type(char *title, tag_t feat)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feat, &type)))  
        {  
            sprintf(msg, "%s = %d %s (%s)\n", title, feat, name, type);  
            ECHO(msg);  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    #define RFE(X) report_feature_expressions(#X, X)  
    static void report_feature_expressions(char *which, tag_t feat)  
    {  
        int  
            ii,  
            n_exps;  
        tag_t  
            *exps,  
            master;  
        char  
            **descs,  
            msg[MAX_LINE_SIZE+1],  
            *string,  
            *type;  
        report_feature_name_and_type(which, feat);  
        if(!UF_CALL(UF_MODL_ask_exp_desc_of_feat(feat, &n_exps, &descs, &exps)))  
        {  
            for (ii = 0; ii < n_exps; ii++)  
            {  
                if (!UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string)))  
                {  
                    sprintf(msg, "  %d.  %s (%s)\n", ii+1, string, descs[ii]);  
                    UF_free(string);  
                    ECHO(msg);  
                }  
            }  
            UF_free(exps);  
            UF_free_string_array(n_exps, descs);  
        }  
        UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
        if (!strcmp(type, "INSTANCE"))  
        {  
            UF_CALL(UF_MODL_ask_master(feat, &master));  
            RFE(master);  
        }  
        UF_free(type);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Report expressions")) != NULL_TAG)  
            RFE(feat);  
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

> 这段代码是一个NX的二次开发示例，主要功能是选择特征并报告其表达式信息。以下是代码的主要组成部分：
>
> 1. 头文件包含：引入了UF、UF_UI、UF_MODL等NX API的头文件。
> 2. 宏定义：ECHO：用于输出信息到日志窗口和系统日志。UF_CALL：用于调用NX API函数，并捕获错误。
> 3. ECHO：用于输出信息到日志窗口和系统日志。
> 4. UF_CALL：用于调用NX API函数，并捕获错误。
> 5. 错误报告函数：report_error，用于在API调用出错时报告错误。
> 6. 选择特征函数：select_a_feature，用于提示用户选择一个特征，并返回特征tag。
> 7. 报告特征信息函数：report_feature_name_and_type：用于报告特征名称和类型。report_feature_expressions：用于报告特征的所有表达式，包括名称、类型和描述。如果特征是实例，还会递归报告其主模型特征的表达式。
> 8. report_feature_name_and_type：用于报告特征名称和类型。
> 9. report_feature_expressions：用于报告特征的所有表达式，包括名称、类型和描述。
> 10. 如果特征是实例，还会递归报告其主模型特征的表达式。
> 11. 主函数：do_it，循环调用select_a_feature和report_feature_expressions，直到用户选择退出。
> 12. ufusr函数：NX二次开发程序的入口函数，初始化NX，调用do_it，然后终止NX。
> 13. 卸载函数：ufusr_ask_unload，用于在卸载时立即卸载二次开发程序。
>
> 总体来说，这段代码实现了选择特征并查看其表达式信息的功能，通过递归处理实例特征，可以查看整个特征树的表达式信息。
>
