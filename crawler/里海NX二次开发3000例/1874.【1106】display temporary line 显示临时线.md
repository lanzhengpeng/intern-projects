### 【1106】display temporary line 显示临时线

#### 代码

```cpp
    static void display_temporary_line(double start[3], double end[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
    }

```

#### 代码解析

> 这段代码是一个用于在NX中绘制临时线的函数。其基本功能包括以下几点：
>
> 1. 函数名为display_temporary_line，接受两个参数start和end，分别表示线的起点和终点坐标。
> 2. 定义了一个结构体props，用于设置临时线的显示属性，包括颜色、是否填充、线宽、字体样式等。
> 3. 调用UF_DISP_display_temporary_line函数，传入起点和终点坐标，以及props属性，绘制出一条临时线。
> 4. 临时线会在NX的图形窗口中显示，但不会保存到模型中，会在下一次交互或视图刷新时消失。
> 5. UF_DISP_USE_ACTIVE_PLUS表示临时线会在当前激活视图和所有平行视图上显示。
> 6. NULL_TAG表示临时线没有标签，即不会在图形窗口中显示标签。
> 7. 通过设置props结构体，可以控制临时线的显示效果，例如颜色、线宽等。
> 8. UF_CALL是NX提供的宏，用于调用UF函数，并处理错误。
>
> 总的来说，这个函数提供了在NX中绘制临时线的简单方法，适用于NX二次开发中的可视化需求。
>
