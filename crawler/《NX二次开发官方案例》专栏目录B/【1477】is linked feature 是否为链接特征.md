### 【1477】is linked feature 是否为链接特征

#### 代码

```cpp
    //  See ER 6669518  
    #include <uf_kf.h>  
    static logical is_linked_feature(tag_t feature)  
    {  
        logical  
            is_init = false,  
            is_linked = false;  
        char  
            rule[133],  
            unique[UF_CFI_MAX_FILE_NAME_SIZE],  
            *value;  
        UF_CALL(UF_KF_is_initialized(&is_init));  
        if (!is_init)  
            UF_CALL(UF_KF_init_part(UF_ASSEM_ask_work_part()));  
        UF_CALL(uc4577(unique));  
        sprintf(rule, "mqc_isLinkedFeature(%d)", feature);  
        UF_CALL(UF_KF_create_rule_no_update("root:", unique, "Boolean", rule, ""));  
        sprintf(rule, "root:%s:", unique);  
        UF_CALL(UF_KF_evaluate_rule_to_string(rule, &value));  
        UF_CALL(UF_KF_delete_instance_rule("root:", unique));  
        if (strcmp(value, "TRUE") == 0) is_linked = true;  
        UF_free(value);  
        return is_linked;  
    }

```

#### 代码解析

> 这段NX Open C++代码实现了以下功能：
>
> 1. 定义了一个名为is_linked_feature的函数，用于判断传入的特征是否为关联特征。
> 2. 函数首先使用UF_KF_is_initialized判断知识库是否已初始化。如果没有初始化，则调用UF_KF_init_part进行初始化。
> 3. 调用uc4577函数生成一个唯一的规则名，用于后续创建规则。
> 4. 使用sprintf构造一个布尔型规则的字符串，并调用UF_KF_create_rule_no_update在知识库中创建这个规则。
> 5. 再次使用sprintf构造规则的路径字符串，并调用UF_KF_evaluate_rule_to_string来评估规则，获取其值。
> 6. 判断规则值是否为"TRUE"，如果是，则表示特征是关联特征，返回true。
> 7. 最后，删除刚才创建的规则，释放分配的内存。
> 8. 函数返回布尔值，表示特征是否为关联特征。
>
> 总的来说，这段代码通过知识库规则机制，实现了在NX中判断特征是否为关联特征的功能。其优点在于，规则可以动态创建和删除，便于管理。
>
