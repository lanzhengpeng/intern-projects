### 【0322】ask part feats 询问零件特征

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
    static int ask_part_feats(tag_t part, tag_p_t *feats)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            body = NULL_TAG,  
            feat;  
        uf_list_p_t  
            feat_list,  
            part_list;  
        UF_CALL(UF_MODL_create_list(&part_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_MODL_ask_body_feats(body, &feat_list)))  
            {  
                UF_CALL(UF_MODL_ask_list_count(feat_list, &n));  
                for (ii = 0; ii < n; ii++)  
                {  
                    UF_CALL(UF_MODL_ask_list_item(feat_list, ii, &feat));  
                    UF_CALL(UF_MODL_put_list_item(part_list, feat));  
                }  
                UF_CALL(UF_MODL_delete_list(&feat_list));  
            }  
        }  
        n = make_an_array(&part_list, feats);  
        return n;  
    }

```

#### 代码解析

> 这段代码是用于NX CAD系统的二次开发，主要包含以下几个部分：
>
> 1. ask_next_body函数：该函数遍历指定零件中的实体体，并返回下一个实体体的tag。它通过UF_OBJ_cycle_objs_in_part函数遍历实体体，然后判断实体体的子类型，如果为实体体子类型则返回该实体体的tag。
> 2. allocate_memory函数：该函数用于分配指定大小的内存，并返回内存指针。它调用UF_allocate_memory函数进行内存分配。
> 3. make_an_array函数：该函数将列表中的对象转换为一个tag数组，并返回数组长度。它首先获取列表中对象的数量，然后为每个对象分配内存，并从列表中获取对象tag存入数组，最后删除列表。
> 4. ask_part_feats函数：该函数获取指定零件中的特征，并将特征存入数组返回。它首先创建一个空列表，然后遍历零件中的所有实体体，获取每个实体体的特征列表，将特征添加到空列表中，最后将列表转换为一个特征数组并返回数组长度。
>
> 综合来看，这段代码通过遍历零件中的实体体，获取每个实体体的特征，并将特征存储在数组中返回。它提供了对NX零件特征进行操作的基础功能。
>
