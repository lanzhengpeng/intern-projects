### 【0045】ask all ordinate dimensions 询问所有坐标尺寸

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
    static int ask_all_ordinate_dimensions(tag_t part, tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &object))  
            && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if ((this_subtype == UF_dim_ordinate_horiz_subtype) ||  
                (this_subtype == UF_dim_ordinate_vert_subtype))  
                (UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要用于查询和创建数组。下面是对代码的简要介绍：
>
> 1. allocate_memory函数：用于分配指定大小的内存，返回指向分配内存的指针，并设置响应码。
> 2. make_an_array函数：首先获取列表中的对象数量，然后分配相应大小的内存，将列表中的对象ID放入数组，最后删除列表。
> 3. ask_all_ordinate_dimensions函数：用于查询指定部件中所有水平和垂直坐标尺寸的对象，并将它们的ID放入数组返回。
>
> 代码的关键点包括：
>
> 总的来说，这段代码实现了在NX部件中查询指定类型的对象，并将它们的ID放入数组的功能。
>
