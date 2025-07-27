### 【0213】ask feat edges 询问特征边

#### 代码

```cpp
    static int ask_feat_edges(tag_t feat, tag_p_t *feat_edges)  
    {  
        int  
            ii,  
            jj,  
            n_edges,  
            n_feats;  
        tag_t  
            *edges,  
            *feats;  
        uf_list_p_t  
            edge_list,  
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
            UF_CALL(UF_MODL_create_list(&edge_list));  
            UF_CALL(UF_MODL_ask_all_members_of_set(feat, &feats, &n_feats));  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                n_edges = ask_feat_edges(feats[ii], &edges);  
                for (jj = 0; jj < n_edges; jj++)  
                    UF_CALL(UF_MODL_put_list_item(edge_list, edges[jj]));  
                if (n_edges > 0) UF_free(edges);  
            }  
            if (n_feats > 0) UF_free(feats);  
        }  
        else if (!strcmp(type, "LINEAR_ISET") ||  
                 !strcmp(type, "CIRCULAR_ISET"))  
        {  
            UF_CALL(UF_MODL_create_list(&edge_list));  
            UF_CALL(UF_MODL_ask_instance(feat, &partial_list));  
            n_feats = make_an_array(&partial_list, &feats);  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                n_edges = ask_feat_edges(feats[ii], &edges);  
                for (jj = 0; jj < n_edges; jj++)  
                    UF_CALL(UF_MODL_put_list_item(edge_list, edges[jj]));  
                if (n_edges > 0) UF_free(edges);  
            }  
            if (n_feats > 0) UF_free(feats);  
        }  
        else UF_CALL(UF_MODL_ask_feat_edges(feat, &edge_list));  
        UF_free(type);  
        return make_an_array(&edge_list, feat_edges);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了获取NX特征对象中的边线信息的功能。
>
> 具体来说：
>
> 1. 首先判断特征的类型，如果是集合特征，如UDF、镜像集、实例集、线性阵列、圆形阵列、特征集等，则获取集合中的所有成员特征，然后递归调用ask_feat_edges函数，以获取所有成员特征的边线。
> 2. 如果是线性实例集或圆形实例集，则获取实例集中的所有实例特征，然后递归调用ask_feat_edges函数，以获取所有实例特征的边线。
> 3. 如果是普通特征，直接调用NX API UF_MODL_ask_feat_edges获取该特征的边线。
> 4. 使用NX提供的列表数据结构，将所有获取到的边线汇总到一个列表中。
> 5. 最后，将列表转换成数组，并返回给调用者。
>
> 该函数的主要功能是递归地获取NX特征对象中的所有边线，无论特征是普通特征还是集合特征。通过递归调用，可以处理各种复杂的特征类型，最终获取完整的边线信息。
>
