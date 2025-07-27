### 【1127】display temporary rectangle 显示临时矩形

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
    static void display_temporary_rectangle(double llc[3], double urc[3], tag_t matrix)  
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

```

#### 代码解析

> 这段代码用于在NX CAD软件中显示一个临时矩形。
>
> 主要功能包括：
>
> 1. map_point_from_matrix函数：该函数将一个点从给定的坐标系转换到绝对坐标系。它首先获取给定坐标系的矩阵值，然后调用Fortran函数将点从给定坐标系转换到绝对坐标系。
> 2. display_temporary_rectangle函数：该函数显示一个临时矩形。它首先计算出矩形的四个顶点，然后调用NX的UF_DISP_display_temporary_line函数在NX界面中显示这四个顶点连接成的矩形。
> 3. 临时矩形是通过四条线段连接起来的，颜色为白色，线宽为正常。
> 4. 调用map_point_from_matrix函数将矩形顶点从给定坐标系转换到绝对坐标系，以便在NX界面中正确显示。
> 5. 通过UF_DISP_display_temporary_line函数在NX界面中显示矩形线段，线段仅在当前NX会话中临时显示，不会保存到NX文件中。
> 6. 该代码主要用于NX二次开发，可以方便地在NX界面中绘制临时几何图形。
>
