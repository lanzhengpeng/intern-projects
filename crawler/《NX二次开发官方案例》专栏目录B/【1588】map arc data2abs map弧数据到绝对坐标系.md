### 【1588】map arc data2abs map弧数据到绝对坐标系

#### 代码

```cpp
    static void map_arc_data2abs(UF_CURVE_arc_t *arc_data)  
    {  
        double  
            add,  
            arc_xyz[9],  
            abs_xyz[9] = { 1,0,0, 0,1,0, 0,0,1 };  
        map_arc_center2abs(arc_data);  
        UF_CALL(UF_CSYS_ask_matrix_values(arc_data->matrix_tag, arc_xyz));  
        UF_VEC3_angle_between(&abs_xyz[6], &arc_xyz[6], abs_xyz, &add);  
        if (add == PI)  
        {  
            add = arc_data->start_angle;  
            arc_data->start_angle = TWOPI - arc_data->end_angle;  
            arc_data->end_angle = TWOPI - add;  
        }  
        UF_VEC3_angle_between(abs_xyz, arc_xyz, &abs_xyz[6], &add);  
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

> 这段代码是用于NX二次开发中，用于将弧线数据的中心坐标系转换到绝对坐标系。主要功能包括：
>
> 1. 首先调用map_arc_center2abs()函数，将弧线的中心点转换到绝对坐标系下。
> 2. 获取弧线数据的坐标系变换矩阵，并与绝对坐标系的变换矩阵进行计算，以获取变换角度。
> 3. 根据变换角度，调整弧线的起始角度和终止角度。
> 4. 对调整后的角度进行取模运算，使其落在0到2π之间。
> 5. 最后，更新弧线数据的起始角度和终止角度，完成从中心坐标系到绝对坐标系的转换。
>
> 总体来看，这段代码实现了弧线数据的坐标系转换，是NX二次开发中坐标转换的一个重要组成部分。
>
