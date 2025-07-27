### 【0307】ask next wave linked feature 询问下一个链接特征

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
    static tag_t ask_next_wave_linked_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (is_linked_feature(feat)) return feat;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于实现以下功能：
>
> 1. 判断一个特征是否为链接特征：通过调用UF_KF_is_initialized来判断知识框架是否已初始化。如果未初始化，则调用UF_KF_init_part来初始化当前工作部件的知识框架。使用uc4577函数生成一个唯一的规则名称。创建一个布尔类型的规则，规则内容为调用mqc_isLinkedFeature函数来判断指定特征是否为链接特征。调用UF_KF_evaluate_rule_to_string来执行规则，并获取返回的字符串。判断返回的字符串是否为"TRUE"，如果是则表示该特征是链接特征。最后删除该临时规则，释放返回值字符串的内存。
> 2. 通过调用UF_KF_is_initialized来判断知识框架是否已初始化。
> 3. 如果未初始化，则调用UF_KF_init_part来初始化当前工作部件的知识框架。
> 4. 使用uc4577函数生成一个唯一的规则名称。
> 5. 创建一个布尔类型的规则，规则内容为调用mqc_isLinkedFeature函数来判断指定特征是否为链接特征。
> 6. 调用UF_KF_evaluate_rule_to_string来执行规则，并获取返回的字符串。
> 7. 判断返回的字符串是否为"TRUE"，如果是则表示该特征是链接特征。
> 8. 最后删除该临时规则，释放返回值字符串的内存。
> 9. 询问下一个链接特征：使用UF_OBJ_cycle_objs_in_part函数遍历当前部件中的所有特征。如果遍历到的特征不是空特征，则调用is_linked_feature函数来判断它是否为链接特征。如果是链接特征，则返回该特征。如果遍历结束仍未找到链接特征，则返回空特征。
> 10. 使用UF_OBJ_cycle_objs_in_part函数遍历当前部件中的所有特征。
> 11. 如果遍历到的特征不是空特征，则调用is_linked_feature函数来判断它是否为链接特征。
> 12. 如果是链接特征，则返回该特征。
> 13. 如果遍历结束仍未找到链接特征，则返回空特征。
>
> 这段代码通过NX提供的知识框架机制，实现了一个自定义的规则来识别链接特征，并在部件中遍历查找下一个链接特征。
>
