### 【0267】ask next edge 查询下一个边

#### 代码

```cpp
    static tag_t ask_next_edge(tag_t part, tag_t edge)  
    {  
        int  
            subtype,  
            type;  
        while ((!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &edge)))  
            && (edge != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(edge, &type, &subtype));  
            if (subtype == UF_solid_edge_subtype) return edge;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这是段NX Open C++ API的代码，用于遍历NX部件中的边，并找到下一个边实体。
>
> 主要思路如下：
>
> 1. 使用UF_OBJ_cycle_objs_in_part()函数遍历指定部件中的实体。该函数会返回部件中的下一个实体，直到遍历完所有实体。
> 2. 判断返回的实体是否为NULL_TAG，如果是，表示已遍历完所有实体，返回NULL_TAG结束。
> 3. 如果实体不为NULL_TAG，使用UF_OBJ_ask_type_and_subtype()函数获取实体的类型和子类型。
> 4. 如果实体的子类型为UF_solid_edge_subtype，表示这是一个边实体，返回该边实体的tag。
> 5. 如果实体的子类型不是边实体，继续遍历下一个实体。
> 6. 当找到下一个边实体时，返回该边实体的tag。
> 7. 如果遍历完所有实体后没有找到边实体，最终返回NULL_TAG。
>
> 总的来说，这段代码实现了遍历NX部件中的边，并按顺序返回下一个边实体的功能。
>
