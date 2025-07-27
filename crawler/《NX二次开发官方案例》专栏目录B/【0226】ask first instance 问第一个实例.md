### 【0226】ask first instance 问第一个实例

#### 代码

```cpp
    static tag_t ask_first_instance(tag_t instance)  
    {  
        int  
            n_feats,  
            n_sets;  
        tag_t  
            *feats,  
            first = NULL_TAG,  
            iset,  
            *sets;  
        char  
            *type;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_ask_feat_type(instance, &type));  
        if (!strcmp(type, "INSTNACE_SET") || !strcmp(type, "INSTANCE_SET"))  
        {  
            UF_CALL(UF_MODL_ask_sets_of_member(instance, &sets, &n_sets));  
            if (n_sets > 0)  
            {  
                UF_CALL(UF_MODL_ask_all_members_of_set(sets[0], &feats, &n_feats));  
                if (n_feats > 0)  
                {  
                    first = feats[0];  
                    UF_free(feats);  
                }  
                UF_free(sets);  
            }  
        }  
        if (!strcmp(type, "INSTANCE"))  
        {  
            UF_CALL(UF_MODL_ask_instance_iset(instance, &iset));  
            UF_CALL(UF_MODL_ask_instance(iset, &feat_list));  
            UF_CALL(UF_MODL_ask_list_item(feat_list, 0, &first));  
            UF_CALL(UF_MODL_delete_list(&feat_list));  
        }  
        UF_free(type);  
        return (first);  
    }

```

#### 代码解析

> 这段NX二次开发代码的目的是获取一个实例的第一个实例成员。
>
> 主要步骤包括：
>
> 1. 获取实例的类型，判断是实例集还是单个实例。
> 2. 如果是实例集，获取该实例集的所有成员，并取第一个成员作为结果。
> 3. 如果是单个实例，获取该实例所在的实例集，然后获取实例集的第一个成员作为结果。
> 4. 最后返回获取到的第一个成员实例。
>
> 通过判断实例类型，并获取实例集或单个实例的第一个成员，该代码实现了获取实例的第一个实例成员的功能。
>
