### 【1099】display temporary asterisk 显示临时星号

#### 代码

```cpp
    static void display_temporary_asterisk(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_ASTERISK));  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API的二次开发代码，其主要功能是在NX中显示一个临时的星号标记。具体来说：
>
> 1. display_temporary_asterisk是一个静态函数，接受一个指向双精度数组的指针coords，该数组包含星号标记的坐标。
> 2. 定义了一个UF_OBJ_disp_props_t类型的结构体attrib，用于设置星号标记的显示属性，包括颜色、线型、是否被遮挡、线宽、字体、是否可见等。
> 3. 调用UF_DISP_display_temporary_point函数，在NX中显示一个临时的星号标记。其中，NULL_TAG表示不使用标签，UF_DISP_USE_ACTIVE_PLUS表示使用当前激活的视图加，coords指定了星号的位置，&attrib指定了星号的显示属性，UF_DISP_ASTERISK表示显示类型为星号。
> 4. UF_CALL宏用于调用NX的API函数，并检查返回码，如果出错则抛出异常。
>
> 综上所述，这段代码的主要作用是接受一个坐标数组，然后在NX中显示一个具有指定属性的临时星号标记，以辅助用户进行定位或标注。
>
