### 【1589】map arc data2abs 将弧线数据转换为绝对坐标系

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

> 这段代码是NX Open C++ API的二次开发代码，用于将弧线的中心坐标系下的参数转换到绝对坐标系下。代码的主要步骤包括：
>
> 1. 调用map_arc_center2abs()函数，将弧线的中心坐标系转换到绝对坐标系下。
> 2. 获取弧线中心坐标系下的矩阵，并计算与绝对坐标系之间的旋转角度。
> 3. 如果旋转角度为180度，则交换起点和终点角度，并调整角度值。
> 4. 计算绝对坐标系下的旋转角度，并更新起点和终点角度。
> 5. 对角度值进行取模运算，确保角度值在0到2PI之间。
> 6. 最后，更新弧线的起点和终点角度，完成从中心坐标系到绝对坐标系的转换。
>
> 总体来说，这段代码实现了弧线参数从中心坐标系到绝对坐标系的转换，是NX二次开发中常见的功能之一。
>
