### 【1092】display notes origin and text box 显示注释的原始位置和文本框

#### 代码

```cpp
    static void map_point_from_matrix(tag_t matrix, double *pos)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12];  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
    static void display_temporary_box(double llc[3], double urc[3], tag_t matrix)  
    {  
        double  
            ulc[3],  
            lrc[3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        ulc[0] = llc[0];  
        ulc[1] = urc[1];  
        ulc[2] = llc[2];  
        lrc[0] = urc[0];  
        lrc[1] = llc[1];  
        lrc[2] = llc[2];  
        map_point_from_matrix(matrix, llc);  
        map_point_from_matrix(matrix, lrc);  
        map_point_from_matrix(matrix, urc);  
        map_point_from_matrix(matrix, ulc);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, lrc, &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            lrc, urc, &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, ulc, &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            ulc, llc, &props));  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void display_notes_origin_and_text_box(tag_t a_note)  
    {  
        int  
            resp;  
        tag_t  
            matrix;  
        double  
            llc[3] = { 0,0,0 },  
            origin[3],  
            urc[3] = { 0,0,0 };  
        FTN(uf5552)(&a_note, origin, &resp);  
        display_temporary_point(origin);  
        FTN(uf5575)(&a_note, llc, urc, &resp);  
        UF_CALL(UF_CSYS_ask_matrix_of_object(a_note, &matrix));  
        display_temporary_box(llc, urc, matrix);  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD软件中显示注释的原始位置和文本框的二次开发代码。主要功能包括：
>
> 1. map_point_from_matrix函数：将注释的局部坐标转换到全局坐标。首先，获取注释的坐标系矩阵，然后调用NX提供的Fortran接口函数将局部坐标转换到全局坐标。
> 2. display_temporary_box函数：在NX中显示注释的文本框。首先，根据注释的局部坐标计算文本框的四个角点坐标，然后调用NX提供的Fortran接口函数在NX中显示临时线条来构成文本框的轮廓。
> 3. display_temporary_point函数：在NX中显示注释的原始位置。通过调用NX提供的Fortran接口函数在NX中显示一个临时点来表示注释的原始位置。
> 4. display_notes_origin_and_text_box函数：主函数，用于显示注释的原始位置和文本框。首先获取注释的原始位置坐标，然后显示临时点。接着获取文本框的局部坐标范围，显示文本框。最后，获取注释的坐标系矩阵，并显示文本框。
>
> 综上所述，这段代码通过调用NX提供的Fortran接口函数，实现了在NX中显示注释的原始位置和文本框的功能，以辅助用户查看注释的详细信息。
>
