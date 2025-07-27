### 【0258】ask next body 查询实体的类型和子类型，查询下一个实体

#### 代码

```cpp
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发，其目的是在NX部件中遍历并获取下一个实体的主体（body）。
>
> 代码的主要逻辑如下：
>
> 1. 定义了一个名为ask_next_body的静态函数，接受两个参数：part（部件）和body（实体）。
> 2. 使用循环结构遍历部件part中的所有实体，直到找到下一个实体主体（body）。
> 3. 在循环中，使用UF_OBJ_cycle_objs_in_part函数遍历部件中的实体，并使用UF_OBJ_ask_type_and_subtype函数获取每个实体的类型和子类型。
> 4. 判断实体的子类型是否为实体主体（UF_solid_body_subtype），如果是，则返回该实体主体。
> 5. 如果遍历完所有实体后仍未找到下一个实体主体，则返回NULL_TAG。
> 6. 函数返回找到的下一个实体主体，如果没有找到，则返回NULL_TAG。
>
> 总体来说，这段代码的作用是遍历NX部件中的实体，并返回下一个实体主体。它通过UF_OBJ提供的函数进行NX实体遍历和查询，实现了实体的遍历查找功能。
>
