### 【1622】map wcs2abs 将wcs坐标系转换到绝对坐标系

#### 代码

```cpp
    /*  
        NOTE:  this is obsolete - see UF_CSYS_map_point  
    */  
    static void map_wcs2abs(double *cc)  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(wcs, abs_mx, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于将WCS(工作坐标系)坐标转换为绝对坐标系坐标。主要步骤包括：
>
> 1. 定义绝对坐标系到WCS的转换矩阵abs_mx。
> 2. 获取当前WCS的信息，包括WCS实体wcs_ent、WCS矩阵wcs_mx以及WCS变换矩阵wcs。
> 3. 获取WCS变换矩阵的值。
> 4. 调用Fortran子程序FTN(uf5940)计算WCS到绝对坐标系的转换矩阵mx。
> 5. 调用Fortran子程序FTN(uf5941)使用转换矩阵mx将给定的WCS坐标cc转换到绝对坐标系。
> 6. 注释中提到该函数已经过时，建议使用UF_CSYS_map_point函数。
>
> 总的来说，这段代码实现了WCS到绝对坐标系的坐标转换，但已不建议使用，建议使用更新的UF_CSYS_map_point函数。
>
