### 【0332】ask master of instance 获取实例的主件

#### 代码

```cpp
    /*HEAD ASK_MASTER_OF_INSTANCE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本中新增的函数。它用于将系统日志消息打印到文件中。 */  
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
    static tag_t ask_master_of_instance(tag_t instance)  
    {  
        int  
            n_feats,  
            n_sets;  
        tag_t  
            *feats,  
            master = NULL_TAG,  
            *sets;  
        char  
            *type;  
        UF_CALL(UF_MODL_ask_feat_type(instance, &type));  
        if (!strcmp(type, "INSTNACE_SET") || !strcmp(type, "INSTANCE_SET"))  
        {  
            UF_CALL(UF_MODL_ask_sets_of_member(instance, &sets, &n_sets));  
            if (n_sets > 0)  
            {  
                UF_CALL(UF_MODL_ask_all_members_of_set(sets[0], &feats, &n_feats));  
                if (n_feats > 0)  
                {  
                    master = feats[0];  
                    UF_free(feats);  
                }  
                UF_free(sets);  
            }  
        }  
        if (!strcmp(type, "INSTANCE"))  
            UF_CALL(UF_MODL_ask_master(instance, &master));  
        UF_free(type);  
        return (master);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat,  
            master;  
        while ((feat = select_a_feature("Select INSTANCE feature")) != NULL_TAG)  
        {  
            report_feature_name_and_type("feat", feat);  
            if ((master = ask_master_of_instance(feat)) != NULL_TAG)  
                report_feature_name_and_type("master", master);  
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

> 这段代码是NX的二次开发代码，主要功能是查询特征实例的主特征。具体来说：
>
> 1. 定义了一个错误报告函数report_error，用于打印函数调用失败的信息。
> 2. 定义了一个选择特征的函数select_a_feature，用于从用户选择的特征中返回第一个特征。
> 3. 定义了一个报告特征名称和类型的函数report_feature_name_and_type，用于打印特征的信息。
> 4. 定义了一个查询实例的主特征的函数ask_master_of_instance，根据实例类型，查询实例集合或直接查询实例的主特征。
> 5. 定义了一个主函数do_it，循环选择特征，打印特征信息，并查询特征实例的主特征，打印主特征信息。
> 6. 定义了ufusr函数，作为NX二次开发的入口函数，初始化环境后调用do_it函数，完成后终止环境。
> 7. 定义了ufusr_ask_unload函数，用于卸载二次开发代码时立即卸载。
>
> 综上，这段代码实现了选择特征实例，打印特征信息，查询特征实例的主特征并打印主特征信息的功能。通过NX二次开发接口实现，采用了面向对象的编程思想。
>
