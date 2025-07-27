### 【1383】highlight feat edges 高亮特征边

#### 代码

```cpp
    static void highlight_feat_edges(tag_t feat)  
    {  
        int  
            n_edges;  
        char  
            msg[133];  
        uf_list_p_t  
            ii,  
            edge_list;  
        UF_CALL(UF_MODL_ask_feat_edges(feat, &edge_list));  
        for (ii = edge_list; ii != NULL; ii = ii->next)  
            UF_CALL(UF_DISP_set_highlight(ii->eid, TRUE));  
        UF_CALL(UF_MODL_ask_list_count(edge_list, &n_edges));  
        sprintf(msg, "Feature has %d edges", n_edges);  
        uc1601(msg, TRUE);  
        for (ii = edge_list; ii != NULL; ii = ii->next)  
            UF_CALL(UF_DISP_set_highlight(ii->eid, FALSE));  
        UF_CALL(UF_MODL_delete_list(&edge_list));  
    }

```

#### 代码解析

> 这段NX Open C++代码的功能是高亮显示NX特征的所有边线，并显示特征边的数量。
>
> 具体步骤如下：
>
> 1. 定义一个静态函数highlight_feat_edges，用于高亮显示特征的所有边线。
> 2. 获取特征的所有边线，存储在edge_list链表中。
> 3. 遍历edge_list链表，调用UF_DISP_set_highlight函数，将每条边线设置为高亮显示。
> 4. 获取edge_list链表的长度，即特征边的数量，并显示在消息框中。
> 5. 再次遍历edge_list链表，调用UF_DISP_set_highlight函数，将每条边线的高亮显示关闭。
> 6. 释放edge_list链表占用的内存。
> 7. 在需要的地方调用highlight_feat_edges函数，传入特征ID，即可高亮显示该特征的所有边线。
>
> 这段代码实现了对特征边线的高亮显示和数量的统计，可以用于NX二次开发中的特征操作和显示。
>
