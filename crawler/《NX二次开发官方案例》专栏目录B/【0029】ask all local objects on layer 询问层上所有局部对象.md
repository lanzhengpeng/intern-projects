### 【0029】ask all local objects on layer 询问层上所有局部对象

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
static logical is_immediate_child_component(tag_t component)
{
    tag_t
        inst = UF_ASSEM_ask_inst_of_part_occ(component),
        parent = UF_ASSEM_ask_parent_of_instance(inst),
        part = UF_ASSEM_ask_work_part();
    if (parent == part) return TRUE;
    return FALSE;
}
static int ask_all_local_objects_on_layer(int layer, tag_t **objects)
{
    int
        subtype,
        type;
    tag_t
        object = NULL_TAG;
    uf_list_p_t
        list;
    UF_CALL(UF_MODL_create_list(&list));
    while (!UF_CALL(UF_LAYER_cycle_by_layer(layer, &object)) &&
        (object != NULL_TAG))
    {
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));
        if ((type == UF_component_type && is_immediate_child_component(object))
            || !UF_ASSEM_is_occurrence(object))
            UF_CALL(UF_MODL_put_list_item(list, object));
    }
    return make_an_array(&list, objects);
}

```

#### 代码解析

> 这段NX二次开发代码的主要功能是查询指定图层上的所有本地对象，并将这些对象保存到一个数组中。具体来说，代码做了以下工作：
>
> 1. 内存分配函数：allocate_memory用于动态分配内存，接收要分配的字节数和指向分配内存地址的指针，然后调用UF函数分配内存并返回状态。
> 2. 创建数组函数：make_an_array用于从UF列表中创建一个NX对象数组，接收列表和对象数组指针作为参数，获取列表中的对象数量，为对象数组分配内存，遍历列表将对象ID赋值给数组，最后删除列表。
> 3. 判断组件函数：is_immediate_child_component用于判断一个组件是否为父部件的直接子组件，通过获取组件的实例、实例的父组件和当前工作部件来实现。
> 4. 查询图层对象函数：ask_all_local_objects_on_layer用于查询指定图层上的所有本地对象，接收图层号和对象数组指针作为参数，创建UF列表，遍历图层上的对象，如果是直接子组件或不是装配体，则将对象添加到列表中，最后调用make_an_array将列表中的对象保存到数组中。
>
> 总体来说，这段代码实现了在NX中查询指定图层上的本地对象并保存到数组的二次开发功能。
>
