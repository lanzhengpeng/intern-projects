### 【2843】set wcs to view 设置坐标系到视图

#### 代码

```cpp
    static tag_t set_wcs_to_view(char *view_name)  
    {  
        tag_t  
            csys,                       /* Csys object  译:Csys object的翻译是：

坐标系对象 */  
            matobj,                     /* Matrix object 译:矩阵对象 */  
            orig_wcs;  
        double  
            origin[3],                  /* View's center 译:视图的中心 */  
            matrix[9],                  /* View's matrix 译:视图的矩阵 */  
            scale;  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(uc6430(view_name, origin, &scale));  
        UF_CALL(uc6433(view_name, matrix));  
        if (!UF_CALL( UF_CSYS_create_matrix(matrix, &matobj) ) &&  
            !UF_CALL( UF_CSYS_create_temp_csys(origin, matobj, &csys) ))  
            UF_CALL( UF_CSYS_set_wcs(csys) );  
        return orig_wcs;  
    }

```

#### 代码解析

> 这段代码是用于设置NX中的工作坐标系(WCS)到指定的视图。主要步骤包括：
>
> 1. 获取当前的工作坐标系对象(orig_wcs)。
> 2. 通过函数uc6430和uc6433获取指定视图名称(view_name)的视图中心和视图矩阵。
> 3. 创建一个矩阵对象(matobj)来存储视图的变换矩阵。
> 4. 以视图中心(origin)和变换矩阵(matobj)创建一个临时的坐标系对象(csys)。
> 5. 将临时坐标系对象(csys)设置为新的工作坐标系。
> 6. 返回原始的工作坐标系对象(orig_wcs)。
>
> 通过以上步骤，这段代码实现了在NX中根据视图名称设置工作坐标系的功能，并返回了原始工作坐标系对象。
>
