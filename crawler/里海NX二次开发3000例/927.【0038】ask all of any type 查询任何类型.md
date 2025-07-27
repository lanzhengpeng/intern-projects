### 【0038】ask all of any type 查询任何类型

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
// This is different from "ask_all_of_type" in that UF_OBJ_cycle_all will 
// find types such as UF_scalar_type which UF_OBJ_cycle_objs_in_part will not.
static int ask_all_of_any_type(tag_t part, int the_type, tag_t **objects)
{
    int
        subtype,
        type;
    tag_t
        object = NULL_TAG;
    uf_list_p_t
        object_list;
    UF_CALL(UF_MODL_create_list(&object_list));
    while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)
    {
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));
        if (type == the_type)
            UF_CALL(UF_MODL_put_list_item(object_list, object));
    }
    return (make_an_array(&object_list, objects));
}

```

#### 代码解析

> 这段代码是用于NX Open C++ API的二次开发代码，主要实现了以下几个功能：
>
> 1. 内存分配函数：allocate_memory用于在NX中分配指定大小的内存，并返回内存指针。
> 2. 创建数组函数：make_an_array从NX列表创建一个数组，它首先获取列表中元素的个数，然后分配相应大小的内存，并将列表中的元素复制到数组中，最后删除NX列表。
> 3. 查询类型函数：ask_all_of_any_type用于查询NX部件中所有指定类型的对象。它使用UF_OBJ_cycle_all遍历部件中的所有对象，判断每个对象的类型是否与指定类型相同，如果相同则将其添加到NX列表中。最后调用make_an_array将列表转换为数组返回。
>
> 总体来说，这段代码实现了在NX中创建数组、查询对象类型以及遍历部件中的所有对象等基本功能。这些函数在NX二次开发中是非常实用的。
>
