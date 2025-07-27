### 【0107】ask all solids 询问所有固体

#### 代码

```cpp
    static tag_t ask_next_solid_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SOLID_BODY) return body;  
            }  
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
    static int ask_all_solids(tag_t part, tag_t **solids)  
    {  
        tag_t  
            solid = NULL_TAG;  
        uf_list_p_t  
            solid_list;  
        UF_CALL(UF_MODL_create_list(&solid_list));  
        while ((solid = ask_next_solid_body(part, solid)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(solid_list, solid));  
        return (make_an_array(&solid_list, solids));  
    }

```

#### 代码解析

> 这段代码是NX（前称NX Nastran）二次开发代码，主要实现了以下功能：
>
> 1. ask_next_solid_body函数：该函数用于遍历指定零件中的所有实体，并返回下一个实体体的标签。它首先获取当前实体体的标签，然后循环遍历实体体，直到找到下一个实体体为止。
> 2. allocate_memory函数：该函数用于申请指定大小的内存，并返回内存指针。
> 3. make_an_array函数：该函数用于将列表转换为数组。首先获取列表的长度，然后申请相应大小的内存，将列表中的元素复制到数组中，最后删除列表。
> 4. ask_all_solids函数：该函数用于获取指定零件中的所有实体体的标签数组。首先创建一个实体体列表，然后遍历零件中的所有实体体，将实体体添加到列表中，最后将列表转换为数组并返回。
>
> 总体来说，这段代码实现了遍历实体体、申请内存、列表转数组等功能，主要用于获取零件中所有实体体的标签数组。
>
