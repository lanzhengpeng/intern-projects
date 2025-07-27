### 【0039】ask all of name ask all of name 查询所有同名对象

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
    static tag_t ask_next_of_name(char *name, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_by_name(name, &object));  
        return object;  
    }  
    static int ask_all_of_name(char *name, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while ((object = ask_next_of_name(name, object)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return make_an_array(&object_list, objects);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要实现了以下几个功能：
>
> 1. 内存分配：allocate_memory函数用于调用UF函数分配指定大小的内存，并返回内存地址。
> 2. 创建数组：make_an_array函数根据传入的对象列表，创建一个对象ID数组，并返回数组长度。它首先获取列表的长度，然后分配相应大小的内存，最后将列表中的对象ID复制到数组中，并释放列表。
> 3. 查询下一个对象：ask_next_of_name函数根据名称查询下一个对象，并返回其ID。
> 4. 查询所有对象：ask_all_of_name函数根据名称查询所有对象，将它们放入一个列表中，然后调用make_an_array函数创建对象ID数组，并返回数组长度。
>
> 这些函数通过NX Open C++ API提供的UF函数实现了NX中的对象查询、内存管理等功能。它们可以用于NX二次开发中根据名称查询对象，或者创建对象ID数组等场景。
>
