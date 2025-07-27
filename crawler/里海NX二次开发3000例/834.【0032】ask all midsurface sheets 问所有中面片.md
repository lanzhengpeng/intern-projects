### 【0032】ask all midsurface sheets 问所有中面片

#### 代码

```cpp
    static tag_t ask_next_midsurface_sheet_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        char  
            *f_type;  
        uf_list_p_t  
            feat_list,  
            tmp;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SHEET_BODY)  
                {  
                    UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
                    for (tmp = feat_list; tmp != NULL; tmp = tmp->next)  
                    {  
                        UF_CALL(UF_MODL_ask_feat_type(tmp->eid, &f_type));  
                        if (!strcmp(f_type, "MIDSURFACE"))  
                        {  
                            UF_free(f_type);  
                            UF_CALL(UF_MODL_delete_list(&feat_list));  
                            return body;  
                        }  
                        UF_free(f_type);  
                    }  
                    UF_CALL(UF_MODL_delete_list(&feat_list));  
                }  
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
    static int ask_all_midsurface_sheets(tag_t part, tag_t **sheets)  
    {  
        tag_t  
            sheet = NULL_TAG;  
        uf_list_p_t  
            sheet_list;  
        UF_CALL(UF_MODL_create_list(&sheet_list));  
        while ((sheet = ask_next_midsurface_sheet_body(part, sheet)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(sheet_list, sheet));  
        return (make_an_array(&sheet_list, sheets));  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，其主要功能是从NX部件中检索所有具有中面特征的中面片体。代码逻辑如下：
>
> 1. 函数 ask_next_midsurface_sheet_body：该函数用于从部件中循环遍历所有实体，并判断实体是否为片体。如果是片体，会进一步判断其特征列表中是否存在“MIDSURFACE”特征。如果找到具有“MIDSURFACE”特征的片体，函数会返回该片体的标签。
> 2. 函数 allocate_memory：该函数用于分配指定大小的内存。
> 3. 函数 make_an_array：该函数将链表中的元素提取到数组中，并释放链表。
> 4. 函数 ask_all_midsurface_sheets：该函数首先创建一个空链表，然后循环调用 ask_next_midsurface_sheet_body 函数将找到的中面片体添加到链表中。最后调用 make_an_array 函数将链表中的中面片体提取到一个数组中并返回。
>
> 总体来说，这段代码实现了从NX部件中检索所有具有中面特征的中面片体的功能。通过链表和数组的操作，代码实现了对大量中面片体的有效管理。
>
