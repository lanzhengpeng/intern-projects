### 【1079】display drf object origin and text box 显示DRF对象的文本框和原点

#### 代码

```cpp
    static void map_point_to_matrix(tag_t matrix, double *pos)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12];  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
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
    static void ask_annotation_text_box_abs(tag_t obj, double lower_left_corner[3],  
        double lower_right_corner[3], double upper_left_corner[3],  
        double upper_right_corner[3])  
    {  
        tag_t  
            matrix;  
        double  
            height,  
            length;  
        UF_CALL(UF_DRF_ask_annotation_text_box(obj, upper_left_corner, &length,  
            &height));  
        UF_CALL(UF_CSYS_ask_matrix_of_object(obj, &matrix));  
        map_point_to_matrix(matrix, upper_left_corner);  
        UF_VEC3_copy(upper_left_corner, lower_left_corner);  
        lower_left_corner[1] = upper_left_corner[1] - height;  
        UF_VEC3_copy(lower_left_corner, lower_right_corner);  
        lower_right_corner[0] = lower_left_corner[0] + length;  
        UF_VEC3_copy(lower_right_corner, upper_right_corner);  
        upper_right_corner[1] = lower_right_corner[1] + height;  
        map_point_from_matrix(matrix, lower_left_corner);  
        map_point_from_matrix(matrix, lower_right_corner);  
        map_point_from_matrix(matrix, upper_right_corner);  
        map_point_from_matrix(matrix, upper_left_corner);  
    }  
    static void display_drf_object_origin_and_text_box(tag_t a_note)  
    {  
        double  
            llc[3],  
            lrc[3],  
            origin[3],  
            ulc[3],  
            urc[3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DRF_ask_origin(a_note, origin));  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            origin, &props, UF_DISP_POINT));  
        ask_annotation_text_box_abs(a_note, llc, lrc, ulc, urc);  
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

> 这段代码是用于NX二次开发的，其主要功能是显示DRF（Drafting）注释对象的原点及其文本框的边框。具体来说，它实现了以下功能：
>
> 1. map_point_to_matrix和map_point_from_matrix两个函数用于将注释对象的点坐标映射到其坐标系，以及从坐标系反映射回原坐标系。
> 2. ask_annotation_text_box_abs函数用于获取注释对象文本框的四个顶点坐标，首先获取文本框的左上角顶点坐标和文本框的高度、长度，然后通过注释对象的坐标系将左上角顶点坐标映射到绝对坐标系，再根据高度和长度计算出其他三个顶点坐标，最后将这些顶点坐标映射回原坐标系。
> 3. display_drf_object_origin_and_text_box函数首先显示注释对象的原点，然后调用ask_annotation_text_box_abs函数获取文本框的四个顶点坐标，最后通过绘制四条线段连接这四个顶点，以显示文本框的边框。
>
> 综上所述，这段代码实现了在NX中显示DRF注释对象的原点以及文本框边框的功能。
>
