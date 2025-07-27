### 【0379】ask wcs matrix tag 查询工作坐标系矩阵标签

#### 代码

```cpp
    static tag_t ask_wcs_matrix_tag(void)  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        return wcs_mx;  
    }

```

#### 代码解析

> 这段代码是用于获取NX中的工作坐标系(WCS)矩阵的NX Open二次开发代码。
>
> 具体步骤如下：
>
> 1. 首先，定义了两个tag_t类型的变量wcs和wcs_mx，用于存储WCS和工作坐标系矩阵的信息。
> 2. 接着，定义了一个double类型的数组origin，用于存储WCS的原点信息。
> 3. 使用UF_CSYS_ask_wcs函数，获取当前的WCS，并将结果存储在变量wcs中。
> 4. 然后，使用UF_CSYS_ask_csys_info函数，根据获取的wcs信息，获取WCS的矩阵信息wcs_mx和原点信息origin。
> 5. 最后，返回WCS的矩阵信息wcs_mx。
>
> 这段代码的主要作用是获取当前WCS的矩阵信息，可以用于后续的坐标变换等操作。
>
