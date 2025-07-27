### 【0022】ask all features 询问所有特征

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
    static int ask_all_features(tag_t part, tag_p_t *feats)  
    {  
        tag_t  
            feat = NULL_TAG;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(feat_list, feat));  
        return (make_an_array(&feat_list, feats));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要实现了以下功能：
>
> 1. ask_next_feature函数：用于遍历指定部件中的所有特征，并返回下一个激活的特征。通过循环调用UF_OBJ_cycle_objs_in_part函数，直到找到下一个激活的特征并返回。
> 2. allocate_memory函数：用于分配指定大小的内存空间，并将内存地址存入where指针指向的指针变量。
> 3. make_an_array函数：用于将对象列表转换成对象数组。通过遍历列表，将每个对象ID存入数组，然后删除列表，返回数组长度。
> 4. ask_all_features函数：用于获取指定部件中的所有激活特征，并将特征ID存储在数组中返回。首先创建一个特征列表，然后逐个调用ask_next_feature获取特征，存入列表，最后调用make_an_array将列表转换成特征数组并返回。
>
> 通过以上函数的组合，实现了遍历部件中的所有激活特征，并以数组的形式返回这些特征的功能。这样的设计可以方便后续代码对特征进行批量处理。
>
