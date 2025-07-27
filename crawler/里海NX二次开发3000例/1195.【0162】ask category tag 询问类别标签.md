### 【0162】ask category tag 询问类别标签

#### 代码

```cpp
    static tag_t ask_category_tag(char *name)  
    {  
    /*  Prior to V18 - do this instead  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, UF_layer_category_type,  
            FALSE, &object));  
        return object;  
    */  
        tag_t  
            object;  
        UF_CALL(UF_LAYER_ask_category_tag(name, &object));  
        return object;  
    }

```

#### 代码解析

> 这段代码用于在NX中查询指定名称的层类别标签。在NX18版本之前，查询需要通过遍历零件的显示部件来找到对应的层类别标签。而在NX18及之后的版本中，可以直接使用UF_LAYER_ask_category_tag函数来查询。以下是代码的详细解释：
>
> 1. ask_category_tag函数：这是一个静态函数，用于查询指定名称的层类别标签，并返回该标签。
> 2. 在NX18版本之前的查询方式：使用UF_OBJ_cycle_by_name_and_type函数，遍历零件的显示部件，根据名称和类型(层类别类型)来查询层类别标签。查询结果存储在object变量中，并返回该变量。
> 3. 在NX18及之后的查询方式：使用UF_LAYER_ask_category_tag函数，直接根据名称查询层类别标签，并将查询结果存储在object变量中，然后返回该变量。
> 4. 注意事项：在调用UF函数之前，需要确保NX会话已经打开。此外，在NX18版本之前，需要先获取零件的显示部件(part)，而NX18及之后的版本则不再需要。
>
> 综上所述，这段代码展示了在不同NX版本中查询层类别标签的不同方式，并体现了NX二次开发的一些基本知识。
>
