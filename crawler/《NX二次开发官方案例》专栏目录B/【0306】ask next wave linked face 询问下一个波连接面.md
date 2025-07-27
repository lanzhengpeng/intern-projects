### 【0306】ask next wave linked face 询问下一个波连接面

#### 代码

```cpp
    static tag_t ask_next_face(tag_t part, tag_t face)  
    {  
        int  
            status,  
            subtype,  
            type;  
        while ((! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &face)))  
            && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            status = UF_OBJ_ask_status(face);  
            if ((subtype == UF_solid_face_subtype) && (status == UF_OBJ_ALIVE))  
                return face;  
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
    static tag_t ask_next_wave_linked_face(tag_t part, tag_t face, tag_t *link_feat)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            *feats;  
        char  
            *type;  
        uf_list_p_t  
            feat_list;  
        while ((face = ask_next_face(part, face)) != NULL_TAG)  
        {  
            if (UF_ASSEM_is_occurrence(face)) continue;  
            UF_CALL(UF_MODL_ask_face_feats(face, &feat_list));  
            n_feats = make_an_array(&feat_list, &feats);  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feats[ii], &type));  
                if (!strcmp(type, "LINKED_FACE"))  
                {  
                    *link_feat = feats[ii];  
                    UF_free(type);  
                    UF_free(feats);  
                    return face;  
                }  
                UF_free(type);  
            }  
            UF_free(feats);  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API进行NX二次开发的一部分，其主要功能是遍历零件中的所有面，并找到与链接特征(linked feature)相关联的面。
>
> 主要步骤包括：
>
> 1. ask_next_face函数用于遍历零件中的所有面，并返回下一个有效的实体面。它通过调用UF_OBJ_cycle_objs_in_part函数遍历零件中的所有对象，并判断对象类型是否为面实体，状态是否为激活状态。
> 2. allocate_memory函数用于分配指定大小的内存。
> 3. make_an_array函数用于将链表中的对象转换为数组。它首先获取链表中的对象数量，然后分配内存，将链表中的对象id复制到数组中，并删除链表。
> 4. ask_next_wave_linked_face函数用于遍历零件中的所有面，并找到与链接特征(linked feature)相关联的面。它首先调用ask_next_face获取下一个面，然后获取面的特征链表，转换为数组。遍历数组，判断特征类型是否为"LINKED_FACE"，如果是则返回该面和链接特征id。
>
> 通过这些函数的组合，可以实现遍历零件中的所有面，并找到与链接特征相关联的面。这通常用于后续的面处理，如提取链接面进行网格划分等。
>
