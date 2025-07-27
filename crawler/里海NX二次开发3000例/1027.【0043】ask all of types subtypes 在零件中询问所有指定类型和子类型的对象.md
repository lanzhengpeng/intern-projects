### 【0043】ask all of types subtypes 在零件中询问所有指定类型和子类型的对象

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
    static int ask_all_of_types_subtypes(tag_t part, int n_types, int *types,  
        int *subtypes, tag_t **objects)  
    {  
        int  
            ii,  
            type,  
            subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        for (ii = 0; ii < n_types; ii++)  
        {  
            while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, types[ii], &object))  
                && (object != NULL_TAG))  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
                if (subtype == subtypes[ii])  
                    UF_CALL(UF_MODL_put_list_item(object_list, object));  
            }  
        }  
        return (make_an_array(&object_list, objects));  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的函数，主要实现以下功能：
>
> 1. allocate_memory函数：用于在NX中分配指定大小的内存，并将内存地址存储在提供的指针中。
> 2. make_an_array函数：遍历NX对象列表，并将对象ID存储在数组中。首先，调用UF_MODL_ask_list_count获取对象数量，然后调用allocate_memory为对象数组分配内存。接着，遍历对象列表，将每个对象的ID存储在数组中。最后，调用UF_MODL_delete_list删除对象列表。
> 3. ask_all_of_types_subtypes函数：用于查询指定类型和子类型在NX部件中的对象。首先，调用UF_MODL_create_list创建对象列表。然后，遍历提供的类型和子类型，使用UF_OBJ_cycle_objs_in_part和UF_OBJ_ask_type_and_subtype循环查询部件中的对象，并将匹配的对象添加到列表中。最后，调用make_an_array将对象列表转换成对象数组，并返回对象数量。
>
> 整体来看，这段代码实现了在NX部件中按类型和子类型查询对象，并将查询结果存储在数组中。这些函数可以方便地在NX二次开发中使用。
>
