### 【0058】ask all of types 获取所有类型

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
    static int ask_all_of_types(tag_t part, int n_types, int *types,  
        tag_t **objects)  
    {  
        int  
            ii;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        for (ii = 0; ii < n_types; ii++)  
        {  
            while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, types[ii], &object))  
                && (object != NULL_TAG))  
                UF_CALL(UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的二次开发代码，主要功能是从NX部件中查询指定类型的对象，并返回它们的对象标识数组。
>
> 代码主要包括以下几个部分：
>
> 1. allocate_memory：分配指定大小的内存，并返回指向内存的指针。
> 2. make_an_array：将对象列表转换为对象标识数组，并释放列表。
> 3. ask_all_of_types：从指定部件中查询指定类型的对象，并将它们放入列表，最后转换为对象标识数组返回。
>
> 具体来说，ask_all_of_types函数会遍历部件中的每个指定类型对象，将它们放入一个链表。然后，make_an_array函数会遍历链表，创建一个对象标识数组，并将链表中的对象标识复制到数组中。最后，释放链表。
>
> 这段代码实现了NX部件中特定类型对象的查询和返回，为后续的二次开发提供了基础。
>
