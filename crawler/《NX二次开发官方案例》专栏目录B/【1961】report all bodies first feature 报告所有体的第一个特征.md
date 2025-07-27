### 【1961】report all bodies first feature 报告所有体的第一个特征

#### 代码

```cpp
    /*HEAD REPORT_ALL_BODIES_FIRST_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，V18版本新增了UF_print_syslog函数，该函数用于打印系统日志。在翻译时，只需要回答函数的名称和作用，不需要添加其他无关内容。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_feature_name_and_type(tag_t feature)  
    {  
        char  
            *feature_name,  
            *feature_type;  
        WRITE_D(feature);  
        if (!UF_CALL(UF_MODL_ask_feat_name(feature, &feature_name)))  
        {  
            WRITE_S(feature_name);  
            UF_free(feature_name);  
        }  
        if (!UF_CALL(UF_MODL_ask_feat_type(feature, &feature_type)))  
        {  
            WRITE_S(feature_type);  
            UF_free(feature_type);  
        }  
    }  
    static tag_t ask_body_first_feat(tag_t body)  
    {  
        tag_t  
            feat;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
        UF_CALL(UF_MODL_ask_list_item(feat_list, 0, &feat));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
        return feat;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
           report_feature_name_and_type(ask_body_first_feat(body));  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告：通过UF_CALL宏调用NX API，并使用report_error函数来报告错误。该函数会打印错误码、错误消息和出错的API调用到系统日志和列表窗口。
> 2. 遍历部件中的实体：ask_next_body函数用于遍历部件中的实体，返回下一个实体的tag。
> 3. 写入列表窗口：write_integer_to_listing_window和write_string_to_listing_window函数用于将整数和字符串写入列表窗口。
> 4. 报告特征名称和类型：report_feature_name_and_type函数用于查询特征的名称和类型，并打印到列表窗口。
> 5. 查询实体的第一个特征：ask_body_first_feat函数用于查询实体的第一个特征，并返回该特征的tag。
> 6. 执行主逻辑：do_it函数遍历部件中的实体，对每个实体查询第一个特征，并报告该特征的名称和类型。
> 7. ufusr函数：ufusr是NX二次开发的入口函数，初始化NX环境，调用do_it执行主逻辑，然后终止NX环境。
> 8. 卸载请求：ufusr_ask_unload函数返回立即卸载，表示二次开发应用程序可以被立即卸载。
>
> 总体来说，这段代码通过遍历部件中的实体，查询每个实体的第一个特征，并报告该特征的名称和类型，实现了对NX模型特征信息的查询和报告功能。
>
