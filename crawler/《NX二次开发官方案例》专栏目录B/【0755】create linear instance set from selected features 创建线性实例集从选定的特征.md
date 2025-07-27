### 【0755】create linear instance set from selected features 创建线性实例集从选定的特征

#### 代码

```cpp
    /*HEAD CREATE_LINEAR_INSTANCE_SET_FROM_SELECTED_FEATURES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能。 */  
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
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    static void do_it(void)  
    {  
        int  
            n_feats;  
        tag_t  
            *feats,  
            iset;  
        uf_list_p_t  
            feat_list;  
        while ((n_feats = select_features("Select feature for iset", &feats)) > 0)  
        {  
            feat_list = make_a_list(n_feats, feats);  
            UF_CALL(UF_MODL_create_linear_iset(0,"2","3","2","3",feat_list,&iset));  
            UF_free(feats);  
            UF_MODL_delete_list(&feat_list);  
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

> 这段代码是NX Open C++的二次开发示例代码，主要实现了从NX部件中创建线性实例集的功能。
>
> 1. 错误报告函数：report_error()用于输出错误代码和错误信息，以便调试。
> 2. 选择特征函数：select_features()用于提示用户选择特征，并返回特征数组。
> 3. 创建列表函数：make_a_list()用于将特征数组转换成NX的列表结构。
> 4. 主函数：do_it()用于不断提示用户选择特征，直到用户取消。对于每次选择的特征，都会创建一个线性实例集。
> 5. ufusr入口函数：ufusr()是NX二次开发的入口函数，在这里调用do_it()执行主要功能。
> 6. 卸载函数：ufusr_ask_unload()表示该程序可以被NX立即卸载。
>
> 综合来看，这段代码通过交互方式选择特征，并创建了对应的线性实例集，实现了在NX中创建实例集的功能。
>
