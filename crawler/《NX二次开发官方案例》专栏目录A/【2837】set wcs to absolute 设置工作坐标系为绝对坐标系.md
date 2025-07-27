### 【2837】set wcs to absolute 设置工作坐标系为绝对坐标系

#### 代码

```cpp
    static tag_t set_wcs_to_absolute(void)  
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

> 这段代码是NX Open API的二次开发代码，用于将工作坐标系(WCS)设置为绝对坐标系。
>
> 主要步骤包括：
>
> 1. 获取当前的工作坐标系(WCS)。
> 2. 创建一个单位矩阵，表示从当前WCS到绝对坐标系的变换。
> 3. 利用单位矩阵创建一个临时坐标系。
> 4. 将临时坐标系设置为新的WCS。
>
> 这样设置后，WCS就与绝对坐标系完全重合，实现了将WCS设置为绝对坐标系的目的。
>
> 作者可能出于以下考虑编写这段代码：
>
> 1. 方便后续操作，因为绝对坐标系是最简单的坐标系，没有旋转和平移。
> 2. 在某些操作中，需要WCS与绝对坐标系重合，例如在进行某些装配或测量时。
> 3. 方便其他开发者理解和使用，因为绝对坐标系是最常见的坐标系，容易理解和使用。
>
> 总的来说，这段代码通过设置WCS为绝对坐标系，为后续操作提供了便利，简化了问题，并且提高了代码的可读性和易用性。
>
