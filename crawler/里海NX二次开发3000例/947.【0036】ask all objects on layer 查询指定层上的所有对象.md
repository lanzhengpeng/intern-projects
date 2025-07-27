### 【0036】ask all objects on layer 查询指定层上的所有对象

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
    /*  use layer = 0 for all selectable layers 译:翻译：将所有可选层的use layer设置为0。 */  
    static int ask_all_objects_on_layer(int layer, tag_t **objects)  
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
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if ((type != UF_solid_type) || (subtype == UF_solid_body_subtype))  
            {  
                UF_CALL(UF_MODL_put_list_item(list, object));  
            }  
        }  
        return make_an_array(&list, objects);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现的功能是获取指定图层上的所有实体，并将这些实体以数组的形式返回。
>
> 具体步骤如下：
>
> 1. 定义一个函数allocate_memory，用于分配指定大小的内存，并返回指向该内存的指针。
> 2. 定义一个函数make_an_array，用于将UF列表转换为tag数组。首先获取列表中的实体数量，然后为该数量分配内存，最后将列表中的实体ID复制到数组中，并删除列表。
> 3. 定义一个函数ask_all_objects_on_layer，用于获取指定图层上的所有实体。首先创建一个空列表，然后遍历图层上的所有实体，将实体ID添加到列表中。最后调用make_an_array函数，将列表转换为tag数组并返回数组大小。
>
> 总体来说，这段代码通过NX的UF函数，实现了从NX模型中获取指定图层上所有实体的功能，并将这些实体以数组的形式返回，为后续的二次开发提供了基础。
>
