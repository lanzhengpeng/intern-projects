### 【0024】ask all harnesses 询问所有线束

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
    static int ask_all_of_type_and_subtype(tag_t part, int type, int subtype,  
        tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype)  
                (UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_all_harnesses(tag_t part, tag_t **harnesses)  
    {  
        return ask_all_of_type_and_subtype(part, UF_route_route_type,  
            UF_route_harness_subtype, harnesses);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于在NX中遍历装配体，获取所有指定类型和子类型的对象，并以tag数组的形式返回。
>
> 主要功能包括：
>
> 1. allocate_memory：动态分配指定大小的内存。
> 2. make_an_array：遍历链表，将链表中的对象放入数组，并释放链表。
> 3. ask_all_of_type_and_subtype：遍历指定类型的对象，如果子类型匹配，则加入链表。遍历结束后，调用make_an_array将链表对象放入数组。
> 4. ask_all_harnesses：调用ask_all_of_type_and_subtype，获取所有类型为UF_route_route_type且子类型为UF_route_harness_subtype的对象。
>
> 这段代码实现了NX中对象的遍历和筛选，通过链表和数组来实现。
>
