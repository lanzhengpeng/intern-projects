### 【0051】add members to list 向列表中添加成员

#### 代码

```cpp
    static void add_members_to_list(tag_t feature, uf_list_p_t list)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            *feats;  
        if (!UF_MODL_ask_all_members_of_set(feature, &feats, &n_feats))  
        {  
            for (ii = 0; ii < n_feats; ii++) add_members_to_list(feats[ii], list);  
            if (n_feats > 0) UF_free(feats);  
        }  
        else UF_CALL(UF_MODL_put_list_item(list, feature));  
    }

```

#### 代码解析

> 这段代码用于递归地获取NX特征的所有成员并将其添加到列表中。
>
> 主要思路如下：
>
> 1. 调用UF_MODL_ask_all_members_of_set获取给定特征的所有成员。
> 2. 如果成员数量大于0，则递归调用add_members_to_list函数将每个成员添加到列表中。
> 3. 释放成员数组。
> 4. 如果没有成员，则直接将特征添加到列表中。
>
> 这段代码实现了递归地遍历特征树并获取所有成员的功能，从而将NX特征及其所有子特征添加到一个列表中。
>
