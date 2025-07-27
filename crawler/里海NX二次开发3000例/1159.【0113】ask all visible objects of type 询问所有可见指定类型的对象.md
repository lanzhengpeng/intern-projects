### 【0113】ask all visible objects of type 询问所有可见指定类型的对象

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
    static int ask_all_visible_objects_of_type(int the_type, tag_t **objects)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG,  
            view;  
        uf_list_p_t  
            list;  
        if (UF_CALL(UF_VIEW_ask_work_view(&view))) return 0;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_VIEW_cycle_objects(view, UF_VIEW_VISIBLE_OBJECTS,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if (type == the_type)  
                UF_CALL(UF_MODL_put_list_item(list, object));  
        }  
        return make_an_array(&list, objects);  
    }

```

#### 代码解析

> 这段代码是NX Open API的二次开发代码，主要实现了以下功能：
>
> 1. 内存分配函数allocate_memory：用于申请指定大小的内存，并返回内存指针。
> 2. 创建数组函数make_an_array：根据传入的对象列表，创建一个tag数组，并返回数组大小。
> 3. 获取可见对象函数ask_all_visible_objects_of_type：获取当前工作视图中指定类型的所有可见对象，并返回一个tag数组。
> 4. 使用NX Open API中的函数，如UF_MODL_create_list、UF_VIEW_cycle_objects等，来遍历视图中的对象，并将指定类型的对象放入列表。
> 5. 最后，调用make_an_array函数，根据对象列表创建并返回一个tag数组。
>
> 总的来说，这段代码实现了在NX工作视图中获取指定类型的所有可见对象的功能，并返回这些对象的tag数组。这为后续的二次开发操作提供了基础。
>
