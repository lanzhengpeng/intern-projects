### 【2196】report mass properties 报告质量特性

#### 代码

```cpp
    static void report_mass_properties(double *mass_props)  
    {  
        write_double_to_listing_window("Surface Area", mass_props[0]);  
        write_double_to_listing_window("Volume", mass_props[1]);  
        write_double_to_listing_window("Mass", mass_props[2]);  
        write_double_array_to_listing_window("Center of Mass, WCS",  
            &mass_props[3], 1, 3);  
        write_double_array_to_listing_window("First Moments, Centroidal",  
            &mass_props[6], 1, 3);  
        write_double_array_to_listing_window("Moments of Inertia, WCS",  
            &mass_props[9], 1, 3);  
        write_double_array_to_listing_window("Moments of Inertia, Centroidal",  
            &mass_props[12], 1, 3);  
        write_double_to_listing_window("Spherical Moment of inertia",  
            mass_props[15]);  
        write_double_array_to_listing_window("Inertia Products, WCS",  
            &mass_props[16], 1, 3);  
        write_double_array_to_listing_window("Inertia Products, Centroidal",  
            &mass_props[19], 1, 3);  
        write_double_array_to_listing_window("Principal Axes, WCS",  
            &mass_props[22], 3, 3);  
        write_double_array_to_listing_window("Principal Axes, Centroidal",  
            &mass_props[31], 3, 3);  
        write_double_array_to_listing_window("Radius of Gyration, WCS",  
            &mass_props[34], 1, 3);  
        write_double_array_to_listing_window("Radius of Gyration, Centroidal",  
            &mass_props[37], 1, 3);  
        write_double_to_listing_window("Spherical Radius of Gyration",  
            mass_props[40]);  
        write_double_to_listing_window("Density", mass_props[46]);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，主要用于输出NX模型的质量属性信息。
>
> 主要功能包括：
>
> 1. 输出模型的表面面积、体积和质量。
> 2. 输出模型质心在世界坐标系和质心坐标系下的位置。
> 3. 输出模型在世界坐标系和质心坐标系下的第一惯量矩。
> 4. 输出模型在世界坐标系和质心坐标系下的惯性矩。
> 5. 输出模型的世界坐标系和质心坐标系下的惯性积。
> 6. 输出模型的世界坐标系和质心坐标系下的主轴。
> 7. 输出模型的世界坐标系和质心坐标系下的转动惯量半径。
> 8. 输出模型的球形转动惯量半径。
> 9. 输出模型的密度。
>
> 通过调用write_double_to_listing_window和write_double_array_to_listing_window等函数，将质量属性信息输出到NX的列表窗口中，以方便用户查看。
>
> 这段代码为NX模型的后续分析，如结构分析、运动学分析等提供了重要的质量属性信息。
>
