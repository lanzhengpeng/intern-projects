### 【1405】highlight selected features references 高亮所选特征引用

#### 代码

```cpp
    /*HEAD HIGHLIGHT_SELECTED_FEATURES_REFERENCES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define ECHOS(X)   (printf("%s = %s\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。该函数用于打印系统日志信息。 */  
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
    static void suppress_from_feat(tag_t feat, tag_t *body)  
    {  
        uf_list_p_t  
            feat_list,  
            sup_list;  
        UF_CALL(UF_MODL_ask_feat_body(feat, body));  
        UF_CALL(UF_MODL_ask_body_feats(*body, &feat_list));  
        for (sup_list=feat_list; sup_list->eid != feat; sup_list=sup_list->next);  
        UF_CALL(UF_MODL_suppress_feature(sup_list));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
    }  
    static void unsuppress_from_feat(tag_t feat, tag_t body)  
    {  
        uf_list_p_t  
            feat_list,  
            sup_list;  
        UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
        for (sup_list=feat_list; sup_list->eid != feat; sup_list=sup_list->next);  
        UF_CALL(UF_MODL_unsuppress_feature(sup_list));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
    }  
    static void highlight_feature_references(tag_t feat)  
    {  
        int  
            ii,  
            n_parents;  
        tag_t  
            body,  
            *parents;  
        char  
            **parent_names;  
        UF_CALL(UF_MODL_ask_references_of_features(&feat, 1, &parents,  
            &parent_names, &n_parents));  
        if (n_parents > 0)  
        {  
            suppress_from_feat(feat, &body);  
            for (ii = 0; ii < n_parents; ii++)  
            {  
                ECHO(ii);  
                ECHOS(parent_names[ii]);  
                UF_CALL(UF_DISP_set_highlight(parents[ii], TRUE));  
            }  
            uc1601("References highlighted", TRUE);  
            for (ii = 0; ii < n_parents; ii++)  
                UF_CALL(UF_DISP_set_highlight(parents[ii], FALSE));  
            unsuppress_from_feat(feat, body);  
            UF_free(parents);  
            UF_free_string_array(n_parents, parent_names);  
        }  
        else  
            uc1601("Feature has no references", TRUE);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Select a feature")) != NULL_TAG)  
            highlight_feature_references(feat);  
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

> 这段代码是NX Open API的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告：定义了report_error函数，用于在函数调用出错时报告错误信息。
> 2. 特征选择：定义了select_a_feature函数，用于提示用户选择一个特征，并返回该特征。
> 3. 特征抑制与取消抑制：定义了suppress_from_feat和unsuppress_from_feat函数，用于在特征体中抑制和取消抑制一个特征。
> 4. 高亮显示特征引用：定义了highlight_feature_references函数，用于高亮显示一个特征的所有引用。
> 5. 主函数：定义了do_it函数，用于循环选择特征并高亮显示其引用。
> 6. ufusr函数：这是NX Open API的入口函数，用于初始化NX Open API，执行主函数，然后终止NX Open API。
> 7. 卸载函数：定义了ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 总的来说，这段代码通过NX Open API实现了选择特征并高亮显示其引用的功能，以辅助用户进行特征管理。
>
