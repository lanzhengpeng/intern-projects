### 【1377】highlight body edges 高亮显示体边

#### 代码

```cpp
    static void highlight_body_edges(tag_t body)  
    {  
        int  
            n_edges;  
        char  
            msg[133];  
        uf_list_p_t  
            ii,  
            edge_list;  
        UF_CALL(UF_MODL_ask_body_edges(body, &edge_list));  
        for (ii = edge_list; ii != NULL; ii = ii->next)  
            UF_CALL(UF_DISP_set_highlight(ii->eid, TRUE));  
        UF_CALL(UF_MODL_ask_list_count(edge_list, &n_edges));  
        sprintf(msg, "Body has %d edges", n_edges);  
        uc1601(msg, TRUE);  
        for (ii = edge_list; ii != NULL; ii = ii->next)  
            UF_CALL(UF_DISP_set_highlight(ii->eid, FALSE));  
        UF_CALL(UF_MODL_delete_list(&edge_list));  
    }

```

#### 代码解析

> 这段代码实现了NX Open C++ API中高亮显示NX部件体边线功能。主要步骤包括：
>
> 1. 查询边线：通过UF_MODL_ask_body_edges函数获取指定部件体的所有边线，并将结果存储在edge_list链表中。
> 2. 高亮显示：遍历edge_list链表，调用UF_DISP_set_highlight函数将每条边线高亮显示。
> 3. 统计边线数量：通过UF_MODL_ask_list_count函数统计边线数量，并将结果显示在消息框中。
> 4. 取消高亮：再次遍历edge_list链表，调用UF_DISP_set_highlight函数取消每条边线的高亮显示。
> 5. 删除链表：通过UF_MODL_delete_list函数释放edge_list链表。
>
> 总体而言，这段代码实现了NX部件体边线的高亮显示，并统计了边线数量，最终取消了高亮显示。
>
