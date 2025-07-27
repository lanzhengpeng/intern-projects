### 【1196】explode feature set 解散特征集合，即删除特征集合，并删除其中的所有特征

#### 代码

```cpp
    static void explode_feature_set(tag_t set)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            *feats;  
        uf_list_p_t  
            list;  
        if (!UF_MODL_ask_all_members_of_set(set, &feats, &n_feats))  
        {  
            UF_CALL(UF_MODL_edit_set_members(set, NULL, 0));  
            UF_CALL(UF_MODL_create_list(&list));  
            UF_CALL(UF_MODL_put_list_item(list, set));  
            UF_CALL(UF_MODL_delete_feature(list));  
            UF_CALL(UF_MODL_delete_list(&list));  
            for (ii = 0; ii < n_feats; ii++) explode_feature_set(feats[ii]);  
            if (n_feats > 0) UF_free(feats);  
        }  
    }

```

#### 代码解析

> 这段NX Open C++代码的功能是删除一个特征集合中的所有特征及其子特征集合，实现步骤如下：
>
> 1. 获取特征集合的所有成员特征，并判断其数量。如果数量为0，则直接删除该特征集合。
> 2. 如果数量大于0，则创建一个特征列表，将当前特征集合添加到列表中，然后调用UF_MODL_delete_feature删除列表中的所有特征。
> 3. 遍历所有成员特征，递归调用explode_feature_set函数，以删除每个成员特征的子特征集合。
> 4. 最后释放成员特征数组的空间。
>
> 该函数实现了对NX特征树结构的递归遍历和删除操作，能够有效删除一个特征及其所有子特征。
>
