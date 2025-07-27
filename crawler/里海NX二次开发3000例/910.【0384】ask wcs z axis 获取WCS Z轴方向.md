### 【0384】ask wcs z axis 获取WCS Z轴方向

#### 代码

```cpp
    static void ask_wcs_z_axis(double *zaxis)  
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
        UF_VEC3_unitize(&axes[6], 0, &mag, zaxis);  
    }

```

#### 代码解析

> 这是段NX二次开发代码，主要用于获取和转换工作坐标系(WCS)的Z轴方向。
>
> 主要步骤如下：
>
> 1. 调用UF_CSYS_ask_wcs函数，获取当前的工作坐标系。
> 2. 调用UF_CSYS_ask_csys_info函数，获取工作坐标系的变换矩阵和原点。
> 3. 调用UF_CSYS_ask_matrix_values函数，获取变换矩阵中的值。
> 4. 使用UF_VEC3_unitize函数，将变换矩阵中的Z轴方向向量标准化，得到单位向量。
> 5. 将标准化的Z轴方向向量存储在参数zaxis中，作为函数的返回值。
>
> 通过这些步骤，代码实现了获取当前工作坐标系Z轴方向的功能。
>
