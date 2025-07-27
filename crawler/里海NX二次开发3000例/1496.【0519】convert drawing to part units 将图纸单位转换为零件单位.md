### 【0519】convert drawing to part units 将图纸单位转换为零件单位

#### 代码

```cpp
    static void convert_drawing_to_part_units(tag_t member_view, double *loc)  
    {  
        int  
            part_units;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            sheet;  
        UF_DRAW_info_t  
            info;  
        UF_CALL(UF_PART_ask_units(part, &part_units));  
        UF_CALL(UF_DRAW_ask_drawing_of_view(member_view, &sheet));  
        UF_CALL(UF_DRAW_ask_drawing_info(sheet, &info));  
        if (part_units == info.units) return;  
        if (part_units == UF_PART_ENGLISH)  
            UF_VEC3_scale(1/25.4, loc, loc);  
        else  
            UF_VEC3_scale(25.4, loc, loc);  
    }

```

#### 代码解析

> 这段代码的主要功能是将视图中的绘图单位转换成零件的单位。具体步骤包括：
>
> 1. 获取当前零件的显示单位(part_units)。
> 2. 获取当前视图所在图纸(sheet)。
> 3. 获取图纸的绘图信息(info)。
> 4. 如果零件单位和图纸单位相同，则不需要转换，直接返回。
> 5. 如果零件单位是英制，则将视图坐标除以25.4进行转换。
> 6. 如果零件单位是公制，则将视图坐标乘以25.4进行转换。
> 7. 最后，通过UF_VEC3_scale函数将转换后的坐标赋值给loc参数。
>
> 总的来说，该函数实现了视图坐标从图纸单位到零件单位的转换，以确保在后续操作中能够正确地应用零件的单位。
>
