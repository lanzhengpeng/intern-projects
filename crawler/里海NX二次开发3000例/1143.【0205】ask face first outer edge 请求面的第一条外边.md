### 【0205】ask face first outer edge 请求面的第一条外边

#### 代码

```cpp
    static tag_t ask_face_first_outer_edge(tag_t face)  
    {  
        tag_t  
            edge;  
        uf_loop_p_t  
            loop_list,  
            temp;  
        if (!UF_CALL(UF_MODL_ask_face_loops(face, &loop_list)) && loop_list)  
        {  
            for (temp = loop_list; temp != NULL; temp = temp->next)  
                if (temp->type == 1)  
                {  
                    edge = temp->edge_list->eid;  
                    break;  
                }  
            UF_CALL(UF_MODL_delete_loop_list(&loop_list));  
            return edge;  
        }  
        else  
            return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API的二次开发代码，其主要功能是从给定的面中找到第一个外环的边。代码的逻辑如下：
>
> 1. 首先，调用UF_MODL_ask_face_loops函数，获取给定面的环列表。
> 2. 遍历环列表，找到第一个类型为1的外环。
> 3. 获取该外环中的第一个边。
> 4. 删除环列表。
> 5. 返回找到的边。
>
> 该代码实现了从面中获取第一个外环的边的功能。
>
