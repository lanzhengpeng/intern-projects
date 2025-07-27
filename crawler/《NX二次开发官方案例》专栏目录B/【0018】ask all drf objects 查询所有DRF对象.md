### 【0018】ask all drf objects 查询所有DRF对象

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
    static int ask_all_drf_objects(tag_t **objects)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &object))  
            && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if ((type == UF_drafting_entity_type) ||  
                (type == UF_dimension_type) ||  
                (type == UF_note_type) ||  
                (type == UF_tol_feature_instance_type) ||  
                (type == UF_tabular_note_type) ||  
                (type == UF_smart_model_instance_type) ||  
                (type == UF_pmi_filter_type))  
                UF_CALL(UF_MODL_put_list_item(list, object));  
        }  
        return make_an_array(&list, objects);  
    }

```

#### 代码解析

> 这段NX Open C++代码用于遍历NX装配中的所有绘图对象，并将这些对象的tag存储在一个数组中。代码的关键功能包括：
>
> 1. allocate_memory函数用于动态分配指定大小的内存，并返回指向分配内存的指针。
> 2. make_an_array函数遍历一个对象列表，将每个对象的tag存储到一个动态分配的数组中，然后删除对象列表。
> 3. ask_all_drf_objects函数用于创建一个空的对象列表，并遍历装配中的所有层。如果层中的对象是绘图对象，则将其添加到列表中。然后调用make_an_array函数，将列表中的对象tag存储到一个数组中并返回数组的大小。
> 4. 主要流程是创建一个空的对象列表，遍历装配中的所有层，将绘图对象添加到列表中，然后将列表中的对象tag存储到一个动态分配的数组中，并返回数组的大小。
> 5. 该代码的作用是获取NX装配中的所有绘图对象，并将它们的tag存储在一个数组中，以便后续进行进一步处理。
> 6. 代码采用了NX Open C++ API中的UF_MODL模块进行列表操作，UF_OBJ模块查询对象类型，以及标准C函数进行内存分配。
>
