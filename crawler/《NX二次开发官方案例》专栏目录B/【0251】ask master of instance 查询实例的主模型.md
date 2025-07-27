### 【0251】ask master of instance 查询实例的主模型

#### 代码

```cpp
    static tag_t ask_master_of_instance(tag_t instance)  
    {  
        int  
            n_feats,  
            n_sets;  
        tag_t  
            *feats,  
            master = NULL_TAG,  
            *sets;  
        char  
            *type;  
        UF_CALL(UF_MODL_ask_feat_type(instance, &type));  
        if (!strcmp(type, "INSTNACE_SET") || !strcmp(type, "INSTANCE_SET"))  
        {  
            UF_CALL(UF_MODL_ask_sets_of_member(instance, &sets, &n_sets));  
            if (n_sets > 0)  
            {  
                UF_CALL(UF_MODL_ask_all_members_of_set(sets[0], &feats, &n_feats));  
                if (n_feats > 0)  
                {  
                    master = feats[0];  
                    UF_free(feats);  
                }  
                UF_free(sets);  
            }  
        }  
        if (!strcmp(type, "INSTANCE"))  
            UF_CALL(UF_MODL_ask_master(instance, &master));  
        UF_free(type);  
        return (master);  
    }

```

#### 代码解析

> 这段代码用于获取NX模型中实例的Master组件。
>
> 主要步骤如下：
>
> 1. 获取实例的组件类型，如果是实例集(INSTANCE_SET)，则获取该实例集中的所有组件。
> 2. 如果实例集中有组件，则返回第一个组件作为Master。
> 3. 如果实例的组件类型是实例(INSTANCE)，则直接获取该实例的Master组件。
> 4. 返回获取到的Master组件。
>
> 该代码通过NX的UF_MODL函数获取实例的组件类型、实例集中的组件以及Master组件。通过判断实例类型，采用不同的方法获取Master组件，实现了获取实例的Master组件的功能。
>
