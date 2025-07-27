### 【0211】ask feat body ask_feat_body 查询特征体

#### 代码

```cpp
    static tag_t ask_feat_body(tag_t feat)  
    {  
        int  
            n_feats;  
        tag_t  
            body,  
            *feats,  
            mfeat = feat;  
    /*  UF_MODL_ask_feat_body returns a NULL_TAG when the given feature is a set 译:当给定的特性是一个集合时，UF_MODL_ask_feat_body 返回 NULL_TAG。 */  
        if (!UF_MODL_ask_all_members_of_set(feat, &feats, &n_feats))  
        {  
            if (n_feats > 0)  
            {  
                mfeat = feats[0];  
                UF_free(feats);  
            }  
        }  
        UF_CALL(UF_MODL_ask_feat_body(mfeat, &body));  
        return body;  
    }

```

#### 代码解析

> 根据代码注释，这段代码的功能是查询给定特征的身体。具体步骤如下：
>
> 1. 定义静态方法 ask_feat_body，输入参数为特征 feat。
> 2. 判断输入特征 feat 是否是一个集合。如果是，则获取该集合的第一个成员作为新的特征 mfeat。
> 3. 调用 NX API 函数 UF_MODL_ask_feat_body，查询特征 mfeat 的身体，并返回结果。
> 4. 如果输入的特征是一个集合，需要释放动态内存。
> 5. 最终返回特征的身体。
>
> 这段代码实现了对 NX 特征身体的高层封装，能够处理特征是集合的情况，简化了 NX 特征查询的调用方式。
>
