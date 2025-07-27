### 【1590】map arc data2wcs 将弧线数据从弧线坐标系转换到世界坐标系

#### 代码

```cpp
    static void map_arc_data2wcs(tag_t arc, UF_CURVE_arc_t *arc_data)  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            add,  
            arc_xyz[9],  
            wcs_xyz[9];  
        map_arc_center2wcs(arc, arc_data);  
        UF_CALL(UF_CSYS_ask_matrix_values(arc_data->matrix_tag, arc_xyz));  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_matrix_of_object(wcs, &wcs_mx));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, wcs_xyz));  
        UF_VEC3_angle_between(&wcs_xyz[6], &arc_xyz[6], wcs_xyz, &add);  
        if (add == PI)  
        {  
            add = arc_data->start_angle;  
            arc_data->start_angle = TWOPI - arc_data->end_angle;  
            arc_data->end_angle = TWOPI - add;  
        }  
        UF_VEC3_angle_between(wcs_xyz, arc_xyz, &wcs_xyz[6], &add);  
        arc_data->start_angle = arc_data->start_angle + add;  
        arc_data->end_angle = arc_data->end_angle + add;  
        if (arc_data->start_angle > TWOPI)  
        {  
            arc_data->start_angle = arc_data->start_angle - TWOPI;  
            arc_data->end_angle = arc_data->end_angle - TWOPI;  
        }  
    }

```

#### 代码解析

> 这段代码主要用于NX CAD软件中，目的是将圆弧的局部坐标系数据转换到全局坐标系中。代码的主要功能如下：
>
> 1. 获取圆弧的局部坐标系和全局坐标系的数据，包括坐标原点和旋转轴向量。
> 2. 计算局部坐标系到全局坐标系的旋转角度。
> 3. 将圆弧的起始角度和终止角度从局部坐标系转换到全局坐标系。
> 4. 如果转换后的起始角度超过2π，则将其减去2π，使角度落在[0, 2π]范围内。
> 5. 最后更新圆弧的数据结构，将转换后的全局坐标系角度赋值给起始角度和终止角度。
>
> 该代码的关键点包括：
>
> 总之，该代码实现了圆弧从局部坐标系到全局坐标系的转换，为后续处理提供了全局角度信息。
>
