### 【0201】ask face edges 询问面的边界

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
static int ask_face_edges(tag_t face, tag_t **face_edges)
{
    uf_list_p_t
        edge_list;
    UF_CALL(UF_MODL_ask_face_edges(face, &edge_list));
    return make_an_array(&edge_list, face_edges);
}

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要用于处理NX模型中的面和边。代码包含三个函数：
>
> 1. allocate_memory：用于分配指定大小的内存，并返回内存地址。
> 2. make_an_array：从NX提供的列表中提取元素，生成一个tag_t数组，并释放NX提供的列表。
> 3. ask_face_edges：获取指定面的所有边，并返回一个包含所有边tag的数组。
>
> 具体步骤如下：
>
> 1. 使用allocate_memory函数分配指定大小的内存。
> 2. 使用make_an_array函数从NX提供的面边列表中提取所有边，并生成一个tag_t数组。
> 3. 释放NX提供的面边列表。
> 4. 返回生成的tag_t数组，该数组包含指定面的所有边的tag。
>
> 总体来说，这段代码实现了从NX模型中获取指定面的所有边，并将它们存储在一个数组中。
>
