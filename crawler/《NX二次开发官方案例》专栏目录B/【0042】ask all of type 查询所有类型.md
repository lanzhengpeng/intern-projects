### 【0042】ask all of type 查询所有类型

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
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于在NX中查询指定类型（type）的所有对象，并将它们存储在数组中。
>
> 主要功能包括：
>
> 1. allocate_memory函数：用于申请指定大小的内存，返回内存地址并存储在where参数指向的指针中。
> 2. make_an_array函数：遍历对象链表，将每个对象元素的ID存储在数组中，然后释放链表内存。
> 3. ask_all_of_type函数：创建一个对象链表，循环查询指定类型(type)的对象，并将它们添加到链表中。然后调用make_an_array函数，将链表中的对象ID存储到数组中。
> 4. 该代码实现了NX中查询指定类型对象的功能，通过链表和数组进行数据存储，并使用了NX提供的API函数进行对象遍历和内存申请。
>
> 总体来说，这段代码实现了在NX中查询指定类型对象的功能，通过链表和数组进行数据存储，并使用了NX提供的API函数进行对象遍历和内存申请。
>
