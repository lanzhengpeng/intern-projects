### 【0275】ask next label 查询下一个标签

#### 代码

```cpp
    static tag_t ask_next_label(tag_t part, tag_t label)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &label)) && (label != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(label, &type, &subtype));  
            if (subtype == UF_draft_label_subtype) return label;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open API代码，其主要功能是在指定的零件(part)中查找下一个标签(label)，并返回其标签号。
>
> 代码的主要逻辑如下：
>
> 1. 使用UF_CALL宏调用UF_OBJ_cycle_objs_in_part函数，在指定的零件中遍历所有实体。
> 2. 在遍历过程中，使用UF_OBJ_ask_type_and_subtype函数检查每个实体的类型和子类型。
> 3. 如果实体的子类型是UF_draft_label_subtype（表示这是一个标签），则返回该实体的标签号。
> 4. 如果遍历完所有实体后没有找到符合条件的标签，则返回NULL_TAG。
> 5. 使用UF_CALL宏是为了简化错误处理，它会自动检查函数返回值，并在出错时打印错误信息。
> 6. 该函数的参数part表示要遍历的零件，label表示要查找的标签号。如果label为NULL_TAG，表示从第一个实体开始查找。
> 7. 该函数返回找到的下一个标签号，如果没有找到，则返回NULL_TAG。
>
> 这段代码的主要作用是遍历零件中的所有实体，查找下一个标签，并返回其标签号。它可以用于各种需要遍历和查找标签的场景。
>
