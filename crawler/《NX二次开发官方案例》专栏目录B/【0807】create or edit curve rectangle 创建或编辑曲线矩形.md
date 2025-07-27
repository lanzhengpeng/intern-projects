### 【0807】create or edit curve rectangle 创建或编辑曲线矩形

#### 代码

```cpp
    static void create_or_edit_curve_rectangle(tag_t lines[4],  
        double lower_left_corner[3], double lower_right_corner[3],  
        double upper_left_corner[3], double upper_right_corner[3])  
    {  
        UF_CURVE_line_t  
            line_data;  
        UF_VEC3_copy(lower_left_corner, line_data.start_point);  
        UF_VEC3_copy(lower_right_corner, line_data.end_point);  
        if (lines[0] == NULL_TAG)  
            UF_CALL(UF_CURVE_create_line(&line_data, &lines[0]));  
        else  
            UF_CALL(UF_CURVE_edit_line_data(lines[0], &line_data));  
        UF_VEC3_copy(lower_right_corner, line_data.start_point);  
        UF_VEC3_copy(upper_right_corner, line_data.end_point);  
        if (lines[1] == NULL_TAG)  
            UF_CALL(UF_CURVE_create_line(&line_data, &lines[1]));  
        else  
            UF_CALL(UF_CURVE_edit_line_data(lines[1], &line_data));  
        UF_VEC3_copy(upper_right_corner, line_data.start_point);  
        UF_VEC3_copy(upper_left_corner, line_data.end_point);  
        if (lines[2] == NULL_TAG)  
            UF_CALL(UF_CURVE_create_line(&line_data, &lines[2]));  
        else  
            UF_CALL(UF_CURVE_edit_line_data(lines[2], &line_data));  
        UF_VEC3_copy(upper_left_corner, line_data.start_point);  
        UF_VEC3_copy(lower_left_corner, line_data.end_point);  
        if (lines[3] == NULL_TAG)  
            UF_CALL(UF_CURVE_create_line(&line_data, &lines[3]));  
        else  
            UF_CALL(UF_CURVE_edit_line_data(lines[3], &line_data));  
    }

```

#### 代码解析

> 这段代码用于创建或编辑一个矩形的四条边界线，其中矩形的四个顶点坐标分别由lower_left_corner、lower_right_corner、upper_left_corner和upper_right_corner指定。
>
> 代码的主要逻辑包括以下步骤：
>
> 1. 使用lower_left_corner和lower_right_corner创建或编辑矩形的下边界线(lines[0])。
> 2. 使用lower_right_corner和upper_right_corner创建或编辑矩形的右边界线(lines[1])。
> 3. 使用upper_right_corner和upper_left_corner创建或编辑矩形的上边界线(lines[2])。
> 4. 使用upper_left_corner和lower_left_corner创建或编辑矩形的左边界线(lines[3])。
> 5. 如果线(lines[i])为空，则调用UF_CURVE_create_line创建新线，否则调用UF_CURVE_edit_line_data编辑现有线的起点和终点坐标。
> 6. 使用UF_VEC3_copy函数复制顶点坐标到线的起点和终点。
>
> 综上所述，这段代码的主要作用是创建或编辑一个由指定顶点坐标构成的矩形边界线。
>
