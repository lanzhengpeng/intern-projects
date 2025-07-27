### 【0573】create and delete feature set 创建和删除特征集

#### 代码

```cpp
    /*HEAD CREATE_AND_DELETE_FEATURE_SET CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，我理解UF_print_syslog是V18版本中的新功能。所以，我回答如下：

UF_print_syslog是V18版本中的新功能。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
    }  
    static void do_it(void)  
    {  
        int  
            n_feats = 0;  
        tag_t  
            feat_set = NULL_TAG,  
            *feats = NULL;  
        char  
            prompt[132] = { "Select features for feature set:" };  
        uf_list_p_t  
            feat_list;  
        if ((n_feats = select_features(prompt, &feats)) > 0)  
        {  
            UF_CALL( UF_MODL_create_set_of_feature( "Feature_set", feats,   
                                                n_feats, FALSE, &feat_set ));   
            WRITE_D( feat_set );  
            uc1601( "Pause", TRUE ); /* look at the model   
                                            navigator to see the fset */  
            UF_CALL(UF_MODL_create_list(&feat_list));  
            UF_CALL(UF_MODL_put_list_item(feat_list, feat_set));  
            UF_CALL(UF_MODL_delete_feature(feat_list));  
            UF_CALL(UF_MODL_delete_list(&feat_list));  
            if (n_feats > 0) UF_free(feats);  
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

> 这段代码是一个NX Open的二次开发示例，用于创建和删除特征集。主要步骤如下：
>
> 1. 包含了必要的NX Open头文件。
> 2. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 3. 定义了整数写入列表窗口的函数write_integer_to_listing_window。
> 4. 定义了选择特征的函数select_features。
> 5. 定义了主体函数do_it，其中：调用select_features获取用户选择的特征。如果有特征被选择，则调用UF_MODL_create_set_of_feature创建特征集。调用uc1601暂停，以便查看模型导航器中的特征集。创建特征列表，将特征集添加到列表中，然后删除特征列表和特征集。释放特征数组内存。
> 6. 调用select_features获取用户选择的特征。
> 7. 如果有特征被选择，则调用UF_MODL_create_set_of_feature创建特征集。
> 8. 调用uc1601暂停，以便查看模型导航器中的特征集。
> 9. 创建特征列表，将特征集添加到列表中，然后删除特征列表和特征集。
> 10. 释放特征数组内存。
> 11. 定义了ufusr函数作为NX的入口点，在NX启动时调用UF_initialize初始化环境，执行主体函数do_it，最后调用UF_terminate终止环境。
> 12. 定义了卸载函数ufusr_ask_unload，用于卸载用户自定义的动态库。
>
> 总体来说，这段代码通过UI函数与用户交互，通过建模函数创建和删除特征集，展示了NX Open API的典型用法。
>
