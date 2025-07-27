### 【1102】display temporary colored point 显示临时彩色点

#### 代码

```cpp
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }

```

#### 代码解析

> 这段代码是NX Open API中的二次开发代码，主要实现了在NX视图中显示临时彩色点的功能。具体代码逻辑如下：
>
> 1. 定义了一个结构体UF_OBJ_disp_props_t，用于设置点的显示属性，包括是否可见、颜色、是否隐藏、线宽、字体样式和是否显示标签等。
> 2. 初始化结构体attrib，设置点可见、颜色为白色、不隐藏、线宽正常、字体为实线、不显示标签。
> 3. 将传入的颜色值color赋值给attrib的color属性，以设置点的颜色。
> 4. 调用UF_DISP_display_temporary_point函数，在当前活动视图(或指定的视图)中显示一个临时点，坐标由传入的coords参数指定，点的显示属性由attrib指定，点的类型为普通点。
> 5. 该函数调用后会立即显示该点，但不会将其添加到模型树中，仅作为临时显示。
> 6. 临时点在视图更新后(如缩放、平移等)会自动消失。
>
> 综上所述，这段代码实现了在NX视图中根据指定坐标和颜色显示临时点的功能，适用于二次开发中的临时高亮显示、定位等场景。
>
