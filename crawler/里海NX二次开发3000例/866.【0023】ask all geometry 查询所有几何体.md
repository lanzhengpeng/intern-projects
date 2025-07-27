### 【0023】ask all geometry 查询所有几何体

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
    static int ask_all_geometry(tag_t **geometry)  
    {  
        logical  
            ok;  
        int  
            subtype,  
            type;  
        tag_t  
            item = NULL_TAG;  
        uf_list_p_t  
            geo_list;  
        UF_CALL(UF_MODL_create_list(&geo_list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &item)) && (item != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(item, &type, &subtype));  
            UF_CALL(UF_OBJ_is_type_transferable(type, &ok));  
            if (ok) UF_CALL(UF_MODL_put_list_item(geo_list, item));  
        }  
        return make_an_array(&geo_list, geometry);  
    }

```

#### 代码解析

> 这段代码是用于NX（Siemens NX）CAD系统的二次开发代码。下面是代码的功能介绍：
>
> 1. allocate_memory函数：用于在NX中申请指定大小的内存，并将内存地址存储在where指针指向的变量中。该函数封装了NX的UF_allocate_memory函数，并返回NX的错误码。
> 2. make_an_array函数：用于从NX的列表中提取元素，创建一个NX元素的数组，并释放列表。它首先获取列表的元素个数，然后申请相应大小的内存，接着遍历列表，将元素ID存入数组，最后删除列表。
> 3. ask_all_geometry函数：用于获取NX模型中所有可转移的几何体。它创建一个空列表，然后循环遍历所有层，检查每个图元的类型和子类型，如果图元可转移，则添加到列表中。最后调用make_an_array函数，创建一个NX图元数组，并返回数组大小。
>
> 总的来说，这段代码通过NX的API获取了模型中的所有可转移几何体，并将它们的ID存储在一个数组中，为后续处理这些几何体提供了便利。
>
