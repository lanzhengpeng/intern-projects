### 【0274】ask next id symbol 询问下一个ID符号

#### 代码

```cpp
    static tag_t ask_next_id_symbol(tag_t part, tag_t id_symbol)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &id_symbol)) && (id_symbol != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(id_symbol, &type, &subtype));  
            if (subtype == UF_draft_id_symbol_subtype) return id_symbol;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，其主要功能是在NX部件中遍历草绘实体，并找到下一个ID符号。
>
> 主要步骤包括：
>
> 1. 定义了ask_next_id_symbol函数，输入参数是部件part和当前ID符号id_symbol。
> 2. 使用UF_OBJ_cycle_objs_in_part函数遍历部件中的草绘实体。
> 3. 在循环中，使用UF_OBJ_ask_type_and_subtype函数检查当前实体的类型和子类型。
> 4. 如果实体的子类型是UF_draft_id_symbol_subtype，则表示找到了ID符号，返回该ID符号。
> 5. 如果没有找到下一个ID符号，则返回NULL_TAG。
>
> 总之，该代码实现了在NX部件中遍历草绘实体，并找到下一个ID符号的功能。
>
