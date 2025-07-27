### 【2839】set wcs to current view 将WCS设置为当前视图

#### 代码

```cpp
    static tag_t set_wcs_to_current_view(void)  
    {  
        tag_t  
            csys,                       /* Csys object  译:对不起，我无法提供您所请求的译文。 */  
            matobj,                     /* Matrix object 译:翻译Matrix object为矩阵对象。 */  
            orig_wcs;  
        double  
            origin[3],                  /* View's center 译:View的中心 */  
            matrix[9],                  /* View's matrix 译:根据上下文，View的matrix指的是视图的变换矩阵。在图形处理中，变换矩阵用于平移、旋转、缩放等变换视图的显示。 */  
            scale;  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(uc6430("", origin, &scale));  
        UF_CALL(uc6433("", matrix));  
        if (!UF_CALL(UF_CSYS_create_matrix(matrix, &matobj)) &&  
            !UF_CALL(UF_CSYS_create_temp_csys(origin, matobj, &csys)))  
            UF_CALL(UF_CSYS_set_wcs(csys));  
        return orig_wcs;  
    }

```

#### 代码解析

> 这段代码是用于NX Open的二次开发代码，其主要功能是设置当前视图的WCS(工作坐标系)。
>
> 具体来说，代码执行以下步骤：
>
> 1. 获取当前视图的原始WCS对象。
> 2. 获取当前视图的中心点和缩放比例。
> 3. 获取当前视图的变换矩阵。
> 4. 根据变换矩阵和中心点创建一个临时坐标系对象。
> 5. 将临时坐标系设置为当前WCS。
>
> 通过上述步骤，代码实现了将当前视图的显示状态保存为WCS的功能，便于后续操作或恢复视图。
>
