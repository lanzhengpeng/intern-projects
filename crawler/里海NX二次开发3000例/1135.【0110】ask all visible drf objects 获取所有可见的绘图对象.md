### 【0110】ask all visible drf objects 获取所有可见的绘图对象

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
    static int ask_all_visible_drf_objects(tag_t **objects)  
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
            if ((type == UF_drafting_entity_type) || (type == UF_dimension_type))  
                UF_CALL(UF_MODL_put_list_item(list, object));  
        }  
        return make_an_array(&list, objects);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API开发的示例代码，主要实现了以下功能：
>
> 1. 内存分配：allocate_memory函数用于分配指定大小的内存空间，参数nbytes指定内存大小，where指向分配的内存地址。
> 2. 创建数组：make_an_array函数接收一个NX对象列表，创建一个数组，存储列表中所有对象的ID。首先获取列表长度，然后分配相应大小的内存，遍历列表将对象ID存入数组，最后删除列表。
> 3. 查询所有可见DRF对象：ask_all_visible_drf_objects函数首先获取当前工作视图，然后创建一个对象列表。接着遍历视图中的所有可见对象，如果是绘图实体或尺寸，则将对象ID加入列表。最后调用make_an_array将列表转换为数组。
> 4. 获取视图对象：通过UF_VIEW_ask_work_view获取当前工作视图。
> 5. 创建对象列表：通过UF_MODL_create_list创建一个空的对象列表。
> 6. 遍历视图对象：通过UF_VIEW_cycle_objects遍历视图中的所有可见对象。
> 7. 获取对象类型和子类型：通过UF_OBJ_ask_type_and_subtype获取对象类型和子类型。
> 8. 向列表中添加对象：通过UF_MODL_put_list_item向列表中添加对象。
> 9. 删除对象列表：通过UF_MODL_delete_list删除对象列表。
> 10. 返回对象数组：最后返回创建的对象数组。
>
> 总体来看，这段代码实现了NX中获取当前工作视图下所有可见的绘图实体和尺寸对象的ID，并以数组形式返回，为后续的二次开发提供了基础。
>
