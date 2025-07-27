### 【2846】set wcs2abs 将工作坐标系设置为绝对坐标系

#### 代码

```cpp
    static tag_t set_wcs2abs(void)  
    {  
        tag_t  
            csys,  
            mx,  
            orig_wcs;  
        double  
            abs_mx[9] = { 1,0,0, 0,1,0, 0,0,1 },  
            zero[3] = { 0,0,0 };  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(UF_CSYS_create_matrix(abs_mx, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(zero, mx, &csys));  
        UF_CALL(UF_CSYS_set_wcs(csys));  
        return orig_wcs;  
    }

```

#### 代码解析

> 这段代码是用于NX Open的二次开发，其主要功能是将工作坐标系(WCS)设置为绝对坐标系。下面是代码的具体步骤：
>
> 1. 定义所需的tag类型变量，包括坐标系csys、矩阵mx和工作坐标系orig_wcs。
> 2. 初始化一个3x3的单位矩阵abs_mx，用于表示从WCS到绝对坐标系的转换。
> 3. 获取当前的工作坐标系并保存到orig_wcs变量。
> 4. 根据单位矩阵abs_mx创建一个变换矩阵mx。
> 5. 在原点(0,0,0)处，根据变换矩阵mx创建一个新的坐标系csys。
> 6. 将新的坐标系csys设置为工作坐标系WCS。
> 7. 返回原始的工作坐标系orig_wcs。
>
> 总之，这段代码的主要功能是将WCS设置为与绝对坐标系完全重合。这通常用于在建模前将WCS重置为初始状态。
>
