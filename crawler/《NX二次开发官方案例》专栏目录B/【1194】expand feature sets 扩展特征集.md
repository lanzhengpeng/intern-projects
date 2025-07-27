### 【1194】expand feature sets 扩展特征集

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
    static int expand_feature_sets(int n_feats, tag_t **feats)  
    {  
        int  
            ii;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        for (ii = 0; ii < n_feats; ii++)  
            add_members_to_list((*feats)[ii], feat_list);  
        UF_free(*feats);  
        return (make_an_array(&feat_list, feats));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，主要用于NX的建模和特征操作。具体来说，它实现了以下功能：
>
> 1. allocate_memory函数：用于分配内存空间。它接受要分配的字节数nbytes和指向内存地址的指针where，使用UF_allocate_memory API来分配内存，并返回成功或失败的响应码。
> 2. make_an_array函数：用于将特征链表转换成特征数组。它接受特征链表指针object_list和指向特征数组指针的指针objects，首先获取链表中的特征数量，然后为特征数组分配内存，接着遍历链表，将每个特征ID复制到数组中，最后删除链表并返回特征数量。
> 3. expand_feature_sets函数：用于扩展特征集。它接受特征数量n_feats和指向特征数组指针的指针feats。首先创建一个空链表feat_list，然后遍历特征数组，将每个特征添加到链表中。接着释放特征数组内存，最后调用make_an_array函数将链表转换成新的特征数组，并返回特征数量。
>
> 总体来说，这段代码实现了特征链表到特征数组的转换，以及特征集的扩展，为NX建模和特征操作提供了基础支持。
>
