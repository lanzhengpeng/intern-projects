### 【1549】list udf children features name and type 列出UDF子特征的名称和类型

#### 代码

```cpp
    /*HEAD LIST_UDF_CHILDREN_FEATURES_NAME_AND_TYPE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能，其功能是打印系统日志。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_FNT(X) report_feature_name_and_type(#X, X)  
    static void report_feature_name_and_type(char *which, tag_t feature)  
    {  
        char  
            *name,  
            *type,  
            msg[MAX_LINE_LENGTH+1];  
        if (!UF_CALL(UF_MODL_ask_feat_name(feature, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feature, &type)))  
        {  
            sprintf(msg, "%s - tag_t %d - %s (%s)\n", which, feature, name, type);  
            WRITE(msg);  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            *feats,  
            udf;  
        while ((udf = select_a_feature("Select UDF to report")) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_MODL_ask_features_of_udf(udf, &feats, &n_feats)))  
            {  
                for (ii = 0; ii < n_feats; ii++)  
                {  
                    WRITE_FNT(feats[ii]);  
                }  
                if (n_feats > 0) UF_free(feats);  
           }  
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

> 这段NX二次开发代码的主要功能是查询用户定义特征(UDF)包含的子特征，并输出每个子特征的名字和类型。
>
> 代码的主要逻辑包括以下几个部分：
>
> 1. 包含NX Open API的头文件，用于调用NX的API函数。
> 2. 定义了一个报告错误的函数report_error，用于在调用NX API函数失败时输出错误信息。
> 3. 定义了一个选择特征的函数select_a_feature，用于让用户选择一个特征，并返回该特征的tag。
> 4. 定义了一个输出特征名和类型的函数report_feature_name_and_type。
> 5. 定义了主函数do_it，循环调用select_a_feature让用户选择UDF，然后调用UF_MODL_ask_features_of_udf查询UDF包含的子特征，并调用report_feature_name_and_type输出每个子特征的信息。
> 6. 定义了ufusr函数作为NX的二次开发入口函数，调用do_it执行主功能。
> 7. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 总的来说，这段代码实现了查询UDF的子特征，并输出每个子特征的名字和类型的功能。
>
