### 【0231】ask font table entity 查询字体表实体

#### 代码

```cpp
    static tag_t ask_font_table_entity(void)  
    {  
        tag_t  
            fte = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_font_table_type, &fte));  
        return (fte);  
    }

```

#### 代码解析

> 这是段NX Open二次开发代码，用于获取当前活动部件中的字体表实体。代码功能包括：
>
> 1. 使用UF_PART_ask_display_part函数获取当前活动部件的tag。
> 2. 定义一个tag_t类型的变量fte，用于存储字体表实体的tag，初始化为NULL_TAG。
> 3. 定义一个tag_t类型的变量part，用于存储当前活动部件的tag，通过调用UF_PART_ask_display_part函数获取。
> 4. 调用UF_OBJ_cycle_objs_in_part函数在当前活动部件中循环查找类型为UF_font_table_type的实体，并将找到的第一个字体表实体的tag存储到fte变量中。
> 5. 返回fte变量，即当前活动部件中的字体表实体的tag。
>
> 这段代码的主要作用是获取当前活动部件中的字体表实体，以供后续操作，如获取字体表中的字体信息等。
>
