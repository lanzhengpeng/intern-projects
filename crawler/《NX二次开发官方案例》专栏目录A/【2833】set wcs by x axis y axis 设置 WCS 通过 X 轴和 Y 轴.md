### 【2833】set wcs by x axis y axis 设置 WCS 通过 X 轴和 Y 轴

#### 代码

```cpp
    static tag_t set_wcs_by_x_axis_y_axis(double *x_axis, double *y_axis,  
        double *origin)  
    {  
        double  
            csys[9];  
        tag_t  
            mx,  
            orig_wcs,  
            wcs;  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(UF_MTX3_initialize(x_axis, y_axis, csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
        return orig_wcs;  
    }

```

#### 代码解析

> 这段代码是用于在NX中设置工作坐标系(WCS)的函数。其主要步骤包括：
>
> 1. 获取当前的工作坐标系(WCS)。
> 2. 使用给定的X轴和Y轴向量初始化一个3x3的旋转矩阵。
> 3. 根据旋转矩阵创建一个矩阵对象。
> 4. 使用给定的原点坐标和旋转矩阵创建一个新的坐标系对象。
> 5. 设置新的坐标系为当前的工作坐标系(WCS)。
> 6. 返回之前获取的原始工作坐标系(WCS)。
>
> 该函数可以用于在NX中根据给定的X轴、Y轴向量和原点坐标，快速设置一个新的工作坐标系。
>
