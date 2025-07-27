### 【1101】display temporary colored line 显示临时彩色线

#### 代码

```cpp
    static void display_temporary_colored_line(double start[3], double end[3], UF_DISP_color_name_t color_name)  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        int color_index = 0;  
        UF_CALL(UF_DISP_ask_closest_color_in_displayed_part(color_name, &color_index));  
        props.color = color_index;  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_MINUS,  
            start, end, &props));  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，目的是在NX中显示一条临时的彩色线。
>
> 具体来说，这段代码定义了一个名为display_temporary_colored_line的函数，用于在NX中显示一条从起点start到终点end的临时线段，并指定颜色为color_name。
>
> 函数内部首先初始化一个UF_OBJ_disp_props_t结构体，用于设置线的显示属性，包括颜色、线宽、线型等。然后，通过调用UF_DISP_ask_closest_color_in_displayed_part函数，获取与color_name最接近的颜色索引，并设置到结构体中。最后，调用UF_DISP_display_temporary_line函数，根据传入的起点、终点和显示属性，在NX中显示一条临时的线段。
>
> 通过这个函数，用户可以方便地在NX中绘制出指定颜色的临时线段，用于标注或辅助查看模型。
>
