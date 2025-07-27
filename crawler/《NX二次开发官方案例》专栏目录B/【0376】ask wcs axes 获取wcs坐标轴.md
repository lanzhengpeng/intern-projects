### 【0376】ask wcs axes 获取wcs坐标轴

#### 代码

```cpp
    static void ask_wcs_axes(double *xaxis, double *yaxis, double *zaxis)  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            axes[9],  
            origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
        UF_VEC3_copy(&axes[0], xaxis);  
        UF_VEC3_copy(&axes[3], yaxis);  
        UF_VEC3_copy(&axes[6], zaxis);  
    }

```

#### 代码解析

> 这段NX Open C++代码用于查询当前工作坐标系(WCS)的X、Y、Z轴向量，并将它们分别赋值给提供的double数组参数。以下是代码的逻辑流程：
>
> 1. 调用UF_CSYS_ask_wcs()获取当前WCS的tag。
> 2. 调用UF_CSYS_ask_csys_info()获取WCS的CSYS信息，并存储在wcs_mx中。
> 3. 调用UF_CSYS_ask_matrix_values()获取wcs_mx的矩阵值，存储在axes数组中。
> 4. 将axes数组的前3个元素(即X轴向量)复制到参数xaxis指向的数组。
> 5. 将axes数组的第4-6个元素(即Y轴向量)复制到参数yaxis指向的数组。
> 6. 将axes数组的第7-9个元素(即Z轴向量)复制到参数zaxis指向的数组。
>
> 通过以上步骤，代码成功获取了当前WCS的X、Y、Z轴向量，并将它们赋值给提供的参数数组，从而实现了获取WCS轴向量的功能。
>
