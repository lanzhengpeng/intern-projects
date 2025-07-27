### 【0215】ask feat first circular edge 获取特征的首个圆形边

#### 代码

```cpp
    static tag_t ask_feat_first_circular_edge(tag_t feat)  
    {  
        int  
            type;  
        tag_t  
            edge = NULL_TAG;  
        uf_list_p_t  
            edge_list,  
            temp;  
        UF_CALL(UF_MODL_ask_feat_edges(feat, &edge_list));  
        for (temp = edge_list; temp != NULL; temp = temp->next)  
        {  
            UF_CALL(UF_MODL_ask_edge_type(temp->eid, &type));  
            if (type == UF_MODL_CIRCULAR_EDGE)  
            {  
                edge = temp->eid;  
                break;  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&edge_list));  
        return edge;  
    }

```

#### 代码解析

> 这段代码是一个用于NX二次开发的函数，其主要功能是查询给定特征（feat）的第一个圆形边。具体步骤如下：
>
> 1. 首先定义了函数ask_feat_first_circular_edge，该函数接收一个特征标签feat作为输入参数。
> 2. 在函数内部，首先定义了变量edge来存储找到的圆形边，并初始化为NULL_TAG。
> 3. 使用UF_MODL_ask_feat_edges函数查询输入特征的所有边，并将结果存储在链表edge_list中。
> 4. 遍历链表edge_list，使用UF_MODL_ask_edge_type函数查询每条边的类型。
> 5. 如果边的类型为UF_MODL_CIRCULAR_EDGE（圆形边），则将边标签存储在变量edge中，并结束循环。
> 6. 使用UF_MODL_delete_list函数删除链表edge_list。
> 7. 函数返回找到的圆形边标签edge。
>
> 综上所述，该函数的功能是查询输入特征的所有边，找到第一个类型为圆形边的边，并返回该边的标签。
>
