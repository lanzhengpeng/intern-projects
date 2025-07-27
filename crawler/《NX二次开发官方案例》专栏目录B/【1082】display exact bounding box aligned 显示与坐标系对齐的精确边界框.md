### 【1082】display exact bounding box aligned 显示与坐标系对齐的精确边界框

#### 代码

```cpp
    static void map_abs2csys(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
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
        if (csys == NULL_TAG)  
            UF_CALL(UF_CSYS_ask_wcs(&csys));  
        UF_VEC3_copy(llc, start);  
        map_abs2csys(csys, start);  
        UF_VEC3_copy(urc, end);  
        map_abs2csys(csys, end);  
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
    static void display_exact_bounding_box_aligned(tag_t object, tag_t csys)  
    {  
        int  
            ii,jj;  
        double  
            min[3],  
            dir[3][3],  
            dist[3],  
            max[3];  
        UF_CALL(UF_MODL_ask_bounding_box_exact(object, csys, min, dir, dist ));  
        for (ii = 0; ii < 3; ii++)  
        {  
            max[ii] = min[ii];  
            for (jj = 0; jj < 3; jj++)  
                max[ii] += dir[jj][ii] * dist[jj];  
        }  
        display_temporary_box( min, max, csys);  
    }

```

#### 代码解析

> 这段代码是用于在NX中显示精确对齐的边界框的二次开发代码。主要功能如下：
>
> 1. map_abs2csys函数：该函数将绝对坐标系下的点转换到指定的坐标系下。通过查询坐标系信息，获取坐标系到绝对坐标系的转换矩阵，然后使用Fortran函数进行坐标转换。
> 2. display_temporary_box函数：该函数在指定的坐标系下显示边界框。它首先计算边界框的8个顶点坐标，然后使用display_temporary_line函数显示12条边界框边线。
> 3. display_exact_bounding_box_aligned函数：该函数用于在指定的坐标系下显示精确对齐的边界框。它通过UF_MODL_ask_bounding_box_exact函数查询NX模型的精确边界框信息，然后调用display_temporary_box函数显示边界框。
> 4. 代码使用NX提供的UF函数进行坐标系变换和边界框查询，通过自定义的display_temporary_box函数进行边界框的显示，实现了在指定坐标系下精确显示NX模型边界框的功能。
> 5. 代码具有良好的可复用性，通过封装坐标系转换和边界框显示的函数，可以方便地在其他地方调用，实现类似功能。
>
> 总体来说，这段代码实现了在NX中显示精确对齐的边界框的功能，并且代码结构清晰，具有良好的可维护性和可复用性。
>
