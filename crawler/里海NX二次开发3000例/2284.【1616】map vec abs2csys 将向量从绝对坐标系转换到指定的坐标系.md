### 【1616】map vec abs2csys 将向量从绝对坐标系转换到指定的坐标系

#### 代码

```cpp
    static void map_vec_abs2csys(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 },  
            origin[3],  
            mx[16];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++函数，用于将一个向量从一个坐标系转换到绝对坐标系。
>
> 主要步骤如下：
>
> 1. 定义一个绝对坐标系到当前坐标系变换的4x4矩阵，初始为旋转矩阵，原点平移为0。
> 2. 获取当前坐标系的信息，包括原点和旋转矩阵。
> 3. 调用Fortran函数uf5940，将绝对坐标系到当前坐标系的变换矩阵与当前坐标系到绝对坐标系的变换矩阵相乘，得到从当前坐标系到绝对坐标系的变换矩阵。
> 4. 调用Fortran函数uf5941，使用当前坐标系到绝对坐标系的变换矩阵，将输入向量从当前坐标系变换到绝对坐标系。
> 5. 输出结果向量，完成坐标变换。
>
> 这个函数通过获取坐标系信息，构造变换矩阵，并应用变换矩阵，实现了向量在不同坐标系之间的变换。
>
