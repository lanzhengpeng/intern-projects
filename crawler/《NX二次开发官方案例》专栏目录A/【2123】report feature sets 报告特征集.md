### 【2123】report feature sets 报告特征集

#### 代码

```cpp
    /*HEAD REPORT_FEATURE_SETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新增的，只回答翻译，不要添加其他内容。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == 3) return feat;  
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
            part = UF_PART_ask_display_part();  
        char  
            *feat_type;  
        WRITE_D(part);  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feat, &feat_type));  
            if (strstr(feat_type, "FSET") != NULL)  
                report_feature_name_and_type("feat", feat);  
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

> 这段代码是一个用于NX的二次开发示例，主要功能是遍历当前部件的所有特征集，并报告每个特征集的名称和类型。具体来说：
>
> 1. 定义了宏UF_CALL，用于调用NX的UF函数，并报告错误。
> 2. 定义了宏WRITE_D，用于向日志窗口输出整数值。
> 3. 定义了函数report_error，用于报告UF函数调用错误。
> 4. 定义了函数write_integer_to_listing_window，用于向日志窗口输出整数值。
> 5. 定义了函数ask_next_feature，用于遍历部件中的下一个特征。
> 6. 定义了函数report_feature_name_and_type，用于报告特征集的名称和类型。
> 7. 定义了函数do_it，用于遍历当前部件的所有特征集，并报告名称和类型。
> 8. 定义了ufusr函数，用于初始化NX，调用do_it，然后终止NX。
> 9. 定义了ufusr_ask_unload函数，用于卸载NX插件。
>
> 总体来说，这段代码通过NX的UF函数实现了遍历部件特征集并报告名称和类型的功能，是一个NX二次开发的示例。
>
