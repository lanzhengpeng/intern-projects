### 【1615】map abs2wcs map_abs2wcs表示将绝对坐标系坐标转换为工作坐标系坐标

#### 代码

```cpp
    /*  
        NOTE:  This is obsolete - see UF_CSYS_map_point  
    */  
    static void map_abs2wcs(double *cc)  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[16],  
            mx[16];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(abs_mx, wcs, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要实现了从绝对坐标系到工作坐标系(WCS)的坐标转换功能。
>
> 具体步骤如下：
>
> 1. 定义了一个3x3的绝对坐标系到工作坐标系的旋转矩阵abs_mx，默认为单位矩阵。
> 2. 定义了工作坐标系wcs的4x4变换矩阵wcs，并通过UF_CSYS_ask_wcs和UF_CSYS_ask_csys_info接口获取WCS的变换信息，填充wcs矩阵。
> 3. 调用UF_CSYS_ask_matrix_values接口获取WCS的旋转矩阵，并填充wcs矩阵中的旋转部分。
> 4. 定义了一个中间的4x4变换矩阵mx，通过Fortran函数FTN(uf5940)计算abs_mx和wcs的乘积，并将结果存储在mx中。
> 5. 最后，通过Fortran函数FTN(uf5941)将输入的绝对坐标系坐标cc转换到工作坐标系，结果存储在cc中。
>
> 总的来说，这段代码实现了绝对坐标系到工作坐标系的坐标转换功能，通过调用NX Open C++ API获取WCS信息，并利用Fortran函数进行矩阵运算。
>
