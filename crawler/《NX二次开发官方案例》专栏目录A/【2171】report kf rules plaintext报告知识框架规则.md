### 【2171】report kf rules plaintext报告知识框架规则

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void report_kf_rules(char *name_chain)  
    {  
        int  
            ii,  
            n_rules;  
        char  
            child_name[UF_UI_MAX_STRING_LEN],  
            *class_name,  
            *rule_type,  
            **rules,  
            *value;  
        UF_KF_rule_p_t  
            rule;  
        UF_CALL(UF_KF_ask_rules(name_chain, &n_rules, &rules));  
        for (ii = 0; ii < n_rules; ii++)  
        {  
            UF_CALL(UF_KF_ask_rule_of_instance(name_chain, rules[ii], &rule));  
            UF_CALL(UF_KF_ask_rule_type(rule, &rule_type));  
            WRITE("(");  
            WRITE(rule_type);  
            WRITE(") ");  
            sprintf(child_name, "%s%s:", name_chain, rules[ii]);  
            WRITE(child_name);  
            WRITE(" ");  
            UF_CALL(UF_KF_evaluate_rule_to_string(child_name, &value));  
            if (!strcmp(rule_type, "Instance"))  
            {  
                UF_CALL(UF_KF_ask_instance_class(child_name, &class_name));  
                WRITE("(");  
                WRITE(class_name);  
                WRITE(")\n");  
                report_kf_rules(child_name);  
            }  
            else  
            {  
                WRITE(value);  
                WRITE("\n");  
            }  
            UF_free(rule_type);  
            UF_free(value);  
        }  
        if (n_rules > 0) UF_free_string_array(n_rules, rules);  
    }

```

#### 代码解析

> 根据代码，这是一个用于报告NX CAD系统中知识框架(Knowledge Framework)规则的函数。
>
> 主要功能包括：
>
> 1. 调用UF_KF_ask_rules函数，获取指定name_chain的规则列表和数量。
> 2. 遍历规则列表，对每个规则执行以下操作：a. 调用UF_KF_ask_rule_of_instance获取规则实例。b. 调用UF_KF_ask_rule_type获取规则类型。c. 将规则类型和名称输出到列表窗口。d. 调用UF_KF_evaluate_rule_to_string获取规则值，并输出到列表窗口。e. 如果规则类型为Instance，则获取实例的类名，并递归调用report_kf_rules函数。
> 3. 释放内存。
>
> 该函数通过递归方式，可以输出指定name_chain及其子节点的所有知识框架规则，并以树形结构显示在列表窗口中。
>
