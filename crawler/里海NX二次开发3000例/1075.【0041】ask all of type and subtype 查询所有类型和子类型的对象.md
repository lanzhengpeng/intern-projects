### 【0041】ask all of type and subtype 查询所有类型和子类型的对象

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

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要用于查询NX部件中的对象。
>
> 主要包含以下函数：
>
> 1. allocate_memory：用于申请指定大小的内存，并将内存地址赋值给传入的void**指针参数。
> 2. make_an_array：将NX列表转换为tag_t类型的数组。首先获取列表中对象的个数，然后申请相应大小的内存，并将列表中的对象id复制到数组中。最后删除NX列表。
> 3. ask_all_of_type_and_subtype：用于查询指定NX部件中指定类型和子类型对象。首先创建一个NX列表，然后遍历部件中的所有对象，如果对象的子类型匹配传入的子类型，则将该对象添加到列表中。最后调用make_an_array函数将列表转换为数组并返回。
>
> 总体来说，这段代码通过NX Open C++ API实现了NX部件中对象的查询和转换功能。
>
