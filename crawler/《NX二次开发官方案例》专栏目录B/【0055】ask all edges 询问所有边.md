### 【0055】ask all edges 询问所有边

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
    static int ask_all_edges(tag_t part, tag_t **edges)  
    {  
        return ask_all_of_type_and_subtype(part, UF_solid_type,  
            UF_solid_edge_subtype, edges);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要功能是遍历NX部件中的所有边并获取其ID。具体来说：
>
> 1. allocate_memory函数用于分配指定大小的内存，并返回指向该内存的指针。
> 2. make_an_array函数将一个NX对象列表转换为tag数组。
> 3. ask_all_of_type_and_subtype函数遍历NX部件中的所有对象，如果对象的子类型匹配参数指定的子类型，则将其添加到列表中，最后调用make_an_array将列表转换为数组。
> 4. ask_all_edges函数是ask_all_of_type_and_subtype的特化版本，专门用于获取部件中所有边的tag数组。
> 5. 代码整体流程是：先创建一个列表，然后循环遍历部件中的所有对象，将类型为UF_solid_type且子类型为UF_solid_edge_subtype的对象添加到列表中，最后将列表转换为数组返回。
> 6. 通过这段代码，可以方便地获取NX部件中的所有边信息，为后续的几何处理等操作提供了基础。
>
> 这段代码实现了NX部件遍历的基本框架，并且通过参数化可以获取不同类型和子类型的对象，具有很高的复用价值。
>
