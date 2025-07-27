### 【0377】ask wcs info 请求坐标系信息

#### 代码

```cpp
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是获取当前工作坐标系(WCS)的信息。
>
> 具体步骤如下：
>
> 1. 调用UF_CSYS_ask_wcs()函数，获取当前工作坐标系(WCS)的tag。
> 2. 调用UF_CSYS_ask_csys_info()函数，根据WCS的tag，获取WCS的矩阵信息，并传入origin数组用于存储WCS的原点坐标。
> 3. 调用UF_CSYS_ask_matrix_values()函数，根据WCS的矩阵信息，获取WCS的轴信息，并传入axes数组用于存储WCS的9个轴向量。
>
> 通过调用这些函数，可以获取当前WCS的完整信息，包括原点坐标和轴向量。这些信息对于后续进行坐标变换等操作非常重要。
>
