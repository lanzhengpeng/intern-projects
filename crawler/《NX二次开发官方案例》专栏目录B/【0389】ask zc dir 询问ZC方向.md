### 【0389】ask zc dir 询问ZC方向

#### 代码

```cpp
    static void ask_zc_dir(double zc_dir[3])  
    {  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        double  
            origin[3],  
            wcs[9];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, wcs));  
        UF_VEC3_copy(&wcs[6], zc_dir);  
    }

```

#### 代码解析

> 这段代码是用于获取NX中的工作坐标系(WCS)的Z轴方向，并将其存储在数组zc_dir[3]中。代码的主要步骤包括：
>
> 1. 使用UF_CSYS_ask_wcs函数获取当前的WCS实体。
> 2. 使用UF_CSYS_ask_csys_info函数获取WCS的变换矩阵实体和原点坐标。
> 3. 使用UF_CSYS_ask_matrix_values函数获取WCS的变换矩阵值。
> 4. 将WCS变换矩阵的第三行复制到数组zc_dir中，这代表了WCS的Z轴方向。
>
> 通过这些步骤，代码成功获取了当前WCS的Z轴方向，并将其存储在zc_dir数组中。
>
