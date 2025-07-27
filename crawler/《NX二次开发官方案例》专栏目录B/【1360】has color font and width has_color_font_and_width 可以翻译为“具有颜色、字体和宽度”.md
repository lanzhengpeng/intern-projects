### 【1360】has color font and width has_color_font_and_width 可以翻译为“具有颜色、字体和宽度”

#### 代码

```cpp
    static logical has_color_font_and_width(tag_t object)  
    {  
        logical  
            is_displayable;  
        int  
            subtype,  
            type;  
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayable));  
        if (!is_displayable) return FALSE;  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        if (((type == UF_solid_type) && (subtype == UF_solid_edge_subtype))  
            || (type == UF_feature_type)) return FALSE;  
        return TRUE;  
    }

```

#### 代码解析

> 这段NX二次开发代码定义了一个名为has_color_font_and_width的静态逻辑函数，用于检查NX对象是否具有颜色、字体和线宽属性。以下是该函数的详细描述：
>
> 1. 函数接收一个NX对象作为输入参数。
> 2. 首先，使用UF_OBJ_is_displayable函数检查对象是否可显示。若对象不可显示，则返回FALSE。
> 3. 然后，使用UF_OBJ_ask_type_and_subtype函数获取对象的类型和子类型。
> 4. 接着，对对象类型和子类型进行判断：如果对象是实体边，则返回FALSE。如果对象是特征，则返回FALSE。其他类型对象返回TRUE。
> 5. 如果对象是实体边，则返回FALSE。
> 6. 如果对象是特征，则返回FALSE。
> 7. 其他类型对象返回TRUE。
> 8. 最终，根据对象类型和子类型的判断结果，返回TRUE或FALSE，表示对象是否具有颜色、字体和线宽属性。
>
> 综上，该函数的作用是检查NX对象是否具有颜色、字体和线宽属性，除了实体边和特征对象。
>
