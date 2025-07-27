### 【0016】ask all drafting reference components 查询所有绘图参考组件

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
static int ask_all_drafting_reference_components(tag_t part, tag_t **children)
{
    logical
        is_ref;
    int
        ii,
        n;
    tag_t
        *comps;
    uf_list_p_t
        child_list;
    UF_CALL(UF_MODL_create_list(&child_list));
    n = UF_ASSEM_ask_all_part_occ_children(
        UF_ASSEM_ask_root_part_occ(part), &comps);
    for (ii = 0; ii < n; ii++)
    {
        UF_CALL(UF_DRAW_is_drafting_component(comps[ii], &is_ref));
        if (is_ref) UF_CALL(UF_MODL_put_list_item(child_list, comps[ii]));
    }
    return (make_an_array(&child_list, children));
}

```

#### 代码解析

> 这段代码是用于NX二次开发的示例，其主要功能是遍历装配体的所有零件，筛选出其中的参考组件，并返回这些组件的ID数组。
>
> 具体步骤如下：
>
> 1. 调用UF_MODL_create_list创建一个空列表child_list。
> 2. 调用UF_ASSEM_ask_all_part_occ_children获取装配体的根零件的所有子零件，并将这些子零件的ID存入comps数组。
> 3. 遍历comps数组，对每个子零件调用UF_DRAW_is_drafting_component判断是否为参考组件。如果是，则调用UF_MODL_put_list_item将其ID添加到child_list列表中。
> 4. 调用make_an_array函数将child_list列表转换成ID数组，并返回数组的大小。
> 5. make_an_array函数中，先调用UF_MODL_ask_list_count获取列表大小，然后分配内存，遍历列表将每个组件的ID复制到数组中，最后删除列表。
> 6. allocate_memory函数用于申请指定大小的内存并返回指针。
>
> 总的来说，这段代码实现了遍历装配体，筛选出参考组件，并返回这些组件ID数组的功能。通过示例展示了NX二次开发中常用的API调用流程。
>
