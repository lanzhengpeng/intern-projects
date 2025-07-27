### 【0122】ask body edges of name 询问名称的实体边界

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
static tag_t cycle_by_name_type_and_subtype(tag_t part, char *name, int type,
    int subtype, tag_t object)
{
    int
        this_type,
        this_subtype;
    while (!UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, type, TRUE,
        &object)) && (object != NULL_TAG))
    {
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));
        if (this_subtype == subtype) return object;
    }
    return NULL_TAG;
}
static int ask_body_edges_of_name(tag_t body, char *name, tag_p_t *body_edges)
{
    tag_t
        edge = NULL_TAG,
        part;
    uf_list_p_t
        edge_list;
    UF_CALL(UF_OBJ_ask_owning_part(body, &part));
    UF_CALL(UF_MODL_create_list(&edge_list));
    while ((edge = cycle_by_name_type_and_subtype(part, name,
        UF_solid_type, UF_solid_edge_subtype, edge)) != NULL_TAG)
        UF_CALL(UF_MODL_put_list_item(edge_list, edge));
    return make_an_array(&edge_list, body_edges);
}

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，主要实现了以下几个功能：
>
> 1. 内存分配：allocate_memory函数使用NX提供的UF_allocate_memory接口，根据指定的大小分配内存，并返回分配的内存指针。
> 2. 创建数组：make_an_array函数从NX对象列表中创建一个数组，首先调用UF_MODL_ask_list_count获取列表中的对象数量，然后使用allocate_memory函数为该数量分配内存，并将列表中的对象ID放入数组中，最后删除NX对象列表。
> 3. 根据名称、类型和子类型遍历对象：cycle_by_name_type_and_subtype函数用于在指定部件下，根据名称、类型和子类型遍历对象，找到匹配的对象并返回。
> 4. 查询体的边：ask_body_edges_of_name函数首先获取体的拥有部件，然后在部件下根据名称遍历边，并将遍历到的边放入NX列表中，最后调用make_an_array函数将列表中的边ID放入数组中。
>
> 综合来看，这段代码通过NX Open C++ API实现了从NX模型中查询特定名称的边，并将其放入数组中的功能。
>
