### 【0302】ask next user defined symbol 询问下一个用户定义符号

#### 代码

```cpp
    static tag_t ask_next_user_defined_symbol(tag_t part, tag_t uds)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &uds)) && (uds != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(uds, &type, &subtype));  
            if (subtype == UF_draft_user_defined_subtype) return uds;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD系统中查询用户自定义符号的二次开发函数。
>
> 函数的主要逻辑如下：
>
> 1. 使用UF_OBJ_cycle_objs_in_part函数遍历指定部件(part)中的所有实体。
> 2. 检查遍历到的每个实体的类型和子类型，通过UF_OBJ_ask_type_and_subtype函数。
> 3. 如果实体的子类型是用户自定义类型(UF_draft_user_defined_subtype)，则返回该实体的标签(uds)。
> 4. 如果没有找到用户自定义符号，则返回NULL_TAG。
>
> 该函数的作用是遍历指定部件中的所有实体，并找出第一个用户自定义符号实体，返回其标签。在NX CAD系统中，用户可以自定义符号实体，这个函数可以用来查询这些自定义符号。
>
> 总的来说，这段代码通过遍历和类型检查，实现了在NX部件中查询用户自定义符号的功能，对于需要处理自定义符号的二次开发场景非常有用。
>
