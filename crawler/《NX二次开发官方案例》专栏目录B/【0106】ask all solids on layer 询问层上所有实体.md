### 【0106】ask all solids on layer 询问层上所有实体

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
    static int ask_all_solids_on_layer(int layer, tag_t **objects)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(layer, &object)) &&  
            (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype))  
            if ((type == UF_solid_type) && (subtype == UF_solid_subtype))  
                UF_CALL(UF_MODL_put_list_item(list, object));  
        }  
        return make_an_array(&list, objects);  
    }

```

#### 代码解析

> 这段代码是用于NX Open的二次开发代码，其主要功能是获取指定层上的所有实体并存储在一个tag数组中。代码的逻辑如下：
>
> 1. allocate_memory函数用于申请指定大小的内存，并返回指向内存的指针。
> 2. make_an_array函数用于将一个实体列表转换为一个tag数组。它首先获取列表中的实体数量，然后申请相应大小的内存来存储这些实体，并遍历列表，将每个实体的tag存入数组中。最后，删除实体列表。
> 3. ask_all_solids_on_layer函数用于获取指定层上的所有实体。它首先创建一个实体列表，然后遍历指定层上的所有实体，如果实体是实体类型，则添加到列表中。最后，调用make_an_array将列表转换为tag数组。
>
> 总体来说，这段代码通过NX提供的API实现了对特定层上的所有实体进行遍历和获取的功能，并通过一个tag数组来存储这些实体，以便后续进行进一步处理。
>
