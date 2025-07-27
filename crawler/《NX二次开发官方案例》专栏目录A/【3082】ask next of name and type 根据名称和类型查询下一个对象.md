### 【3082】ask next of name and type 根据名称和类型查询下一个对象

#### 代码

```cpp
    static tag_t ask_next_of_name_and_type(tag_t part, char *name, int type,  
        tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, type, TRUE, &object));  
        return object;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API中的一部分，用于在NX部件对象中按照名称和类型查询下一个对象。具体来说：
>
