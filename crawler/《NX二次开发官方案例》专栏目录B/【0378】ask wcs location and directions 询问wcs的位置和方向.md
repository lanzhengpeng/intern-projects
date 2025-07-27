### 【0378】ask wcs location and directions 询问wcs的位置和方向

#### 代码

```cpp
static void ask_wcs_location_and_directions(double origin[3], double x_axis[3], 
    double y_axis[3], double z_axis[3])
{
    tag_t
        wcs,
        wcs_mx;
    double
        axes[9],
        mag;
    UF_CALL(UF_CSYS_ask_wcs(&wcs));
    UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));
    UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));
    UF_VEC3_unitize(&axes[0], 0, &mag, x_axis);
    UF_VEC3_unitize(&axes[3], 0, &mag, y_axis);
    UF_VEC3_unitize(&axes[6], 0, &mag, z_axis);
}

```

#### 代码解析

> 这段代码是NX Open C++ API中用于获取工作坐标系(WCS)的位置和方向信息的函数。代码的主要功能包括：
>
> 1. 使用UF_CSYS_ask_wcs函数获取当前活动的WCS。
> 2. 使用UF_CSYS_ask_csys_info函数获取WCS的矩阵信息，包括原点坐标(origin)。
> 3. 使用UF_CSYS_ask_matrix_values函数获取WCS的旋转矩阵的值，并将其存储在axes数组中。
> 4. 使用UF_VEC3_unitize函数分别对axes数组中的x、y、z轴向量进行单位化，得到单位向量x_axis、y_axis、z_axis。
> 5. 这3个单位向量表示了WCS的3个坐标轴的方向。
>
> 总之，这段代码通过NX Open API获取了当前WCS的详细信息，包括原点坐标和3个坐标轴的方向，为后续的几何运算提供了基础。
>
