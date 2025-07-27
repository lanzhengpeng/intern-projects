### 【0744】create kf child rule and find new feature tag 创建KF子规则并找到新特征标签

#### 代码

```cpp
    /*HEAD CREATE_KF_CHILD_RULE_AND_FIND_NEW_FEATURE_TAG CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中新增的函数。

UF_print_syslog是V18版本中新增的函数。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_FNT(X) report_feature_name_and_type(#X, X)  
    static void report_feature_name_and_type(char *which, tag_t feature)  
    {  
        char  
            *name,  
            *type,  
            msg[MAX_LINE_LENGTH+1];  
        if (!UF_CALL(UF_MODL_ask_feat_name(feature, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feature, &type)))  
        {  
            sprintf(msg, "%s - tag_t %d - %s (%s)\n", which, feature, name, type);  
            WRITE(msg);  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            ug_object;  
        char  
            *generated_name,  
            *parameter_names[3] = { "Length", "Width", "Height" },  
            *parameter_rules[3] = { "1.0", "2.0", "3.0" };  
        UF_KF_instance_p_t  
            instance;  
        UF_KF_value_p_t  
            value;  
        UF_CALL(UF_KF_init_part(part));  
        UF_CALL(UF_KF_create_child_rule(part, "root:", "cube", "ug_block",  
            3, parameter_names, parameter_rules, &generated_name));  
        if (generated_name != NULL) UF_free(generated_name);  
        UF_CALL(UF_MODL_update());  
        UF_CALL(UF_VIEW_fit_view(NULL_TAG, 1.0));  
        UF_CALL(UF_KF_evaluate_rule("root:cube:", &value));  
        UF_CALL(UF_KF_ask_instance(value, &instance));  
        UF_CALL(UF_KF_ask_ug_object(instance, &ug_object));  
        UF_CALL(UF_KF_free_rule_value(value));  
        WRITE_FNT(ug_object);  
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

> 这段代码是用于NX CAD软件的二次开发，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个输出特征名和类型信息的函数report_feature_name_and_type。
> 3. 在do_it函数中，首先获取当前显示的零件，并初始化知识融合（KF）模块。
> 4. 创建一个名为"cube"的子规则，该规则引用了一个名为"ug_block"的UG对象，并指定了3个参数及其规则值。
> 5. 更新模型并适应视图。
> 6. 评估"cube"规则，获取其实例和UG对象。
> 7. 输出UG对象的名称和类型。
> 8. 定义了ufusr函数作为二次开发程序的入口点，在函数中初始化NX、调用do_it函数实现主要功能，并终止NX。
> 9. 定义了ufusr_ask_unload函数，用于指定程序卸载方式。
>
> 总的来说，这段代码实现了创建一个名为"cube"的子规则，并生成了一个与之关联的UG对象，最后输出了该UG对象的信息。这段代码可作为NX二次开发的知识融合（KF）模块的一个示例。
>
