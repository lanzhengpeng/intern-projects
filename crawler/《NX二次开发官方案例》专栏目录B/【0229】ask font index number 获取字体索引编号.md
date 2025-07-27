### 【0229】ask font index number 获取字体索引编号

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
    static int ask_font_index_number(char *font_name)  
    {  
        int  
            index;  
        tag_t  
            fte = ask_font_table_entity();  
        UF_CALL(UF_UGFONT_add_font(fte, &index, font_name));  
        return (index);  
    }

```

#### 代码解析

> 这段代码是NX二次开发中与字体相关的两个函数，主要功能如下：
>
> 1. ask_font_table_entity函数：该函数用于获取NX中的字体表实体。通过调用UF_PART_ask_display_part获取显示部分的tag，然后循环查找该部分中的字体表实体，并返回字体表实体的tag。
> 2. ask_font_index_number函数：该函数用于获取指定字体名称在NX字体表中的索引号。首先调用ask_font_table_entity获取字体表实体，然后调用UF_UGFONT_add_font向字体表中添加指定名称的字体，并返回该字体在字体表中的索引号。
>
> 这两个函数的主要用途是获取NX中的字体表实体以及获取指定字体的索引号，为后续的字体设置等操作提供基础。
>
