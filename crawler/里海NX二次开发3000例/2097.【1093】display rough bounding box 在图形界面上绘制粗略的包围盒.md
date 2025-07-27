### 【1093】display rough bounding box 在图形界面上绘制粗略的包围盒

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
            dirs[9] = { 1,0,0, 0,1,0, 0,0,1 },  
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
        UF_VEC3_copy(llc, start);  
        UF_VEC3_copy(urc, end);  
        if (csys != NULL_TAG)  
        {  
            UF_CALL(UF_CSYS_ask_csys_info(csys, &csys_mx, dirs));  
            UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, dirs));  
            UF_VEC3_unitize(&dirs[0], 0, &mag, &dirs[0]);  
            UF_VEC3_unitize(&dirs[3], 0, &mag, &dirs[3]);  
            UF_VEC3_unitize(&dirs[6], 0, &mag, &dirs[6]);  
            map_abs2csys(csys, start);  
            map_abs2csys(csys, end);  
        }  
        UF_VEC3_sub(end, start, sizes);  
        ECHO("Box is %.3f X %.3f X %.3f\n", sizes[0], sizes[1], sizes[2]);  
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
    static void display_rough_bounding_box(tag_t object)  
    {  
        double  
            box[6];  
        UF_CALL(UF_MODL_ask_bounding_box(object, box));  
        display_temporary_box(&box[0], &box[3], NULL_TAG);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于NX的二次开发。以下是代码的主要功能：
>
> 1. map_abs2csys函数：该函数用于将绝对坐标系下的点转换到指定的坐标系下。它首先获取指定坐标系的变换矩阵，然后利用该矩阵将绝对坐标系下的点转换到该坐标系下。
> 2. display_temporary_box函数：该函数用于显示一个临时框。它接受框的8个顶点坐标和坐标系信息，并显示框的12条边线。如果提供了坐标系信息，则先将点转换到该坐标系下。
> 3. display_rough_bounding_box函数：该函数用于显示NX对象的粗糙包围盒。它首先获取对象在绝对坐标系下的最小和最大顶点，然后调用display_temporary_box函数显示框。
> 4. 代码中使用了NX Open C++ API的几何模块、显示模块和坐标系模块的函数，实现了从绝对坐标系到指定坐标系的点转换、临时框的显示等功能。
> 5. 代码结构清晰，功能明确，可作为NX二次开发中坐标系转换和图形显示的参考。
>
