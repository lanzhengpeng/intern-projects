### 【0011】ask all components 获取所有组件

#### 代码

```cpp
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
    static int ask_all_components(tag_t part, tag_t **components)  
    {  
        tag_t  
            component = NULL_TAG;  
        uf_list_p_t  
            comp_list;  
        UF_CALL(UF_MODL_create_list(&comp_list));  
        while ((component = ask_next_component(part, component)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(comp_list, component));  
        return make_an_array(&comp_list, components);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要实现了以下功能：
>
> 1. ask_next_component()函数：用于遍历零件中的所有组件，并返回下一个组件的tag。它使用UF_OBJ_cycle_objs_in_part()函数遍历零件中的所有组件，并检查组件的类型和子类型，如果组件是零件实例，则返回该组件的tag。
> 2. allocate_memory()函数：用于申请指定大小的内存，并返回指向该内存的指针。它使用UF_allocate_memory()函数申请内存，并设置返回值。
> 3. make_an_array()函数：用于将列表中的所有元素转换为数组。它先获取列表中的元素数量，然后申请相应大小的内存来存储数组，接着遍历列表，将元素添加到数组中，最后删除列表。
> 4. ask_all_components()函数：用于获取零件中的所有组件，并以数组形式返回。它首先创建一个列表，然后调用ask_next_component()函数遍历零件中的所有组件，并将组件添加到列表中。最后调用make_an_array()函数将列表转换为数组。
>
> 综合来看，这段代码通过UF_MODL、UF_OBJ等模块的函数，实现了NX零件组件的遍历和数组化处理，为后续的二次开发提供了基础支持。
>
