### 【0021】ask all features of type 询问所有特定类型的特征

#### 代码

```cpp
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
    static tag_t ask_next_feature_of_type(tag_t part, char *type, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (!strcmp(this_type, type))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    static int ask_all_features_of_type(tag_t part, char *type, tag_t **feats)  
    {  
        tag_t  
            feat = NULL_TAG;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        while ((feat = ask_next_feature_of_type(part, type, feat)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(feat_list, feat));  
        return (make_an_array(&feat_list, feats));  
    }

```

#### 代码解析

> 这段NX二次开发代码实现了以下功能：
>
> 1. allocate_memory函数：该函数用于动态分配指定大小的内存空间，并返回指向内存空间的指针。
> 2. make_an_array函数：该函数用于将NX对象列表转换成NX对象数组。首先，它获取对象列表的长度，然后动态分配一个大小为列表长度乘以对象大小的内存空间，并将对象列表中的对象依次放入该内存空间。最后，它删除对象列表，并返回数组长度。
> 3. ask_next_feature_of_type函数：该函数用于遍历NX部件中的特征，并返回下一个指定类型的特征。如果找到符合条件的特征，则返回特征ID；否则返回NULL_TAG。
> 4. ask_all_features_of_type函数：该函数用于获取NX部件中所有指定类型的特征。它首先创建一个特征列表，然后逐个获取特征，并将它们放入特征列表。最后，它调用make_an_array函数将特征列表转换为特征数组，并返回数组长度。
>
> 总体来说，这段代码提供了NX对象列表与数组之间的转换，以及遍历和查询NX部件中特定类型特征的功能。
>
