### 【0435】ask rules in kf class 询问KF类中的规则

#### 代码

```cpp
    /*HEAD ASK_RULES_OF_KF_CLASS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 中的新增功能，专门用于打印系统日志消息。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        char  
            *rule_type,  
            *rule_name,  
            msg[133],  
            kfclass[133];  
        int  
            ii,  
            n_rules;  
        UF_KF_rule_p_t   
            *rules;  
        while(prompt_for_text("Report rules of Class: ", kfclass))  
        {  
            UF_CALL(UF_KF_ask_rules_of_class( kfclass, &n_rules, &rules ));  
            sprintf(msg, "No. of Rules: %d\n", n_rules);  
            for (ii = 0; ii < n_rules; ii++)  
            {  
                WRITE_D(ii);  
                UF_KF_ask_rule_name( rules[ii], &rule_name );  
                WRITE_S(rule_name);  
                UF_CALL(UF_KF_ask_rule_type(rules[ii], &rule_type));  
                WRITE_S(rule_type);  
                UF_free(rule_type);  
            /*  Do NOT free the rule - See PR 4356459  
                UF_free(rule);  
            */  
            }  
            if (n_rules > 0) UF_free(rules);  
            uc1601(msg, TRUE);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是查询NX系统中的规则类，并报告每个规则类的名称和类型。代码的主要部分如下：
>
> 1. 头文件包含：包含了必要的NX API头文件。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于打印错误代码、错误消息和调用失败的函数。
> 3. 整数/字符串写入列表窗口：定义了宏WRITE_D和WRITE_S，用于将整数或字符串写入列表窗口。
> 4. 提示文本函数：定义了一个函数prompt_for_text，用于提示用户输入文本并返回布尔值。
> 5. 查询规则类：定义了一个函数do_it，用于查询指定类名的规则，并打印每个规则的名称和类型。
> 6. ufusr函数：定义了NX的入口函数ufusr，用于初始化NX API、执行查询操作并终止NX API。
> 7. 卸载请求：定义了卸载请求函数ufusr_ask_unload，返回立即卸载标志。
>
> 总体来说，这段代码通过NX的API实现了查询规则类及其规则名称和类型的功能，并提供了用户交互提示。代码结构清晰，注释详细，是一个很好的NX二次开发示例。
>
