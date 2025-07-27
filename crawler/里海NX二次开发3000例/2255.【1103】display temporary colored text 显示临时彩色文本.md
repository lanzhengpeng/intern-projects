### 【1103】display temporary colored text 显示临时彩色文本

#### 代码

```cpp
    static void display_temporary_colored_text(char *text, double loc[3], int color)  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        props.color = color;  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_ACTIVE_MINUS,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }

```

#### 代码解析

> 这段代码是用于在NX中显示临时彩色文本的函数。
>
> 主要步骤包括：
>
> 1. 定义一个UF_OBJ_disp_props_t结构体，用于设置文本的显示属性，包括是否可见、颜色、是否被遮盖、线宽、字体样式和是否反相显示。
> 2. 设置文本的颜色属性，这里通过结构体成员props.color来设置。
> 3. 调用UF_DISP_display_temporary_text函数，用于在指定位置显示临时文本。参数包括：
>
> 这个函数通过设置文本的颜色属性，并在指定位置显示临时文本，实现了在NX中显示彩色文本的功能。
>
