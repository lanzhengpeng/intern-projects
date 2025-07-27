### 【3122】map point from csys 根据坐标系将点映射

#### 代码

```cpp
    static void map_point_from_csys(tag_t csys, double *pos)  
    {  
        int  
            irc;  
        tag_t  
            matrix;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys_mx[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12],  
            origin[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys_mx[3]));  
        FTN(uf5940)(csys_mx, abs_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }

```

#### 代码解析

> 这段代码是NX二次开发的代码，用于实现从NX的坐标系转换点到绝对坐标系的功能。
>
> 代码的主要步骤包括：
>
> 1. 获取坐标系信息：使用UF_CSYS_ask_csys_info函数，从输入的坐标系csys中获取变换矩阵matrix和原点origin。
> 2. 获取变换矩阵值：使用UF_CSYS_ask_matrix_values函数，从变换矩阵matrix中获取变换矩阵值，并存储在csys_mx数组中。
> 3. 计算绝对坐标系下的变换矩阵：调用Fortran子程序FTN(uf5940)，计算绝对坐标系下的变换矩阵mx。
> 4. 点转换：调用Fortran子程序FTN(uf5941)，将输入的点pos通过变换矩阵mx转换到绝对坐标系下。
> 5. 返回结果：转换后的点存储在pos数组中。
>
> 总体来说，这段代码通过获取坐标系信息，计算变换矩阵，并调用Fortran子程序进行坐标转换，实现了坐标系点从csys到绝对坐标系的转换。
>
