### 【0222】ask features shown in part navigator 在部件导航器中询问显示的功能

#### 代码

```cpp
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

```

#### 代码解析

> 这段NX二次开发代码实现了以下功能：
>
> 1. 检查NX知识融合(Knowledge Fusion)是否已初始化，如果没有初始化，则初始化工作零件。
> 2. 调用函数uc4577生成一个唯一的字符串，用于创建知识融合规则时使用。
> 3. 创建一个知识融合规则，该规则会调用函数mqc_askBrowseableFeatures()来获取可浏览特征，并将结果存储在列表中。该规则以生成的唯一字符串为前缀。
> 4. 评估创建的规则，获取特征列表的值。
> 5. 删除创建的规则实例，避免重复。
> 6. 获取特征列表，并计算列表中的特征数量。
> 7. 根据特征数量分配内存，用于存储特征数组。
> 8. 遍历特征列表，获取每个特征，并将其存储在特征数组中。
> 9. 返回特征数量。
>
> 该代码的主要目的是通过知识融合规则调用函数获取NX中的可浏览特征，并返回特征数组及其数量。它利用了知识融合提供的规则和列表机制来管理特征数据。
>
