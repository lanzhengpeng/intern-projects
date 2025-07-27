### 【0012】ask all condemned edges 询问所有废弃的边

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
static int ask_all_condemned_objects_of_type_and_subtype(tag_t part, int type,
    int subtype, tag_t **objects)
{
    int
        this_type,
        this_subtype;
    tag_t
        object = NULL_TAG;
    uf_list_p_t
        object_list;
    UF_CALL(UF_MODL_create_list(&object_list));
    while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)
    {
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));
        if ((this_type == type) && (this_subtype == subtype) &&
            (UF_OBJ_ask_status(object) == UF_OBJ_CONDEMNED))
                UF_MODL_put_list_item(object_list, object);
    }
    return (make_an_array(&object_list, objects));
}
static int ask_all_condemned_edges(tag_t part, tag_t **edges)
{
    return ask_all_condemned_objects_of_type_and_subtype(part, UF_solid_type,
        UF_solid_edge_subtype, edges);
}

```

#### 代码解析

> 这段代码是NX Open C++ API编写的二次开发代码，主要功能是遍历指定部件中的所有被删除的边。
>
> 主要函数包括：
>
> 1. allocate_memory()：用于分配指定大小的内存。
> 2. make_an_array()：将列表转换成数组。
> 3. ask_all_condemned_objects_of_type_and_subtype()：获取指定类型和子类型的被删除对象列表。
> 4. ask_all_condemned_edges()：获取指定部件中所有被删除的边列表。
>
> 具体流程是：
>
> 1. 调用UF_MODL_create_list()创建一个空列表。
> 2. 遍历指定部件中的所有对象，检查类型、子类型和状态，如果是被删除的边，则加入列表。
> 3. 调用make_an_array()将列表转换成数组并返回。
> 4. 调用UF_MODL_delete_list()删除列表。
> 5. 最后ask_all_condemned_edges()函数返回被删除边的数组。
>
> 这段代码实现了NX部件中特定类型对象的遍历和筛选功能，可以基于此进行二次开发，以满足特定需求。
>
