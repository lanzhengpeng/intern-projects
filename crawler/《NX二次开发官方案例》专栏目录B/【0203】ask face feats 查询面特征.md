### 【0203】ask face feats 查询面特征

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
    static int ask_face_feats(tag_t face, tag_t **feats)  
    {  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_ask_face_feats(face, &feat_list));  
        return make_an_array(&feat_list, feats);  
    }

```

#### 代码解析

> 这段NX二次开发代码包含以下几个关键部分：
>
> 1. allocate_memory函数：用于分配指定大小的内存，并返回内存指针。它接收需要分配的内存字节数和返回的指针地址。
> 2. make_an_array函数：用于创建一个tag_t类型的数组，从NX对象列表中获取对象并赋值给数组元素。首先获取对象列表的长度，然后分配相应大小的内存，遍历对象列表，将对象ID赋值给数组，最后释放对象列表。
> 3. ask_face_feats函数：用于获取NX面特征列表，并转换为tag_t数组。首先获取面特征列表，然后调用make_an_array函数将其转换为数组。
>
> 这些函数通过NX的UF_MODL API获取模型信息，并使用动态内存分配来处理NX对象列表。整体来说，这段代码实现了NX模型信息的查询和转换功能。
>
