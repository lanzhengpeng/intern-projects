### 【0221】ask feat second edge 询问特征第二边

#### 代码

```cpp
    static tag_t ask_feat_second_edge(tag_t feat)  
    {  
        int  
            n;  
        tag_t  
            edge;  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_ask_feat_edges(feat, &edge_list));  
        UF_CALL(UF_MODL_ask_list_count(edge_list, &n));  
        if (n > 1)  
        {  
            UF_CALL(UF_MODL_ask_list_item(edge_list, 1, &edge));  
            UF_CALL(UF_MODL_delete_list(&edge_list));  
            return edge;  
        }  
        else  
            return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API开发的，其主要功能是查询特征中的第二个边。
>
> 代码的主要逻辑如下：
>
> 1. 使用UF_MODL_ask_feat_edges函数获取特征的所有边的列表。
> 2. 使用UF_MODL_ask_list_count函数获取边的数量。
> 3. 如果边的数量大于1，说明特征至少有两个边。
> 4. 使用UF_MODL_ask_list_item函数获取列表中的第二个边（索引为1，因为列表是从0开始的）。
> 5. 使用UF_MODL_delete_list函数删除边列表。
> 6. 返回第二个边的tag。
> 7. 如果边的数量不大于1，则返回NULL_TAG表示没有第二个边。
>
> 通过以上逻辑，这段代码实现了查询特征中第二个边的功能。
>
