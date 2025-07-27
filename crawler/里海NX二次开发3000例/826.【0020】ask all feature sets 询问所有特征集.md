### 【0020】ask all feature sets 询问所有特征集

#### 代码

```cpp
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_feature_sets(tag_t part, tag_p_t *feats)  
    {  
        int  
            ii,  
            is_member,  
            n_sets;  
        tag_t  
            feat = NULL_TAG,  
            *sets;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_is_feature_a_set_member(feat, &is_member));  
            if (is_member)  
            {  
                UF_CALL(UF_MODL_ask_sets_of_member(feat, &sets, &n_sets));  
                for (ii = 0; ii < n_sets; ii++)  
                    UF_CALL(UF_MODL_put_list_item(feat_list, sets[ii]));  
                if (n_sets > 0) UF_free(sets);  
            }  
        }  
        return (make_an_array(&feat_list, feats));  
    }

```

#### 代码解析

> 这段代码是用于NX CAD软件的二次开发代码，其主要功能是从NX部件中提取所有特征集（Feature Set）。
>
> 主要函数及其功能如下：
>
> 1. ask_next_feature(part, feat)：递归遍历指定部件中的所有特征，返回下一个存活状态的特征。
> 2. allocate_memory(nbytes, where)：分配指定大小的内存空间，并将内存地址赋值给where指针。
> 3. make_an_array(object_list, objects)：将UF列表转换为tag_t数组，并释放列表内存。
> 4. ask_all_feature_sets(part, feats)：提取指定部件中所有特征集，并返回特征集的tag_t数组。
>
> 主要流程包括：
>
> 1. 遍历部件中的所有特征，检查是否为特征集成员。
> 2. 如果是特征集成员，则获取其所属的特征集。
> 3. 将所有特征集添加到列表中。
> 4. 将特征集列表转换为数组并返回。
> 5. 释放临时列表的内存。
>
> 该代码实现了NX部件中特征集的查询和提取功能，对于特征集的管理和分析具有重要意义。
>
