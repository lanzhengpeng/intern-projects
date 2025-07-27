### ask next solid body 查询下一个实体

#### 代码

```cpp
    static tag_t ask_next_solid_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
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

> 这段代码是用于在NX中遍历组件并查询实体的类型，以找到下一个实体的代码。以下是代码的主要步骤：
>
> 1. 使用UF_OBJ_cycle_objs_in_part函数遍历指定组件中的所有实体。
> 2. 检查每个实体的类型和子类型。当实体是实体体时，即其子类型为UF_solid_body_subtype。
> 3. 使用UF_MODL_ask_body_type函数进一步检查实体体是否为实体类型。
> 4. 如果找到下一个实体体，则返回其tag。
> 5. 如果没有找到，则返回NULL_TAG。
>
> 这段代码的作用是在NX中遍历组件，查询并返回下一个实体体的tag。
>
