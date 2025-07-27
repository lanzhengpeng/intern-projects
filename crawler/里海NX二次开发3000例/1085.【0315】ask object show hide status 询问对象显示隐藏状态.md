### 【0315】ask object show hide status 询问对象显示隐藏状态

#### 代码

```cpp
    static logical isHidden(tag_t object)  
    {  
        logical  
            is_disp;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(object, &is_disp));  
        if (!is_disp) return TRUE;  // its not displayable so its not visible  
        UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
        if (disp_props.blank_status == UF_OBJ_BLANKED) return TRUE;  
        return FALSE;  
    }

```

#### 代码解析

> 这段代码用于判断NX中的对象是否为隐藏状态。主要逻辑如下：
>
> 1. 首先调用UF_OBJ_is_displayable函数，判断对象是否为可显示状态。如果不是可显示的，则直接返回true，表示对象是隐藏的。
> 2. 如果对象是可显示的，则调用UF_OBJ_ask_display_properties函数，获取对象的显示属性。
> 3. 检查显示属性中的blank_status字段，如果等于UF_OBJ_BLANKED，表示对象被隐藏了，返回true。
> 4. 如果对象是可显示的，并且blank_status不是UF_OBJ_BLANKED，则表示对象是可见的，返回false。
>
> 综上所述，这段代码通过判断对象的可显示状态和显示属性，实现了判断对象是否为隐藏状态的功能。
>
