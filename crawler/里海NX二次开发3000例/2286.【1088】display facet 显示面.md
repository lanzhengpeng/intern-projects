### 【1088】display facet 显示面

#### 代码

```cpp
    static void display_facet(tag_t model, int facet)  
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
            UF_CALL(UF_DISP_display_temporary_line(NULL_TAG,  
                UF_DISP_USE_WORK_VIEW, &verts[ii][0], &verts[ii+1][0], &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG,  
            UF_DISP_USE_WORK_VIEW, &verts[n-1][0], &verts[0][0], &props));  
    }

```

#### 代码解析

> 这段代码是用于在NX中显示一个面的顶点线框的二次开发代码。代码的主要功能包括：
>
> 1. 定义了一个静态函数display_facet，接受模型tag和面的tag作为参数。
> 2. 使用UF_FACET_ask_vertices_of_facet函数查询指定面的顶点坐标，并保存在verts数组中。
> 3. 遍历顶点数组，使用UF_DISP_display_temporary_line函数绘制线段，连接相邻顶点，形成一个闭合的线框。
> 4. 最后连接最后一个顶点到第一个顶点，形成一个完整的面线框。
> 5. 线框的显示属性由props结构体定义，包括颜色、线宽、字体等。
>
> 总体来说，这段代码通过遍历面的顶点，调用NX提供的显示函数，在NX视口中绘制了面的线框模型，并支持设置线框的显示属性。通过在NX二次开发中使用该函数，可以方便地显示和调试NX中的面模型。
>
