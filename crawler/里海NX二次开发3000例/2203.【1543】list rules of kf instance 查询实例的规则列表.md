### 【1543】list rules of kf instance 查询实例的规则列表

#### 代码

```cpp
    /*HEAD LIST_RULES_OF_KF_INSTANCE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的功能。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_rules;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            name_chain[133] = { "root:" },  
            *rule_type,  
            **rules;  
        UF_KF_rule_p_t  
            rule;  
        UF_CALL(UF_KF_init_part(part));  
        while (prompt_for_text("Enter name chain of instance", name_chain))  
        {  
            WRITE_S(name_chain);  
            UF_CALL(UF_KF_ask_rules(name_chain, &n_rules, &rules));  
            WRITE_D(n_rules);  
            for (ii = 0; ii < n_rules; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_S(rules[ii]);  
                UF_CALL(UF_KF_ask_rule_of_instance(name_chain, rules[ii], &rule));  
                UF_CALL(UF_KF_ask_rule_type(rule, &rule_type));  
                WRITE_S(rule_type);  
                UF_free(rule_type);  
            /*  Do NOT free the rule - See PR 4356459  
                UF_free(rule);  
            */  
            }  
            if (n_rules > 0) UF_free_string_array(n_rules, rules);  
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

> 这段NX二次开发代码的主要功能是获取指定实例的规则信息并显示在日志窗口中。
>
> 代码主要分为以下几个部分：
>
> 1. 引入NX提供的库文件，包括UF、UF_UI、UF_KF、UF_PART等。
> 2. 定义了report_error函数，用于在出错时打印错误信息。
> 3. 定义了prompt_for_text函数，用于在日志窗口中获取用户输入的实例名称链。
> 4. 定义了WRITE_D和WRITE_S宏，用于在日志窗口中打印整数和字符串。
> 5. 定义了do_it函数，这是代码的核心功能：获取当前显示的零件。循环获取用户输入的实例名称链，并调用UF_KF_ask_rules获取该实例的规则列表。遍历规则列表，调用UF_KF_ask_rule_of_instance获取每个规则的详细信息。打印规则类型到日志窗口。
> 6. 获取当前显示的零件。
> 7. 循环获取用户输入的实例名称链，并调用UF_KF_ask_rules获取该实例的规则列表。
> 8. 遍历规则列表，调用UF_KF_ask_rule_of_instance获取每个规则的详细信息。
> 9. 打印规则类型到日志窗口。
> 10. 定义了ufusr函数，这是NX二次开发的入口函数，在这里调用do_it函数执行主要功能。
> 11. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了在NX中获取指定实例的规则信息并显示的功能，利用了NX提供的API进行二次开发。
>
