### 【1387】highlight master feature of selected instance 高亮选定实例的主特征

#### 代码

```cpp
    /*HEAD HIGHLIGHT_MASTER_FEATURE_OF_SELECTED_INSTANCE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog 是 V18 版本新增的功能。 */  
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
    static void report_feature_name_and_type(char *title, tag_t feat)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feat, &type)))  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            sprintf(msg, "%s = %d %s (%s)\n", title, feat, name, type);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    static tag_t ask_first_instance(tag_t instance)  
    {  
        int  
            n_feats,  
            n_sets;  
        tag_t  
            *feats,  
            first = NULL_TAG,  
            iset,  
            *sets;  
        char  
            *type;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_ask_feat_type(instance, &type));  
        if (!strcmp(type, "INSTNACE_SET") || !strcmp(type, "INSTANCE_SET"))  
        {  
            UF_CALL(UF_MODL_ask_sets_of_member(instance, &sets, &n_sets));  
            if (n_sets > 0)  
            {  
                UF_CALL(UF_MODL_ask_all_members_of_set(sets[0], &feats, &n_feats));  
                if (n_feats > 0)  
                {  
                    first = feats[0];  
                    UF_free(feats);  
                }  
                UF_free(sets);  
            }  
        }  
        if (!strcmp(type, "INSTANCE"))  
        {  
            UF_CALL(UF_MODL_ask_instance_iset(instance, &iset));  
            UF_CALL(UF_MODL_ask_instance(iset, &feat_list));  
            UF_CALL(UF_MODL_ask_list_item(feat_list, 0, &first));  
            UF_CALL(UF_MODL_delete_list(&feat_list));  
        }  
        UF_free(type);  
        return (first);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat,  
            master;  
        while ((feat = select_a_feature("Select INSTANCE feature")) != NULL_TAG)  
        {  
            report_feature_name_and_type("feat", feat);  
            if ((master = ask_first_instance(feat)) != NULL_TAG)  
            {  
                report_feature_name_and_type("master", master);  
                UF_DISP_set_highlight(feat, TRUE);  
                UF_DISP_set_highlight(master, TRUE);  
                uc1601("Feature & Master highlighted", TRUE);  
                UF_DISP_set_highlight(feat, FALSE);  
                UF_DISP_set_highlight(master, FALSE);  
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

> 这段代码是NX Open的二次开发示例代码，主要用于演示如何选择一个实例特征，并高亮显示其主特征。
>
> 主要功能包括：
>
> 1. 错误报告函数：封装了错误报告的函数report_error，用于打印错误信息到日志和对话框。
> 2. 选择特征函数：封装了特征选择函数select_a_feature，通过UF_UI_select_feature函数让用户选择一个特征，并返回特征tag。
> 3. 查询特征信息函数：封装了查询特征名称和类型的函数report_feature_name_and_type。
> 4. 查询实例集函数：封装了查询实例集的函数ask_first_instance，用于获取特征对应的第一个实例或实例集成员。
> 5. 主体函数：主体函数do_it用于循环选择实例特征，并高亮显示其主特征。
> 6. 初始化和终止函数：ufusr函数初始化和终止NX Open。
> 7. 卸载函数：提供了卸载函数ufusr_ask_unload。
>
> 该示例代码通过封装函数的方式简化了二次开发流程，实现了选择实例特征并高亮显示其主特征的功能，对NX Open的二次开发具有一定的参考价值。
>
