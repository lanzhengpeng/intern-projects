### 【0033】ask all model curves 查询所有模型曲线

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
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_all_model_curves(tag_t part, tag_t **curves)  
    {  
        int  
            flag,  
            ii, jj,  
            n,  
            type[5] = { UF_point_type,  
                        UF_line_type,  
                        UF_circle_type,  
                        UF_conic_type,  
                        UF_spline_type };  
        tag_t  
            *objects;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        for (ii = 0; ii < 5; ii++)  
        {  
            n = ask_all_of_type(part, type[ii], &objects);  
            for (jj = 0; jj < n; jj++)  
            {  
                UF_CALL(uc6409(objects[jj], &flag, view_name));  
                if (!flag)  
                    UF_CALL(UF_MODL_put_list_item(object_list, objects[jj]));  
            }  
            if (n > 0) UF_free(objects);  
        }  
        return make_an_array(&object_list, curves);  
    }

```

#### 代码解析

> 这段NX Open API二次开发代码的主要功能是从NX部件中查询所有模型曲线，并将这些曲线存储在一个数组中。
>
> 具体步骤如下：
>
> 1. 定义一个allocate_memory函数，用于分配指定大小的内存。
> 2. 定义一个make_an_array函数，用于从列表中创建一个对象数组。
> 3. 定义一个ask_all_of_type函数，用于查询部件中指定类型的所有对象，并将这些对象存储在一个列表中。
> 4. 定义一个ask_all_model_curves函数，用于查询部件中的所有模型曲线（包括点、线、圆、圆锥曲线和样条曲线），并将这些曲线存储在一个数组中。
> 5. 在ask_all_model_curves函数中，首先创建一个空列表。
> 6. 遍历所有模型曲线类型，使用ask_all_of_type函数查询每种类型的曲线，并将曲线存储在临时数组中。
> 7. 检查每条曲线是否在当前视图中可见，如果不可见，则将其添加到列表中。
> 8. 将列表中的所有曲线转换为最终的曲线数组。
> 9. 最后删除临时列表。
> 10. ask_all_model_curves函数返回曲线数组的元素个数。
>
> 总的来说，这段代码实现了查询和获取NX部件中所有模型曲线的功能，并且可以通过曲线数组来访问这些曲线。
>
