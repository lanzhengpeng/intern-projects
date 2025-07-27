### 【0014】ask all condemned objects of type and subtype 询问所有类型和子类型为指定值且状态为“已删除”的对象

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
    static int ask_all_condemned_objects_of_type_and_subtype(tag_t part, int type,  
        int subtype, tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if ((this_type == type) && (this_subtype == subtype) &&  
                (UF_OBJ_ask_status(object) == UF_OBJ_CONDEMNED))  
                    UF_MODL_put_list_item(object_list, object);  
        }  
        return (make_an_array(&object_list, objects));  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 内存分配：使用allocate_memory函数分配指定大小的内存，返回分配的内存地址。
> 2. 创建对象数组：make_an_array函数接收对象列表和对象数组指针，遍历对象列表，将对象ID存入对象数组，并释放对象列表。
> 3. 查询特定类型和子类型的废弃对象：ask_all_condemned_objects_of_type_and_subtype函数用于查询指定零件中特定类型和子类型的废弃对象，并将这些对象存入数组返回。
> 4. 主要流程：首先创建一个对象列表，然后遍历指定零件中的所有对象，判断对象类型、子类型以及状态，如果符合条件则将其添加到列表中。接着调用make_an_array函数，将列表中的对象ID存入数组，并释放列表。最后返回数组及数组大小。
>
> 这段代码主要用于NX的二次开发，实现了对特定类型废弃对象的查询功能。
>
