### 【1524】list features shown in part navigator 在部件导航器中显示的特性列表

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_kf.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int gtac_browseable_features(tag_t **pnt_features)  
    {  
        logical  
            is_init = false;  
        int  
            ii,  
            n_feats;  
        tag_t  
            feat;  
        char  
            rule[133],  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_KF_value_p_t  
            value;  
        UF_KF_list_p_t  
            list;  
        UF_CALL(UF_KF_is_initialized(&is_init));  
        if (!is_init)  
            UF_CALL(UF_KF_init_part(UF_ASSEM_ask_work_part()));  
        UF_CALL(uc4577(unique));  
        UF_CALL(UF_KF_create_rule_no_update("root:", unique, "List",  
            "mqc_askBrowseableFeatures()", ""));  
        sprintf(rule, "root:%s:", unique);  
        UF_CALL(UF_KF_evaluate_rule(rule, &value));  
        UF_CALL(UF_KF_delete_instance_rule("root:", unique));  
        UF_CALL(UF_KF_ask_list(value, &list));  
        UF_KF_free_rule_value(value);  
        UF_CALL(UF_KF_ask_list_count(list, &n_feats));  
        if (n_feats == 0) return 0;  
        UF_CALL(allocate_memory(n_feats * sizeof(tag_t), (void **)pnt_features));  
        for (ii = 0; ii < n_feats; ii++)  
        {  
            UF_CALL(UF_KF_ask_list_item(list, ii+1, &value));  
            UF_CALL(UF_KF_ask_integer(value, &feat));  
            (*pnt_features)[ii] = feat;  
            UF_KF_free_rule_value(value);  
        }  
        return n_feats;  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    static void report_feature_sysname(tag_t feature)  
    {  
        char  
            *sysname;  
        UF_CALL(UF_MODL_ask_feat_sysname(feature, &sysname));  
        ECHO("%s\n", sysname);  
        UF_free(sysname);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *pnt_features;  
        n = gtac_browseable_features(&pnt_features);  
        WRITE_D(n);  
        for (ii = 0; ii < n; ii++)  
            report_feature_sysname(pnt_features[ii]);  
        if (n > 0) UF_free(pnt_features);  
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

> 这段代码是一个NX Open的二次开发示例，主要实现了以下功能：
>
> 1. 定义错误报告函数：report_error，用于在调用NX API失败时，输出错误代码、错误信息以及出错的函数调用。
> 2. 定义内存分配函数：allocate_memory，用于调用NX的内存分配接口。
> 3. 定义获取特征函数：gtac_browseable_features，用于通过Knowledge Fusion规则获取当前工作部件的浏览特征，并返回特征数组及其数量。
> 4. 定义输出特征名称函数：report_feature_sysname，用于输出特征的系统名称。
> 5. 定义主函数：do_it，用于执行获取特征、输出特征数量的操作，并遍历输出所有特征的名称。
> 6. 定义ufusr函数：作为NX二次开发的入口函数，负责初始化NX、执行主函数、终止NX。
> 7. 定义卸载函数：ufusr_ask_unload，用于卸载二次开发库。
>
> 整体而言，该代码实现了通过Knowledge Fusion获取当前工作部件的特征，并遍历输出特征名称的功能，是一个典型的NX二次开发示例。
>
