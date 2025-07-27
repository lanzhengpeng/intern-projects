### 【0236】ask hidden components 查询隐藏组件

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
    static logical isHidden(tag_t object)  
    {  
        logical  
            is_disp;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(object, &is_disp));  
        if (!is_disp) return TRUE;  // its not displayable so its not visible  
        UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
        if (disp_props.blank_status == UF_OBJ_BLANKED) return TRUE;  
        return FALSE;  
    }  
    static int ask_hidden_components(tag_t part, tag_t **hidden_components)  
    {  
        int  
            ii,  
            n_comps;  
        tag_t  
            *comps;  
        uf_list_p_t  
            hidden;  
        UF_CALL(UF_MODL_create_list(&hidden));  
        n_comps = ask_all_components(part, &comps);  
        for (ii = 0; ii < n_comps; ii++)  
            if (isHidden(comps[ii]))  
                UF_CALL(UF_MODL_put_list_item(hidden, comps[ii]));  
        return make_an_array(&hidden, hidden_components);  
    }

```

#### 代码解析

> 这段代码是用于NX CAD系统中的二次开发，其功能是遍历一个零件中的所有组件，并找出其中被隐藏的组件。
>
> 主要函数及其功能如下：
>
> 1. ask_next_component(part, comp)：用于遍历零件中的组件，并返回下一个组件的tag。其中，part代表零件的tag，comp代表当前组件的tag。函数会递归遍历直到返回NULL_TAG。
> 2. allocate_memory(nbytes, where)：用于分配指定大小的内存，并将内存地址存储在where指向的指针中。
> 3. make_an_array(object_list, objects)：将object_list中的所有元素（即组件的tag）保存到objects指向的数组中，然后删除object_list。object_list是UF_MODL_create_list创建的链表。
> 4. ask_all_components(part, components)：获取零件part中所有组件的tag，并保存到components指向的数组中。首先调用ask_next_component获取组件，然后调用make_an_array将组件保存到数组。
> 5. isHidden(object)：判断组件object是否被隐藏。如果是不可显示的组件，或者其显示属性为空白，则返回TRUE，表示被隐藏。
> 6. ask_hidden_components(part, hidden_components)：获取零件part中所有被隐藏的组件，并将它们的tag保存到hidden_components指向的数组中。首先调用ask_all_components获取所有组件，然后遍历组件，调用isHidden判断是否被隐藏，如果是，则保存到数组中。
>
> 这些函数实现了遍历组件、内存分配、链表操作、组件隐藏状态判断等功能，为NX CAD系统的二次开发提供了基础支持。
>
