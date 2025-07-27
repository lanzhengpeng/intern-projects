### 【3025】unsuppress selected features 选择要取消抑制的特征

#### 代码

```cpp
    /*HEAD UNSUPPRESS_SELECTED_FEATURES CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。 */  
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
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n,  
            suppressed;  
        tag_t  
            *feats;  
        uf_list_p_t  
            feat_list;  
        while ((n = select_features("Select features to unsuppress", &feats)) != 0)  
        {  
            UF_CALL(UF_MODL_create_list(&feat_list));  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_MODL_ask_suppress_feature(feats[ii], &suppressed));  
                if (suppressed)  
                    UF_CALL(UF_MODL_put_list_item(feat_list, feats[ii]));  
            }  
            UF_CALL(UF_MODL_unsuppress_feature(feat_list));  
            UF_CALL(UF_MODL_delete_list(&feat_list));  
            UF_free(feats);  
        }  
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

> 这段代码是NX的二次开发示例，用于选择并取消抑制特征。主要功能包括：
>
> 1. 定义错误报告函数report_error，用于打印错误信息。
> 2. 定义特征选择函数select_features，调用UF_UI_select_feature选择特征，返回特征数量。
> 3. 定义执行函数do_it，循环调用select_features选择特征，创建特征列表，判断特征是否被抑制，如果是，加入列表，最后调用UF_MODL_unsuppress_feature取消抑制特征。
> 4. 定义ufusr函数，初始化NX环境，调用do_it执行取消抑制，最后终止NX环境。
> 5. 定义ufusr_ask_unload函数，用于卸载用户函数。
>
> 这段代码通过交互式选择要取消抑制的特征，并通过NX提供的特征操作API实现了特征取消抑制的功能。通过错误报告和用户交互，提高了代码的健壮性和可用性。
>
