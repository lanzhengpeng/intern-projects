### 【0204】ask face first edge 询问面的第一个边

#### 代码

```cpp
    static tag_t ask_face_first_edge(tag_t face)  
    {  
        int  
            n;  
        tag_t  
            edge;  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_ask_face_edges(face, &edge_list));  
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

> 这段代码是用于NX二次开发，旨在获取给定面的第一条边。具体来说，代码执行以下步骤：
>
> 1. 定义所需的函数和数据类型。
> 2. 调用UF_MODL_ask_face_edges获取给定面的边列表。
> 3. 获取边列表的长度。
> 4. 如果边列表不为空，则获取第一条边。
> 5. 删除边列表。
> 6. 如果边列表为空，则返回NULL_TAG。
> 7. 如果边列表不为空，则返回第一条边的标签。
>
> 总体来说，这段代码的作用是获取NX模型中给定面的第一条边的标签，并在获取完成后清理使用的内存。
>
