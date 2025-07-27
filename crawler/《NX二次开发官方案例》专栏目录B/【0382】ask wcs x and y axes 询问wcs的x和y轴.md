### 【0382】ask wcs x and y axes 询问wcs的x和y轴

#### 代码

```cpp
    static void ask_wcs_x_and_y_axes(double *xaxis, double *yaxis)  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            axes[9],  
            mag,  
            origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
        UF_VEC3_unitize(&axes[0], 0, &mag, xaxis);  
        UF_VEC3_unitize(&axes[3], 0, &mag, yaxis);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，其主要功能是获取当前工作坐标系(WCS)的X轴和Y轴方向。
>
> 具体来说，代码首先使用UF_CSYS_ask_wcs函数获取当前WCS。然后，使用UF_CSYS_ask_csys_info函数获取WCS的矩阵和原点信息。接着，通过UF_CSYS_ask_matrix_values函数获取WCS的矩阵值。最后，利用UF_VEC3_unitize函数将X轴和Y轴的向量单位化，并获取其长度，分别存储在xaxis和yaxis参数中。
>
> 这个函数的作用在于获取当前WCS的X轴和Y轴方向，通常用于坐标系转换、方向计算等场景。其设计采用了NX Open C++ API中的坐标系管理、矩阵操作和向量运算相关函数，简洁高效。
>
