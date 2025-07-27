### 【3024】unsuppress from feat 解除特征抑制

#### 代码

```cpp
    static void suppress_from_feat(tag_t feat, tag_t *body)  
    {  
        uf_list_p_t  
            feat_list,  
            sup_list;  
        UF_CALL(UF_MODL_ask_feat_body(feat, body));  
        UF_CALL(UF_MODL_ask_body_feats(*body, &feat_list));  
        for (sup_list=feat_list; sup_list->eid != feat; sup_list=sup_list->next);  
        UF_CALL(UF_MODL_suppress_feature(sup_list));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
    }  
    static void unsuppress_from_feat(tag_t feat, tag_t body)  
    {  
        uf_list_p_t  
            feat_list,  
            sup_list;  
        UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
        for (sup_list=feat_list; sup_list->eid != feat; sup_list=sup_list->next);  
        UF_CALL(UF_MODL_unsuppress_feature(sup_list));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API开发的，主要实现以下功能：
>
> 1. suppress_from_feat函数：用于抑制指定特征在指定body上的显示。首先获取指定特征所在的body，然后获取body上的所有特征列表，遍历特征列表，找到要抑制的特征，然后调用UF_MODL_suppress_feature函数进行抑制，最后删除特征列表。
> 2. unsuppress_from_feat函数：用于取消抑制指定特征在指定body上的显示。首先获取指定body上的所有特征列表，遍历特征列表，找到要取消抑制的特征，然后调用UF_MODL_unsuppress_feature函数进行取消抑制，最后删除特征列表。
>
> 这段代码通过调用NX Open C++ API中的相关函数，实现了在NX模型中抑制和取消抑制特征显示的功能。
>
