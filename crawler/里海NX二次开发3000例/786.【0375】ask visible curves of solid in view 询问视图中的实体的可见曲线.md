### 【0375】ask visible curves of solid in view 询问视图中的实体的可见曲线

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
    static int ask_visible_curves_of_solid_in_view(tag_t body, tag_t view,  
        tag_t **view_curves)  
    {  
        logical  
            is_so;  
        int  
            ii,  
            n_parents,  
            type,  
            subtype;  
        tag_t  
            object = NULL_TAG,  
            *parents,  
            this_body;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_VIEW_cycle_objects(view, UF_VIEW_VISIBLE_OBJECTS,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_edge_subtype))  
            {  
                UF_CALL(UF_MODL_ask_edge_body(object, &this_body));  
                if (this_body == body)  
                    UF_CALL(UF_MODL_put_list_item(list, object));  
                continue;  
            }  
            UF_CALL(UF_SO_is_subclass(object, &is_so));  
            if (is_so)  
            {  
                UF_CALL(UF_SO_ask_parents(object, UF_SO_ASK_ALL_PARENTS,  
                    &n_parents, &parents));  
                for (ii = 0; ii < n_parents; ii++)  
                {  
                    UF_CALL(UF_OBJ_ask_type_and_subtype(parents[ii], &type,  
                        &subtype));  
                    if ((type == UF_solid_type) &&  
                        (subtype == UF_solid_face_subtype))  
                    {  
                        UF_CALL(UF_MODL_ask_face_body(parents[ii], &this_body));  
                        if (this_body == body)  
                            UF_CALL(UF_MODL_put_list_item(list, object));  
                    }  
                }  
                if (n_parents > 0) UF_free(parents);  
            }  
        }  
        return make_an_array(&list, view_curves);  
    }

```

#### 代码解析

> 这段代码实现了在NX中获取特定视图下实体可见曲线的功能。主要步骤包括：
>
> 1. 定义了allocate_memory函数，用于分配内存并返回指向内存的指针。
> 2. 定义了make_an_array函数，将链表中的对象转换为一个数组并返回数组长度。
> 3. 定义了ask_visible_curves_of_solid_in_view函数，用于获取实体在指定视图下的可见曲线。该函数首先遍历视图中的所有可见对象，然后检查是否为实体边缘或实体面。如果是，则将其添加到链表中。最后调用make_an_array函数将链表转换为曲线数组并返回数组长度。
> 4. 使用UF_MODL模块中的函数遍历视图中的对象，判断类型，获取实体的父对象等。
> 5. 使用UF_VIEW模块中的函数获取视图中的可见对象。
> 6. 使用UF_OBJ模块中的函数获取对象的类型和子类型。
> 7. 使用UF_SO模块中的函数判断对象是否为子类，获取父对象等。
> 8. 使用UF_MODL模块中的函数操作对象链表。
>
> 通过这些函数的组合调用，实现了在NX中获取特定视图下实体可见曲线的功能。
>
