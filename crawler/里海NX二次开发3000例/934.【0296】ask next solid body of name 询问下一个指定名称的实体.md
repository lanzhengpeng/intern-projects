### 【0296】ask next solid body of name 询问下一个指定名称的实体

#### 代码

```cpp
    static tag_t ask_next_solid_body_of_name(tag_t part, char *name, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, UF_solid_type,  
            TRUE, &body)) && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SOLID_BODY) return body;  
            }  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于在NX模型中搜索具有特定名称的实体的下一个实体的函数。
>
> 主要功能包括：
>
> 1. 参数：函数接受三个参数，分别是父实体(part)、要搜索的实体名称(name)和当前实体(body)。
> 2. 遍历实体：通过UF_OBJ_cycle_by_name_and_type函数以名称和类型遍历父实体(part)下的所有实体。这里指定类型为UF_solid_type，即只遍历实体类型。
> 3. 实体类型判断：在遍历过程中，通过UF_OBJ_ask_type_and_subtype函数获取每个实体的类型和子类型。如果子类型为UF_solid_body_subtype，即实体是实体类型，则通过UF_MODL_ask_body_type函数确认其类型是否为UF_MODL_SOLID_BODY。
> 4. 返回结果：如果找到满足条件的实体，则返回该实体；否则，在遍历结束后返回NULL_TAG。
> 5. 主要逻辑：该函数的主要逻辑是遍历父实体下的所有实体，直到找到下一个名称匹配且类型为实体的实体。
>
> 总的来说，该函数通过遍历父实体下的所有实体，来找到具有特定名称的下一个实体，并返回该实体的tag。
>
