### 【0381】ask wcs origin 问坐标系原点

#### 代码

```cpp
    static void ask_wcs_origin(double origin[3])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
    }

```

#### 代码解析

> 这段代码是用于NX（Siemens NX）的二次开发，主要功能是查询并获取当前工作坐标系（WCS）的原点坐标。
>
> 代码主要包含以下步骤：
>
> 1. 调用UF_CSYS_ask_wcs函数，获取当前活动的工作坐标系（WCS）的tag。
> 2. 调用UF_CSYS_ask_csys_info函数，传入WCS的tag，以及一个tag_t类型的参数和一个double类型的数组参数。
> 3. UF_CSYS_ask_csys_info函数会查询并获取WCS的原点坐标，并存储在传入的double数组中。
> 4. 最后，ask_wcs_origin函数返回了当前WCS的原点坐标。
>
> 这段代码的主要作用是获取当前活动的工作坐标系的原点坐标，为后续的建模或其他操作提供基础坐标信息。
>
