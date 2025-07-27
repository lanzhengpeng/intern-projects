### 【0291】ask next promotion 查询下一个提升实体

#### 代码

```cpp
    static tag_t ask_next_promotion(tag_t part, tag_t promotion)  
    {  
        int  
            subtype,  
            type;  
        while ((promotion = UF_OBJ_cycle_all(part, promotion)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(promotion, &type, &subtype));  
            if ((type == UF_solid_type) &&  
                (subtype == UF_solid_body_subtype) &&  
                (UF_OBJ_is_object_a_promotion(promotion))) return promotion;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发的代码片段，其主要功能是在NX模型中遍历给定组件的下一级推广零件。具体来说：
>
> 1. 函数ask_next_promotion接受两个输入参数：part表示当前组件，promotion表示当前组件的下一级推广零件。
> 2. 使用UF_OBJ_cycle_all函数遍历part组件的所有下一级推广零件，直到遍历完毕。
> 3. 在遍历过程中，通过UF_OBJ_ask_type_and_subtype函数判断每个推广零件的类型和子类型，如果类型为实体(UF_solid_type)且子类型为实体主体(UF_solid_body_subtype)，并且该零件是一个推广(UF_OBJ_is_object_a_promotion判断)，则返回该零件作为下一级推广零件。
> 4. 如果没有找到满足条件的下一级推广零件，则返回NULL_TAG。
> 5. 该函数主要用于在NX模型中遍历给定组件的下一级推广零件，通常用于获取组件的下一级实体零件。
> 6. 整体逻辑清晰，通过循环遍历和类型判断，实现了在NX模型中查找下一级推广零件的功能。
>
