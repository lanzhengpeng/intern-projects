### 【0123】ask body edges 查询体边缘

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
    static int ask_body_edges(tag_t body, tag_p_t *body_edges)  
    {  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_ask_body_edges(body, &edge_list));  
        return make_an_array(&edge_list, body_edges);  
    }

```

#### 代码解析

> 这段代码是NX二次开发的一部分，主要包含以下几个关键函数：
>
> 1. allocate_memory()：该函数用于分配指定大小的内存空间，并返回分配的内存地址。参数包括要分配的字节数nbytes和用于接收内存地址的指针where。
> 2. make_an_array()：该函数用于创建一个数组，用于存储从NX对象列表中提取的标签。它首先获取列表中的对象数量，然后分配相应大小的内存，并将对象标签存入数组中。最后，它会删除NX对象列表。
> 3. ask_body_edges()：该函数用于查询一个NX体(body)的所有边(edges)，并创建一个包含所有边标签的数组。首先，它会调用NX API获取边的列表，然后调用make_an_array()函数创建数组。
>
> 总的来说，这段代码的目的是实现从NX模型中提取体对象的所有边，并将这些边的标签存储在一个数组中。这为后续的几何分析或处理提供了基础。
>
