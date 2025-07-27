### 【0661】create curve rectangle 创建曲线矩形

#### 代码

```cpp
    struct rectangle_data_s {  
        double  
            origin[3],  
            x_axis[3],  
            y_axis[3],  
            opposite[3];  
    };  
    typedef struct rectangle_data_s rectangle_data_t, *rectangle_data_p_t;  
    static void create_curve_rectangle(rectangle_data_p_t where)  
    {  
        tag_t  
            lines[4];  
        double  
            pos[4][3],  
            dx[3],  
            dy[3],  
            end[3],  
            start[3];  
        UF_CURVE_line_t  
            line_data = { 0,0,0, 0,0,0 };  
        UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, where->origin,  
            UF_CSYS_ROOT_WCS_COORDS, start);  
        UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, where->opposite,  
            UF_CSYS_ROOT_WCS_COORDS, end);  
        UF_VEC3_scale( end[0] - start[0], where->x_axis, dx );  
        UF_VEC3_scale( end[1] - start[1], where->y_axis, dy );  
        UF_VEC3_copy( where->origin, pos[0] );  
        UF_VEC3_add( pos[0], dx, pos[1] );  
        UF_VEC3_add( pos[1], dy, pos[2] );  
        UF_VEC3_sub( pos[2], dx, pos[3] );  
        UF_VEC3_copy( pos[0], line_data.start_point);  
        UF_VEC3_copy( pos[1], line_data.end_point);  
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[0]));  
        UF_VEC3_copy( pos[1], line_data.start_point);  
        UF_VEC3_copy( pos[2], line_data.end_point);  
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[1]));  
        UF_VEC3_copy( pos[2], line_data.start_point);  
        UF_VEC3_copy( pos[3], line_data.end_point);  
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[2]));  
        UF_VEC3_copy( pos[3], line_data.start_point);  
        UF_VEC3_copy( pos[0], line_data.end_point);  
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[3]));  
    }

```

#### 代码解析

> 这段代码用于在NX中创建一个矩形的曲线。代码首先定义了一个结构体rectangle_data_t，用于存储矩形的原点、X轴、Y轴和对面点信息。
>
> 接着定义了一个静态函数create_curve_rectangle，用于根据rectangle_data_t结构体中的数据创建矩形的四条边。函数的主要步骤包括：
>
> 1. 映射矩形的原点和对面点从模型坐标系到世界坐标系。
> 2. 计算X轴和Y轴的向量。
> 3. 根据原点、X轴和Y轴计算矩形的四个顶点位置。
> 4. 利用UF_CURVE_create_line函数，根据四个顶点创建四条直线，构成矩形的四条边。
> 5. 将创建的直线tag赋值给传入的rectangle_data_t结构体的lines数组。
>
> 整体上，这段代码实现了根据矩形的参数信息，在NX中创建相应的矩形曲线的功能。
>
