### 【0214】ask feat faces 查询特征面

#### 代码

```cpp
    static int ask_feat_faces(tag_t feat, tag_p_t *feat_faces)  
    {  
        int  
            ii,  
            jj,  
            n_faces,  
            n_feats;  
        tag_t  
            *faces,  
            *feats;  
        uf_list_p_t  
            face_list,  
            partial_list;  
        char  
            *type;  
        UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
        if (!strcmp(type, "UDF_INS") ||  
            !strcmp(type, "MIRROR_SET") ||  
            !strcmp(type, "INSTNACE_SET") ||  
            !strcmp(type, "INSTANCE_SET") ||  
            !strcmp(type, "LINEAR_PATTERN") ||  
            !strcmp(type, "CIRCULAR_PATTERN") ||  
            !strcmp(type, "FSET"))  
        {  
            UF_CALL(UF_MODL_create_list(&face_list));  
            UF_CALL(UF_MODL_ask_all_members_of_set(feat, &feats, &n_feats));  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                n_faces = ask_feat_faces(feats[ii], &faces);  
                for (jj = 0; jj < n_faces; jj++)  
                    UF_CALL(UF_MODL_put_list_item(face_list, faces[jj]));  
                if (n_faces > 0) UF_free(faces);  
            }  
            if (n_feats > 0) UF_free(feats);  
        }  
        else if (!strcmp(type, "LINEAR_ISET") ||  
                 !strcmp(type, "CIRCULAR_ISET"))  
        {  
            UF_CALL(UF_MODL_create_list(&face_list));  
            UF_CALL(UF_MODL_ask_instance(feat, &partial_list));  
            n_feats = make_an_array(&partial_list, &feats);  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                n_faces = ask_feat_faces(feats[ii], &faces);  
                for (jj = 0; jj < n_faces; jj++)  
                    UF_CALL(UF_MODL_put_list_item(face_list, faces[jj]));  
                if (n_faces > 0) UF_free(faces);  
            }  
            if (n_feats > 0) UF_free(feats);  
        }  
        else UF_CALL(UF_MODL_ask_feat_faces(feat, &face_list));  
        UF_free(type);  
        return make_an_array(&face_list, feat_faces);  
    }

```

#### 代码解析

> 这段代码是一个NX Open API函数，用于查询特征包含的所有面。其主要功能包括：
>
> 1. 首先判断特征的类型。如果是集合特征(UDF_INS、MIRROR_SET等)，则获取集合中的所有成员特征，并递归查询每个成员特征包含的面，然后将这些面添加到面列表中。
> 2. 如果是实例特征(LINEAR_ISET、CIRCULAR_ISET等)，则获取该特征的所有实例，递归查询每个实例特征包含的面，然后将这些面添加到面列表中。
> 3. 对于普通特征，直接调用NX API查询特征包含的面，并添加到面列表中。
> 4. 最后，将面列表转换为数组并返回。
> 5. 在递归查询过程中，会释放临时变量，避免内存泄露。
> 6. 使用NX Open API的UF_MODL系列函数实现NX模型的操作。
> 7. 使用UF_CALL宏封装函数调用，便于错误处理。
> 8. 返回值是查询到的面的数量。
>
> 这段代码实现了递归查询特征包含的所有面，并返回这些面的数组，为后续操作提供了基础。
>
