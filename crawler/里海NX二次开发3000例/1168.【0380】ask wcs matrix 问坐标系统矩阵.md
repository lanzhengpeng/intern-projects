### 【0380】ask wcs matrix 问坐标系统矩阵

#### 代码

```cpp
    static tag_t ask_wcs_matrix(double xyz[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, xyz));  
        return wcs_mx;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的示例，用于获取当前工作坐标系(WCS)的变换矩阵。以下是代码的主要逻辑：
>
> 1. 使用UF_CSYS_ask_wcs函数获取当前工作坐标系的tag。
> 2. 使用UF_CSYS_ask_csys_info函数获取当前工作坐标系的变换矩阵的tag，以及原点坐标。
> 3. 使用UF_CSYS_ask_matrix_values函数获取当前工作坐标系的变换矩阵的9个元素。
> 4. 返回变换矩阵的tag。
>
> 这段代码的关键功能是获取当前工作坐标系的变换矩阵，并将其元素存储在xyz数组中。这个变换矩阵定义了当前WCS相对于模型坐标系(MCS)的旋转和平移。通过获取这个矩阵，可以在后续的代码中计算从WCS到MCS的坐标变换。
>
