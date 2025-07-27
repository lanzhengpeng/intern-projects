### 【1100】display temporary box 在临时坐标系中显示临时边界框，使用坐标系的方向向量计算边界框的8个顶点，并使用UF_DISP_display_temporary_line函数

#### 代码

```cpp
    static void map_point_to_csys(tag_t csys, double *pos)  
    {  
        int  
            irc;  
        tag_t  
            matrix;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys_mx[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12],  
            origin[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys_mx[3]));  
        FTN(uf5940)(abs_mx, csys_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%.3f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void display_temporary_box(double llc[3], double urc[3], tag_t csys)  
    {  
        double  
            dirs[9],  
            delta_x[3],  
            delta_y[3],  
            delta_z[3],  
            end[3],  
            mag,  
            sizes[3],  
            start[3];  
        tag_t  
            csys_mx;  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        WRITE3F(llc);  
        WRITE3F(urc);  
        UF_VEC3_copy(llc, start);  
        map_point_to_csys(csys, start);  
        UF_VEC3_copy(urc, end);  
        map_point_to_csys(csys, end);  
        UF_VEC3_sub(end, start, sizes);  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &csys_mx, dirs));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, dirs));  
        UF_VEC3_unitize(&dirs[0], 0, &mag, &dirs[0]);  
        UF_VEC3_unitize(&dirs[3], 0, &mag, &dirs[3]);  
        UF_VEC3_unitize(&dirs[6], 0, &mag, &dirs[6]);  
        UF_VEC3_scale(sizes[0], &dirs[0], delta_x);  
        UF_VEC3_scale(sizes[1], &dirs[3], delta_y);  
        UF_VEC3_scale(sizes[2], &dirs[6], delta_z);  
        UF_VEC3_add(llc, delta_x, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, end, &props));  
        UF_VEC3_add(llc, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, end, &props));  
        UF_VEC3_add(llc, delta_z, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, end, &props));  
        UF_VEC3_sub(urc, delta_x, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, end, &props));  
        UF_VEC3_sub(urc, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, end, &props));  
        UF_VEC3_sub(urc, delta_z, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, end, &props));  
        UF_VEC3_add(llc, delta_x, start);  
        UF_VEC3_add(start, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_add(start, delta_z, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_sub(end, delta_x, start);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_add(start, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_sub(end, delta_z, start);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_add(start, delta_x, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
    }

```

#### 代码解析

> 这段代码是NX Open API开发的二次开发示例，主要功能包括：
>
> 1. 定义了一个名为map_point_to_csys的函数，用于将一个点从当前坐标系转换到指定的坐标系。它通过获取指定坐标系的矩阵，并将输入点坐标转换到该坐标系下。
> 2. 定义了一个名为write_double_array_to_listing_window的函数，用于在NX的输出窗口中格式化打印一个双精度数组。
> 3. 定义了一个名为display_temporary_box的函数，用于在NX图形窗口中显示一个临时框。它首先将框的8个顶点转换到当前坐标系下，然后绘制12条线段构成框的边框。
> 4. 在display_temporary_box函数中，使用了NX的UF_DISP_display_temporary_line API来绘制临时线段，线段颜色为白色，宽度为正常，并设置不进行隐藏线处理。
> 5. 通过调用UF_VEC3_copy、UF_VEC3_sub等向量运算函数，实现了顶点坐标的计算。
> 6. 在打印数组时，使用了宏WRITE3F来简化代码。
> 7. 整个示例代码结构清晰，各部分功能明确，通过调用NX Open API实现了一个坐标系转换和临时框显示的功能。
>
