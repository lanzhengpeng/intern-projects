### 【1592】map csys2abs map_csys2abs 函数用于将坐标系转换到绝对坐标系

#### 代码

```cpp
    static void map_csys2abs(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[16];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }

```

#### 代码解析

> 这是段NX Open C++ API的二次开发代码，其主要功能是将输入的坐标系中的向量转换到绝对坐标系中。
>
> 代码的关键步骤包括：
>
> 1. 定义绝对坐标系到输入坐标系的方向余弦矩阵，初始设置为I。
> 2. 获取输入坐标系的信息，包括原点坐标和方向余弦矩阵。
> 3. 使用UF_CSYS_ask_matrix_values函数获取输入坐标系的方向余弦矩阵。
> 4. 调用Fortran函数FTN(uf5940)计算从绝对坐标系到输入坐标系的转换矩阵。
> 5. 调用FTN(uf5941)函数，使用转换矩阵将输入向量转换到绝对坐标系中。
> 6. 使用UF_VEC3_sub函数，将转换后的向量减去输入坐标系的原点坐标，得到最终的绝对坐标向量。
>
> 通过上述步骤，代码实现了从输入坐标系到绝对坐标系的向量转换功能。
>
