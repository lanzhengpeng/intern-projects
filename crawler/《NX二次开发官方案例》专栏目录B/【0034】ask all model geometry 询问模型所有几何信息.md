### 【0034】ask all model geometry 询问模型所有几何信息

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
    static int ask_all_model_geometry(tag_t **geometry)  
    {  
        int  
            flag,  
            ii,  
            old_layer_settings[UF_LAYER_MAX_LAYER],  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        for (ii = 1; ii <= UF_LAYER_MAX_LAYER; ii++)  
            UF_CALL(UF_LAYER_ask_status(ii, &old_layer_settings[ii-1]));  
        for (ii = 1; ii <= UF_LAYER_MAX_LAYER; ii++)  
            UF_CALL(UF_LAYER_set_status(ii, UF_LAYER_ACTIVE_LAYER));  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype != UF_solid_body_subtype)) continue;  
            if (type == UF_faceted_model_type) continue;  
            UF_CALL(uc6409(object, &flag, view_name));  
            if (!flag) UF_CALL(UF_MODL_put_list_item(list, object));  
        }  
        for (ii = 1; ii <= UF_LAYER_MAX_LAYER; ii++)  
            UF_CALL(UF_LAYER_set_status(ii, old_layer_settings[ii-1]));  
        return make_an_array(&list, geometry);  
    }

```

#### 代码解析

> 这段代码实现了在NX中查询所有模型几何体的功能，并简要介绍如下：
>
> 1. 内存分配函数allocate_memory：用于动态分配指定大小的内存，并返回指向该内存的指针。
> 2. 数组创建函数make_an_array：遍历传入的列表，将列表中的对象ID提取出来，放入数组中，并释放列表。
> 3. 模型几何体查询函数ask_all_model_geometry：保存所有层的当前状态，并设置所有层为激活状态，以确保查询到所有几何体。创建一个空列表，用于存放查询到的模型几何体。循环遍历所有层，获取每个层的模型对象，并判断其类型和子类型，如果是实体或面网格模型则跳过，否则将其加入列表。最后，将列表中的对象ID提取出来，放入数组中，并恢复所有层的原始状态。
> 4. 保存所有层的当前状态，并设置所有层为激活状态，以确保查询到所有几何体。
> 5. 创建一个空列表，用于存放查询到的模型几何体。
> 6. 循环遍历所有层，获取每个层的模型对象，并判断其类型和子类型，如果是实体或面网格模型则跳过，否则将其加入列表。
> 7. 最后，将列表中的对象ID提取出来，放入数组中，并恢复所有层的原始状态。
> 8. 通过以上函数，实现了查询当前打开的NX部件中所有模型几何体的功能，并将这些几何体的ID保存在一个数组中返回。
>
