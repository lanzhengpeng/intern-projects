### 【0009】ask all assembly piece parts 询问所有装配件的零件

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
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    static int ask_all_assembly_piece_parts(tag_t part, tag_t **parts)  
    {  
        tag_t  
            comp = NULL_TAG,  
            comp_part;  
        uf_list_p_t  
            part_list;  
        UF_CALL(UF_MODL_create_list(&part_list));  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            comp_part = UF_ASSEM_ask_prototype_of_occ(comp);  
            if ((UF_ASSEM_ask_root_part_occ(comp_part)) == NULL_TAG)  
                UF_CALL(UF_MODL_put_list_item(part_list, comp_part));  
        }  
        return make_an_array(&part_list, parts);  
    }

```

#### 代码解析

> 这段NX二次开发代码实现了以下功能：
>
> 1. 内存分配：通过allocate_memory函数实现内存的动态分配，用于后续创建数组。
> 2. 创建数组：通过make_an_array函数，根据传入的对象列表创建一个tag_t类型的数组，并按列表顺序存储对象的tag。
> 3. 遍历组件：ask_next_component函数用于遍历指定组件的下一个组件，并判断其是否为组件实例，如果是则返回。
> 4. 查询所有组件零件：ask_all_assembly_piece_parts函数首先获取指定组件的所有组件实例，然后判断实例是否为根组件，如果是，则将其加入列表，最后调用make_an_array将列表中的零件tag存入数组并返回数组长度。
> 5. 综合功能：通过以上函数的组合，实现了查询组件实例中的所有根组件零件的功能。
>
> 该代码段实现了NX的组件遍历、内存分配以及数组创建等常见功能，为后续的二次开发提供了基础。
>
