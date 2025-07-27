### 【1245】extract face without holes 提取无孔洞的面

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
    static void remove_object_associativity(tag_t object)  
    {  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        UF_CALL(UF_MODL_put_list_item(list, object));  
        UF_CALL(UF_MODL_delete_object_parms(list));  
        UF_CALL(UF_MODL_delete_list(&list));  
    }  
    static tag_t extract_face_without_holes(tag_t face)  
    {  
        int  
            n;  
        tag_t  
            *faces,  
            link_sheet,  
            link_feat,  
            sheet;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_extract_face(face, 0, &sheet));  
        UF_CALL(UF_MODL_ask_body_faces(sheet, &face_list));  
        n = make_an_array(&face_list, &faces);  
        if (n > 0)  
        {  
            UF_CALL(UF_WAVE_create_linked_region(n, faces, 0, NULL,  
                NULL_TAG, face, TRUE, FALSE, TRUE, &link_feat));  
            UF_free(faces);  
        }  
        UF_CALL(UF_WAVE_ask_linked_feature_geom(link_feat, &link_sheet));  
        remove_object_associativity(link_sheet);  
        UF_CALL(UF_OBJ_delete_object(sheet));  
        return link_sheet;  
    }

```

#### 代码解析

> 这段代码是NX Open API的二次开发代码，用于提取面特征并删除其中的孔洞。
>
> 主要步骤包括：
>
> 1. 提取面特征：使用UF_MODL_extract_face函数提取面特征，得到一个包含所有面的片体(sheet)。
> 2. 获取面列表：使用UF_MODL_ask_body_faces函数获取片体中的面列表。
> 3. 创建面数组：将面列表转换为一个面数组，用于后续操作。
> 4. 创建链接区域：使用UF_WAVE_create_linked_region函数在指定面上创建链接区域(link_feat)，链接区域包含原面特征中所有面，但去除了孔洞。
> 5. 获取链接面特征：通过链接区域获取链接面特征(link_sheet)，即提取出的无孔洞的面特征。
> 6. 删除关联性：使用remove_object_associativity函数删除链接面特征与原面特征之间的关联性。
> 7. 删除原片体：使用UF_OBJ_delete_object函数删除原片体。
> 8. 返回结果：返回提取出的无孔洞的面特征(link_sheet)。
>
> 该代码实现了提取面特征并去除其中孔洞的功能，通过链接区域的方式保留面特征之间的拓扑关系，最终得到了一个无孔洞的面特征。
>
