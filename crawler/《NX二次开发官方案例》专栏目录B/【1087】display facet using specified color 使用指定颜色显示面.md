### 【1087】display facet using specified color 使用指定颜色显示面

#### 代码

```cpp
    static void display_temporary_colored_line(double start[3], double end[3],  
        UF_DISP_color_name_t color_name)  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        int color_index = 0;  
        UF_CALL(UF_DISP_ask_closest_color_in_displayed_part(color_name, &color_index));  
        props.color = color_index;  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
    }  
    static void display_facet_using_color(tag_t model, int facet,  
        UF_DISP_color_name_t color_name)  
    {  
        int  
            ii,  
            n;  
        double  
            verts[5][3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_FACET_ask_vertices_of_facet(model, facet, &n, verts));  
        for (ii = 0; ii < (n - 1); ii++)  
            display_temporary_colored_line(&verts[ii][0], &verts[ii+1][0], color_name);  
        display_temporary_colored_line(&verts[n-1][0], &verts[0][0], color_name);  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD系统中以特定颜色显示临时线条的二次开发代码。代码主要包含两个函数：
>
> 1. display_temporary_colored_line：用于显示两条线段之间的临时线条，并设置线条的颜色。它接受线条的起始和终止点坐标，以及颜色名称作为参数。通过调用UF_DISP_ask_closest_color_in_displayed_part函数获取与指定颜色名称最接近的颜色索引，然后调用UF_DISP_display_temporary_line函数在当前工作视图中显示线条，并设置线条的颜色属性。
> 2. display_facet_using_color：用于以特定颜色显示NX模型中的一个面。它接受模型标签、面的索引以及颜色名称作为参数。首先调用UF_FACET_ask_vertices_of_facet函数获取面的顶点坐标，然后遍历顶点坐标，调用display_temporary_colored_line函数显示临时线条，从而绘制出面。
>
> 通过这两个函数，可以方便地在NX中显示具有指定颜色的临时线条或面，从而进行可视化辅助设计。代码通过封装了NX的显示函数，提高了代码的复用性和可维护性。
>
