### 【0305】ask next visible solid 查询下一个可见的实体

#### 代码

```cpp
    static tag_t ask_next_visible_solid(tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_VIEW_cycle_objects(NULL_TAG, UF_VIEW_VISIBLE_OBJECTS,  
                &body)) && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype))  
                return body;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的二次开发代码，用于遍历当前视图中的所有可见实体，并返回下一个可见的实体。
>
> 主要功能包括：
>
> 1. 使用UF_VIEW_cycle_objects()函数遍历视图中的所有可见实体。
> 2. 使用UF_OBJ_ask_type_and_subtype()函数获取每个实体的类型和子类型。
> 3. 判断实体是否为实体类型(typeUF_solid_type)且子类型为实体体(subtypeUF_solid_body_subtype)。
> 4. 如果满足条件，则返回该实体的tag。
> 5. 如果遍历完所有实体后没有找到符合条件的实体，则返回NULL_TAG。
>
> 这段代码的作用是获取当前视图中的下一个可见实体，并判断其是否为实体类型。如果找到符合条件的实体，则返回其tag。如果遍历完所有实体后没有找到符合条件的实体，则返回NULL_TAG。
>
