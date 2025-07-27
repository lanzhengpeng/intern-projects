### 【1526】list features 列出特征

#### 代码

```cpp
    static void list_features(tag_t item)  
    {  
        int  
            i,  
            count,  
            subtype,  
            type;  
        uf_list_p_t  
            list;  
        char  
            *feat_type;  
        tag_t  
            feat;  
        UF_CALL( UF_OBJ_ask_type_and_subtype(item, &type, &subtype));  
        switch (subtype)  
        {  
            case UF_solid_body_subtype:  
                UF_CALL( UF_MODL_ask_body_feats( item, &list));  
                printf("Body");  
                break;  
            case UF_solid_face_subtype:  
                UF_CALL( UF_MODL_ask_face_feats( item, &list));  
                printf("Face");  
                break;  
            case UF_solid_edge_subtype:  
                UF_CALL( UF_MODL_ask_edge_feats( item, &list));  
                printf("Edge");  
                break;  
        }  
        UF_CALL( UF_MODL_ask_list_count( list, &count));  
        printf(" has %d features\n", count);  
        for (i = 0; i < count; i++)  
        {  
            UF_CALL( UF_MODL_ask_list_item( list, i, &feat));  
            UF_CALL( UF_MODL_ask_feat_type( feat, &feat_type));  
            printf("\t%d.  %s\n", i+1, feat_type);  
            UF_free( feat_type);  
        }  
        UF_CALL(UF_MODL_delete_list(&list));  
    }

```

#### 代码解析

> 这段代码是一个用于在NX中查询实体特征列表并打印出来的二次开发示例。
>
> 主要流程如下：
>
> 1. 定义一个函数list_features，用于查询和打印实体特征列表。
> 2. 使用UF_OBJ_ask_type_and_subtype获取输入实体的类型和子类型。
> 3. 根据子类型，使用UF_MODL_ask_body_feats、UF_MODL_ask_face_feats、UF_MODL_ask_edge_feats获取实体上的特征列表。
> 4. 使用UF_MODL_ask_list_count获取特征列表的长度。
> 5. 遍历特征列表，使用UF_MODL_ask_list_item和UF_MODL_ask_feat_type获取每个特征的类型，并打印。
> 6. 最后使用UF_MODL_delete_list删除特征列表，释放内存。
>
> 通过这个函数，可以方便地查询NX实体上的特征列表，并进行打印，从而了解实体的特征构成。这个函数对于理解NX实体的特征结构非常有用。
>
