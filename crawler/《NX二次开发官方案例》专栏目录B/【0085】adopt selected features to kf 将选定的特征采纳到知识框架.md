### 【0085】adopt selected features to kf 将选定的特征采纳到知识框架

#### 代码

```cpp
    /*HEAD ADOPT_SELECTED_FEATURES_TO_KF CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：注意，UF_print_syslog是V18版本中新增的。只回答译文，不要废话。 */  
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
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_CALL(UF_UI_select_feature(prompt, UF_UI_FEAT_SEL_TYPE_BROWSEABLE,  
            &cnt, feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
    }  
    static void do_it(void)  
    {  
        int  
            n_feats;  
        tag_t  
            *feats;  
        UF_KF_instance_p_t  
            *instances;  
        while ((n_feats = select_features("Select features to Adopt", &feats)) > 0)  
        {  
            if (!UF_CALL(UF_KF_adopt_ug_objects(feats, n_feats, &instances)))  
                UF_free(instances);  
            UF_free(feats);  
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

> 这段NX二次开发代码主要用于实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印出NX API调用错误信息。
> 2. 定义了一个特征选择函数select_features，用于让用户通过图形界面选择特征。
> 3. 主函数do_it中，循环调用select_features获取用户选择的一组特征，然后调用UF_KF_adopt_ug_objects将这些特征添加到当前打开的NX部件实例中。
> 4. 定义了ufusr函数，在NX启动时初始化NX环境，调用do_it执行主体功能，然后终止NX环境。
> 5. 定义了ufusr_ask_unload函数，用于在NX卸载该程序时立即执行。
>
> 总的来说，这段代码实现了一个简单的NX特征添加功能，让用户可以通过图形界面选择特征，然后添加到当前打开的NX部件实例中。
>
