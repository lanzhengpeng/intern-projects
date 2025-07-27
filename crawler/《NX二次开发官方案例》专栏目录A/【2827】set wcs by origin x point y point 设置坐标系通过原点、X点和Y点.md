### 【2827】set wcs by origin x point y point 设置坐标系通过原点、X点和Y点

#### 代码

```cpp
    static tag_t set_wcs_by_origin_x_point_y_point(double *origin, double *x_point,  
        double *y_point)  
    {  
        double  
            csys[9],  
            x_axis[3],  
            y_axis[3];  
        tag_t  
            mx,  
            orig_wcs,  
            wcs;  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_VEC3_sub(x_point, origin, x_axis);  
        UF_VEC3_sub(y_point, origin, y_axis);  
        UF_CALL(UF_MTX3_initialize(x_axis, y_axis, csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
        return orig_wcs;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于在NX中设置工作坐标系（WCS）。
>
> 主要步骤包括：
>
> 1. 获取当前WCS的tag。
> 2. 计算X轴和Y轴向量，分别通过从原点(origin)到X点(x_point)和Y点(y_point)的向量减去原点向量得到。
> 3. 根据X轴和Y轴向量初始化3x3的坐标系矩阵。
> 4. 根据坐标系矩阵创建坐标系对象mx。
> 5. 使用原点(origin)和坐标系对象mx创建一个新的临时坐标系wcs。
> 6. 将wcs设置为当前的工作坐标系。
> 7. 返回原始的WCS tag。
>
> 通过调用NX的UF_CSYS相关函数，实现了根据原点、X点和Y点设置WCS的功能。
>
