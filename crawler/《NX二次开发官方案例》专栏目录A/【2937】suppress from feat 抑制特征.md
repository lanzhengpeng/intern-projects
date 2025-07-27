### 【2937】suppress from feat 抑制特征

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

```

#### 代码解析

> 这段NX Open C++代码的功能是抑制NX模型中的一个特征。主要步骤包括：
>
> 1. 调用UF_MODL_ask_feat_body函数获取特征feat所在的body。
> 2. 调用UF_MODL_ask_body_feats函数获取body中所有特征的列表feat_list。
> 3. 遍历feat_list，找到与feat相匹配的特征sup_list。
> 4. 调用UF_MODL_suppress_feature函数抑制找到的特征。
> 5. 调用UF_MODL_delete_list删除feat_list内存。
>
> 该代码主要用于在NX模型中抑制一个指定的特征，以实现某些操作，例如在模型编辑或修复时临时移除特征，以便进行后续操作。通过遍历特征列表找到匹配的特征并抑制，代码实现了所需功能。
>
