### 【0048】ask all points 询问所有点

#### 代码

```cpp
    static tag_t ask_next_point(tag_t part, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_point_type, &object));  
        return (object);  
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
    static int ask_object_layer(tag_t object)  
    {  
        logical  
            is_displayable;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayable));  
        if (is_displayable &&  
            !UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props)))  
                return disp_props.layer;  
        else  
            return -1;  
    }  
    static int ask_all_points(tag_t part, tag_t **points)  
    {  
        int  
            layer;  
        tag_t  
            point = NULL_TAG;  
        uf_list_p_t  
            point_list;  
        UF_CALL(UF_MODL_create_list(&point_list));  
        while ((point = ask_next_point(part, point)) != NULL_TAG)  
        {  
            layer = ask_object_layer(point);  
            if ((layer > 0) && (layer < 257))  
                UF_CALL(UF_MODL_put_list_item(point_list, point));  
        }  
        return (make_an_array(&point_list, points));  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发，其主要功能是遍历指定零件中的所有点，并将这些点的标签存储在一个数组中。
>
> 具体步骤如下：
>
> 1. 定义了一个ask_next_point函数，用于遍历零件中的点，每次调用都会返回下一个点的标签，直到没有点为止。
> 2. 定义了一个allocate_memory函数，用于分配指定大小的内存。
> 3. 定义了一个make_an_array函数，用于将链表中的元素复制到一个数组中，并释放链表内存。
> 4. 定义了一个ask_object_layer函数，用于查询一个NX对象是否可显示，并返回其图层号。
> 5. 定义了一个ask_all_points函数，用于遍历指定零件中的所有点，并将这些点的标签存储在一个数组中返回。
>
> ask_all_points函数是这段代码的核心，它首先创建一个链表，然后循环调用ask_next_point函数获取下一个点的标签，并使用ask_object_layer函数判断点的图层号，如果图层号在1-256之间，则将点添加到链表中。最后调用make_an_array函数将链表中的点标签复制到一个数组中返回。
>
> 通过这段代码，可以实现遍历零件中的所有点，并将点的标签存储在一个数组中，以便后续的二次开发使用。
>
