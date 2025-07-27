### 【0105】ask all solids and solid occs 询问所有实体和实体出现

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
    static void ask_all_solids_and_solid_occs(tag_t part, tag_t **solids, int  
        *n_solids, tag_t **solid_occs, int *n_occs)  
    {  
        tag_t  
            body = NULL_TAG;  
        uf_list_p_t  
            body_list,  
            occ_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        UF_CALL(UF_MODL_create_list(&occ_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            if (UF_ASSEM_is_occurrence(body))  
                UF_CALL(UF_MODL_put_list_item(occ_list, body));  
            else  
                UF_CALL(UF_MODL_put_list_item(body_list, body));  
        *n_solids = make_an_array(&body_list, solids);  
        *n_occs = make_an_array(&occ_list, solid_occs);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. ask_next_body函数：用于遍历零件中的所有body，并返回下一个是实体的body。它通过UF_OBJ_cycle_objs_in_part遍历零件中的所有body，然后判断body的类型，如果是实体body则返回。
> 2. allocate_memory函数：用于申请指定大小的内存空间，并返回内存指针。
> 3. make_an_array函数：用于将UF列表转换成tag_t数组。它首先获取列表中元素的数量，然后申请内存空间，将列表中的元素id存入数组中，最后删除UF列表。
> 4. ask_all_solids_and_solid_occs函数：用于获取零件中的所有实体body和实体occurrence，并将它们分别存储在solids数组和solid_occs数组中。它首先创建两个UF列表，然后遍历所有body，根据body是实体还是实体occurrence将其加入不同的列表中，最后调用make_an_array函数将列表转换为数组。
>
> 总体来说，这段代码通过NX的UF接口遍历和查询零件信息，提取了零件中的实体body和实体occurrence，并分别存储为C数组，以供后续处理使用。
>
