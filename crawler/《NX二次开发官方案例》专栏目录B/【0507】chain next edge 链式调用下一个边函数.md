### 【0507】chain next edge 链式调用下一个边函数

#### 代码

```cpp
    static void chain_next_edge(tag_t edge, double pos[3])  
    {  
        int  
            cnt;  
        tag_t  
            point;  
        uf_list_p_t  
            last,  
            next;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_ask_display_properties(edge, &disp_props));  
        if (!disp_props.highlight_status)  
        {  
            UF_CALL(UF_DISP_set_highlight(edge, TRUE));  
            UF_CALL(UF_MODL_ask_cnnc_edges(edge, pos, &next, &last));  
            if (next)  
            {  
                UF_CALL(UF_MODL_ask_list_count(next, &cnt));  
                if (cnt)  
                {  
                    UF_CALL(UF_MODL_ask_list_item(next, 0, &edge));  
                    ask_edge_far_endpoint(edge, pos);  
                    chain_next_edge(edge, pos);  
                }  
                UF_free(next);  
                UF_free(last);  
            }  
        }  
    }

```

#### 代码解析

> 这段NX Open C++代码定义了一个名为chain_next_edge的静态函数，其功能是递归地遍历指定边连接的边链，直到没有连接的边为止。
>
> 函数的参数包括：
>
> 主要步骤如下：
>
> 1. 获取当前边的显示属性，如果当前边没有高亮显示，则设置其高亮显示。
> 2. 获取当前边连接的边链，包括下一个边和最后一个边。
> 3. 如果存在下一个边，则遍历下一个边的边链。
> 4. 遍历结束后，释放内存。
> 5. 返回递归调用结果。
>
> 通过递归调用，该函数可以遍历整个边链，并获取每个边的远端点坐标。
>
