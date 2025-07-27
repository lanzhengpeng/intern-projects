### 【1126】display temporary point 展示临时点

#### 代码

```cpp
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，实现了在NX中显示一个临时点的功能。
>
> 具体来说，这段代码定义了一个名为display_temporary_point的静态函数，该函数接收一个指向双精度数组的指针coords，该数组包含临时点的坐标。
>
> 函数内部定义了一个UF_OBJ_disp_props_t类型的结构体变量attrib，它用于设置临时点的显示属性，如颜色、是否填充、线宽、字体样式等。
>
> 然后调用UF_DISP_display_temporary_point函数，该函数在NX中显示一个临时点。参数NULL_TAG表示使用默认的标签，UF_DISP_USE_ACTIVE_PLUS表示使用当前活动视图加上指定的视图，coords是临时点的坐标，attrib是临时点的显示属性，UF_DISP_POINT表示显示的是一个点。
>
> 综上所述，这段代码的作用是在NX中显示一个具有指定坐标和属性的临时点。通过设置不同的坐标和属性，可以实现不同的显示效果。
>
