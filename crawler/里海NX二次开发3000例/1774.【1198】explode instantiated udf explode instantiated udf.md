### 【1198】explode instantiated udf explode instantiated udf

#### 代码

```cpp
    /*HEAD EXPLODE_INSTANTIATED_UDF CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog是V18版本新增的，请只回答翻译，不要添加其他无关内容。 */  
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
    static void do_it(void)  
    {  
        int  
            n_feats;  
        tag_t  
            *feats,  
            udf;  
        uf_list_p_t  
            udf_list;  
        while ((udf = select_a_feature("Select UDF to explode")) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_MODL_ask_all_members_of_set(udf, &feats, &n_feats)))  
            {  
                UF_CALL(UF_MODL_edit_set_members(udf, NULL, 0));  
                UF_CALL(UF_MODL_create_list(&udf_list));  
                UF_CALL(UF_MODL_put_list_item(udf_list, udf));  
                UF_CALL(UF_MODL_delete_feature(udf_list));  
                UF_CALL(UF_MODL_delete_list(&udf_list));  
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

> 这段代码是一个NX Open的UF程序，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在发生UF函数调用错误时打印错误信息。
> 2. 定义了一个选择特征函数select_a_feature，用于提示用户选择一个UDF特征。
> 3. 定义了一个执行函数do_it，用于遍历用户选择的UDF特征，并调用UF_MODL_ask_all_members_of_set获取UDF成员特征，然后调用UF_MODL_edit_set_members删除UDF特征，接着调用UF_MODL_create_list、UF_MODL_put_list_item、UF_MODL_delete_feature、UF_MODL_delete_list等函数删除UDF成员特征。
> 4. 定义了ufusr函数，用于初始化UF环境，执行do_it函数，然后终止UF环境。
> 5. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示立即卸载UF环境。
>
> 总体来说，这段代码的主要功能是让用户选择UDF特征，然后删除这些UDF特征及其成员特征。
>
