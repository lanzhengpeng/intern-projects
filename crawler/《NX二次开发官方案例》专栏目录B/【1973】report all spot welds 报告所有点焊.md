### 【1973】report all spot welds 报告所有点焊

#### 代码

```cpp
    /*HEAD REPORT_ALL_SPOT_WELDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我总结了以下关键信息：

1. UF_print_syslog是V18版本新增的功能。

2. UF_print_syslog用于打印系统日志信息。

3. 在V18版本之前，系统日志信息是直接打印到控制台的。

4. 在V18版本中，系统日志信息改为通过UF_print_syslog函数打印。

5. UF_print_syslog可以更好地控制日志信息的打印，比如设置不同的日志级别。

6. 使用UF_print_syslog函数可以更方便地管理和查看系统日志信息。

7. UF_print_syslog函数的使用可以更好地满足系统日志管理的需求。

综上所述，UF_print_syslog是V18版本新增的打印系统日志信息的功能，提供了更好的日志管理方式。 */  
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
    static tag_t ask_next_feature_of_type_prefix(tag_t part, char *type_prefix,  
        tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (strstr(this_type, type_prefix) == this_type)  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
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
    static void do_it(void)  
    {  
        tag_t  
            feat = NULL_TAG,  
            part = UF_ASSEM_ask_work_part();  
        while ((feat = ask_next_feature_of_type_prefix(part, "SPOT_WELD", feat))  
            != NULL_TAG) report_feature_name_and_type("feat", feat);  
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

> 根据代码内容，这是一段基于NX的二次开发代码，主要功能是遍历当前工作部件中的所有焊点特征，并打印出每个焊点特征的名字和类型。具体代码介绍如下：
>
> 1. 头文件包含：引入了必要的NX API头文件，如uf.h, uf_ui.h等。
> 2. 错误报告函数：定义了report_error函数，用于打印错误信息。
> 3. 特征遍历函数：ask_next_feature_of_type_prefix用于遍历部件中指定类型前缀的特征。
> 4. 特征信息打印函数：report_feature_name_and_type用于打印特征的名字和类型。
> 5. 主体函数：do_it函数用于获取当前工作部件，并循环遍历所有焊点特征，并打印出每个焊点的名字和类型。
> 6. ufusr函数：这是NX二次开发的标准入口函数，在这里调用do_it函数执行主体逻辑。
> 7. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，该代码通过遍历部件中的焊点特征，并打印出每个焊点特征的名字和类型，实现了对焊点特征的查询功能。
>
