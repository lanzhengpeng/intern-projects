### 【1620】map vec wcs2abs 将向量从工作坐标系转换为绝对坐标系

#### 代码

```cpp
    static void map_vec_wcs2abs(double vec[3])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            i,  
            irc;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要功能是将工作坐标系(WCS)中的一个向量转换到绝对坐标系(ABS)中。
>
> 具体步骤如下：
>
> 1. 获取当前WCS的信息，包括WCS的矩阵和WCS的名称。
> 2. 获取WCS的坐标系信息，包括WCS的原点、旋转矩阵等。
> 3. 使用Fortran子程序计算从WCS到ABS的转换矩阵。
> 4. 将WCS中的向量应用转换矩阵，得到ABS中的向量。
> 5. 将ABS中的向量减去WCS的原点，得到ABS中向量的绝对位置。
> 6. 最后，使用UF_VEC3_sub函数将WCS原点从ABS向量中减去，得到ABS中向量的绝对位置。
>
> 通过以上步骤，实现了从WCS到ABS的向量转换。这段代码是NX二次开发中常用的功能之一，可以用于在不同坐标系之间进行坐标转换。
>
