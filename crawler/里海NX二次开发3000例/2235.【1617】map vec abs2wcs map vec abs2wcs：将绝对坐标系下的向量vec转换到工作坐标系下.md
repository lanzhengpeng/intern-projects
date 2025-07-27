### 【1617】map vec abs2wcs map vec abs2wcs：将绝对坐标系下的向量vec转换到工作坐标系下

#### 代码

```cpp
    static void map_vec_abs2wcs(double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 },  
            origin[3],  
            mx[16];  
        int  
            irc;  
        tag_t  
            input_csys,  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&input_csys));  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要实现了从绝对坐标系到工作坐标系的转换。
>
> 主要步骤包括：
>
> 1. 定义了绝对坐标系到工作坐标系的转换矩阵abs_mx。
> 2. 获取当前的工作坐标系CSYS。
> 3. 获取工作坐标系的原点坐标origin。
> 4. 获取工作坐标系的旋转矩阵csys。
> 5. 使用UF_CSYS_ask_matrix_values函数获取工作坐标系的矩阵值。
> 6. 使用uf5940函数计算绝对坐标系到工作坐标系的转换矩阵mx。
> 7. 最后，使用uf5941函数将输入的向量vec从绝对坐标系转换到工作坐标系。
>
> 这段代码通过获取当前的工作坐标系信息，并计算绝对坐标系到工作坐标系的转换矩阵，从而实现了从绝对坐标系到工作坐标系的转换。
>
