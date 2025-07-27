### 【0153】ask annotation text box abs 询问注释文本框的绝对位置

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

```

#### 代码解析

> 这段代码是NX二次开发代码，主要实现了以下功能：
>
> 1. map_point_to_matrix函数：将一个点从绝对坐标系转换到指定的坐标系。它通过获取指定坐标系的矩阵，然后使用该矩阵将点进行转换。
> 2. map_point_from_matrix函数：与map_point_to_matrix函数相反，它将一个点从指定的坐标系转换回绝对坐标系。通过获取指定坐标系的矩阵，然后使用该矩阵的逆矩阵将点进行转换。
> 3. ask_annotation_text_box_abs函数：获取NX对象（如注释文本）的文本框在绝对坐标系中的四个顶点位置。首先获取对象在对象坐标系中的文本框尺寸，然后转换到绝对坐标系中，并计算出文本框的四个顶点位置。
>
> 总体来说，这些函数实现了坐标系之间的转换，以及获取对象在绝对坐标系中的位置信息。这为后续的二次开发操作提供了基础支持。
>
