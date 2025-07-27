### 【0030】ask all mechanisms 查询所有机制

#### 代码

```cpp
    static tag_t ask_next_mechanism(tag_t part, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_mdm_mechanism_type, &object));  
        return (object);  
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
    static int ask_all_mechanisms(tag_t part, tag_t **mechanisms)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            mechanism_list;  
        UF_CALL(UF_MODL_create_list(&mechanism_list));  
        while ((object = ask_next_mechanism(part, object)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(mechanism_list, object));  
        return (make_an_array(&mechanism_list, mechanisms));  
    }

```

#### 代码解析

> 这段代码主要用于在NX中查询并获取零件中的所有机制对象。主要功能包括：
>
> 1. ask_next_mechanism函数：通过循环调用UF_OBJ_cycle_objs_in_part函数，获取零件中的下一个机制对象。如果找到，返回对象标签；如果没有更多对象，则返回NULL_TAG。
> 2. allocate_memory函数：用于申请指定大小的内存空间，并将内存地址存储在指定指针中。
> 3. make_an_array函数：遍历对象列表，将每个对象的标签放入数组中，然后释放对象列表，并返回数组长度。
> 4. ask_all_mechanisms函数：创建一个空的对象列表，循环调用ask_next_mechanism函数，将找到的每个机制对象放入列表中。最后调用make_an_array函数，将所有机制对象标签存入数组中，并返回数组长度。
>
> 总体来说，这段代码实现了在NX中查询并获取零件中所有机制对象的功能。它首先通过循环调用NX API获取对象，然后使用内存管理函数申请空间，最后将对象标签存入数组中。
>
