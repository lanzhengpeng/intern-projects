### 【0112】ask all visible local objects 询问所有可见的本地对象

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
static int ask_all_visible_local_objects(tag_t **objects)
{
    int
        subtype,
        type;
    tag_t
        object = NULL_TAG,
        view;
    uf_list_p_t
        list;
    if (UF_CALL(UF_VIEW_ask_work_view(&view))) return 0;
    UF_CALL(UF_MODL_create_list(&list));
    while (!UF_CALL(UF_VIEW_cycle_objects(view, UF_VIEW_VISIBLE_OBJECTS,
        &object)) && (object != NULL_TAG))
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

> 这段代码是NX Open API的C++代码，用于获取当前工作视图中的所有可见本地组件或零件。
>
> 主要功能包括：
>
> 1. 内存分配：使用allocate_memory函数，用于为对象数组分配内存。
> 2. 对象数组创建：make_an_array函数，用于从列表创建对象数组。
> 3. 子组件判断：is_immediate_child_component函数，用于判断一个组件是否为直接子组件。
> 4. 获取所有可见本地对象：ask_all_visible_local_objects函数，用于获取当前工作视图中所有可见的本地组件或零件。它首先获取当前工作视图，然后遍历视图中的所有可见对象，将组件或零件添加到列表中。最后，调用make_an_array将列表转换为对象数组。
>
> 总体来说，这段代码通过遍历当前工作视图，获取所有可见的本地组件或零件，并以数组形式返回。这为后续操作提供了基础。
>
