### 【0054】ask all bodies 询问所有实体

#### 代码

```cpp
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
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
    static int ask_all_bodies(tag_t part, tag_t **solids)  
    {  
        tag_t  
            body = NULL_TAG;  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(body_list, body));  
        return (make_an_array(&body_list, solids));  
    }

```

#### 代码解析

> 这段代码是用于NX(UG)的二次开发，主要实现了以下功能：
>
> 1. ask_next_body函数：用于遍历指定部件(part)中的所有实体，并返回下一个实体(body)的tag。若当前实体为空，则返回NULL。同时，该函数通过调用UF_OBJ_cycle_objs_in_part来遍历实体，并判断实体类型是否为实体体(solid_body_subtype)，若是则返回该实体。
> 2. allocate_memory函数：用于为NX分配指定大小的内存空间，并返回内存指针。其中，nbytes表示需要分配的内存大小，而where用于返回内存指针。
> 3. make_an_array函数：用于将NX的列表转换为数组。它首先获取列表的长度n，然后分配n个tag_t大小的内存空间，并将列表中的元素复制到数组中，最后删除NX的列表。参数object_list表示NX的列表，而objects表示返回的数组。
> 4. ask_all_bodies函数：用于获取指定部件中的所有实体体，并返回实体体的数组。首先调用ask_next_body来遍历实体，并将实体添加到NX的列表中。最后调用make_an_array来将列表转换为数组。
>
> 这些函数相互配合，实现了NX实体遍历、内存分配、列表与数组转换等功能，为NX二次开发提供了便利。
>
