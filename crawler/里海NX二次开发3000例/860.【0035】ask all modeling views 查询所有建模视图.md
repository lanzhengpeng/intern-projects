### 【0035】ask all modeling views 查询所有建模视图

#### 代码

```cpp
    static tag_t ask_next_view(tag_t part, tag_t view)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_view_type, &view));  
        return (view);  
    }  
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
    static int ask_all_modeling_views(tag_t part, tag_t **views)  
    {  
        tag_t  
            view = NULL_TAG;  
        uf_list_p_t  
            view_list;  
        UF_VIEW_type_t  
            type;  
        UF_VIEW_subtype_t  
            subtype;  
        UF_CALL(UF_MODL_create_list(&view_list));  
        while ((view = ask_next_view(part, view)) != NULL_TAG)  
        {  
            UF_CALL(UF_VIEW_ask_type(view, &type, &subtype));  
            if (type == UF_VIEW_MODEL_TYPE)  
                UF_CALL(UF_MODL_put_list_item(view_list, view));  
        }  
        return (make_an_array(&view_list, views));  
    }

```

#### 代码解析

> 这段代码主要用于NX Open C++ API进行NX二次开发。其主要功能是遍历指定部件的所有建模视图，并将这些视图存储在一个数组中。代码的主要步骤包括：
>
> 1. 定义视图遍历函数：使用ask_next_view函数，通过调用UF_OBJ_cycle_objs_in_part函数，遍历指定部件的所有视图。
> 2. 内存分配函数：使用allocate_memory函数，通过调用UF_allocate_memory函数，分配指定大小的内存空间。
> 3. 创建视图数组：使用make_an_array函数，将视图列表转换为一个视图数组。首先，通过调用UF_MODL_ask_list_count获取列表中的视图数量，然后分配相应大小的内存空间，并将视图的标签存储在数组中。
> 4. 获取所有建模视图：使用ask_all_modeling_views函数，首先创建一个视图列表，然后遍历所有视图，如果视图类型为UF_VIEW_MODEL_TYPE，则将该视图添加到列表中。最后，调用make_an_array函数，将视图列表转换为视图数组。
> 5. 视图遍历逻辑：在ask_all_modeling_views函数中，通过while循环逐个获取下一个视图，直到没有更多视图为止。如果视图类型为建模视图，则将其添加到列表中。
> 6. 内存释放：在make_an_array函数中，调用UF_MODL_delete_list释放视图列表的内存。
> 7. 返回视图数组：ask_all_modeling_views函数返回视图数组及其长度。
>
> 总的来说，这段代码实现了遍历指定部件的所有建模视图，并将这些视图存储在数组中的功能。
>
