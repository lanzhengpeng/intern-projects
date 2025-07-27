### 【2868】show curve ends 显示曲线端点

#### 代码

```cpp
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            limits[2];  
        UF_EVAL_p_t  
            evaluator;  
        UF_CALL(UF_EVAL_initialize(curve, &evaluator));  
        UF_CALL(UF_EVAL_ask_limits(evaluator, limits));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[0], start, NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[1], end, NULL));  
        UF_CALL(UF_EVAL_free(evaluator));  
    }  
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void show_curve_ends(tag_t curve)  
    {  
        double  
            start[3],  
            end[3];  
        ask_curve_ends(curve, start, end);  
        display_temporary_colored_point(start, UF_OBJ_GREEN);  
        display_temporary_colored_point(end, UF_OBJ_RED);  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发函数，主要用于展示曲线的起点和终点。
>
> 主要功能包括：
>
> 1. ask_curve_ends函数：用于获取曲线的起点和终点坐标。
> 2. display_temporary_colored_point函数：用于在图形窗口中显示一个临时点，并设置点的颜色。
> 3. show_curve_ends函数：首先调用ask_curve_ends获取曲线的起点和终点坐标，然后调用display_temporary_colored_point函数分别在起点和终点显示绿色的点和红色的点。
> 4. 使用UF_EVAL相关的函数获取曲线的起点和终点。
> 5. 使用UF_DISP相关的函数显示临时点。
> 6. 通过设置不同的颜色来区分起点和终点。
> 7. 使用了UF_OBJ_DISP_PROPS_T结构体来设置点的显示属性，例如颜色、线宽等。
> 8. 调用了UF_DISP_display_temporary_point函数在图形窗口中显示临时点。
> 9. 整体代码结构清晰，实现了展示曲线起点和终点的功能。
>
> 通过这段代码，可以直观地看到NX模型中曲线的起点和终点，对于曲线的分析和理解非常有帮助。
>
