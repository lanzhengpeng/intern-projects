### 【0989】create XC YC plane 创建XC YC平面

#### 代码

```cpp
    static tag_t create_XC_YC_plane(void)  
    {  
        tag_t  
            plane,  
            wcs,  
            wcs_mx;  
        double  
            axes[9],  
            origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
        UF_CALL(UF_CURVE_create_plane(origin, &axes[6], &plane));  
        return plane;  
    }

```

#### 代码解析

> 这段代码定义了一个静态函数create_XC_YC_plane()，用于创建一个以WCS的XC-YC平面为基准的平面。代码的主要步骤如下：
>
> 1. 定义了tag_t类型的变量plane、wcs和wcs_mx，用于存储创建的平面、WCS坐标系和WCS坐标系矩阵。
> 2. 定义了double类型的数组axes和origin，用于存储WCS坐标系的轴矢量和原点。
> 3. 调用UF_CSYS_ask_wcs()获取当前WCS坐标系。
> 4. 调用UF_CSYS_ask_csys_info()获取WCS坐标系的矩阵wcs_mx和原点origin。
> 5. 调用UF_CSYS_ask_matrix_values()获取wcs_mx的9个轴矢量分量，存储在axes数组中。
> 6. 调用UF_CURVE_create_plane()创建平面，参数origin是原点，axes[6]是YC轴的单位矢量，因此创建的平面平行于XC-ZC平面，且通过原点。
> 7. 返回创建的平面plane。
>
> 综上，这段代码实现了根据当前WCS的XC-YC平面创建一个平面的功能。
>
