### 【1037】delete selected features 删除选定特征

#### 代码

```cpp
    /*HEAD DELETE_SELECTED_FEATURES CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *string;  
        UF_CALL(UF_MODL_ask_feat_name(feat, &string));  
        printf("%d = %s\n", feat, string);  
        UF_free(string);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *features;  
        uf_list_p_t  
            feat_list;  
        while ((n = select_features("Select features to delete", &features)) > 0)  
        {  
            UF_CALL(UF_MODL_create_list(&feat_list));  
            for (ii = 0; ii < n; ii++)  
            {  
                printf("%d.  ", ii+1);  
                report_feature_name(features[ii]);  
                UF_CALL(UF_MODL_put_list_item(feat_list, features[ii]));  
            }  
            UF_CALL(UF_MODL_delete_feature(feat_list));  
            UF_CALL(UF_MODL_delete_list(&feat_list));  
            UF_free(features);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是让用户选择要删除的特征，然后删除这些特征。以下是代码的主要功能：
>
> 1. 引入必要的头文件，包括uf.h、uf_ui.h和uf_modl.h等。
> 2. 定义了report_error函数，用于打印错误信息。
> 3. 定义了select_features函数，用于让用户选择特征，并返回选择的特征数量。
> 4. 定义了report_feature_name函数，用于打印特征名称。
> 5. 定义了do_it函数，包含主循环，让用户反复选择特征进行删除，直到用户取消。
> 6. 定义了ufusr函数，这是NX二次开发的标准入口函数，在NX启动时调用。在这个函数中初始化NX环境，调用do_it函数执行主体功能，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，用于在卸载NX时调用，返回立即卸载标志。
>
> 通过以上代码，可以实现让用户交互式选择特征并进行删除的功能，是NX二次开发的一个简单示例。
>
