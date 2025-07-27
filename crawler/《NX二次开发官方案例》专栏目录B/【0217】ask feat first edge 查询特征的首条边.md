### 【0217】ask feat first edge 查询特征的首条边

#### 代码

```cpp
    static tag_t ask_feat_first_edge(tag_t feat)  
    {  
        int  
            n;  
        tag_t  
            edge;  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_ask_feat_edges(feat, &edge_list));  
        UF_CALL(UF_MODL_ask_list_count(edge_list, &n));  
        if (n > 0)  
        {  
            UF_CALL(UF_MODL_ask_list_item(edge_list, 0, &edge));  
            UF_CALL(UF_MODL_delete_list(&edge_list));  
            return edge;  
        }  
        else  
            return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API中的函数，用于获取特征的第一条边。
>
> 函数的主要逻辑如下：
>
> 1. 首先调用UF_MODL_ask_feat_edges(feat, &edge_list)获取特征feat的所有边构成的列表edge_list。
> 2. 然后调用UF_MODL_ask_list_count(edge_list, &n)获取列表中的边数量n。
> 3. 如果n大于0，表示特征有边，则调用UF_MODL_ask_list_item(edge_list, 0, &edge)获取列表中的第一条边edge。
> 4. 最后调用UF_MODL_delete_list(&edge_list)删除边列表。
> 5. 如果n等于0，表示特征没有边，则返回NULL_TAG。
> 6. 如果获取边成功，则返回第一条边的tag。
>
> 总的来说，这个函数的目的是获取NX特征的第一条边。通过NX Open C++ API的UF_MODL系列函数实现。
>
