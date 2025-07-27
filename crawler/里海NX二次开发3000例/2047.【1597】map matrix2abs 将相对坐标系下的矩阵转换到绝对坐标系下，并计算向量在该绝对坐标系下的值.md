### 【1597】map matrix2abs 将相对坐标系下的矩阵转换到绝对坐标系下，并计算向量在该绝对坐标系下的值

#### 代码

```cpp
    static void map_matrix2abs(tag_t input_matrix, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_CSYS_ask_matrix_values(input_matrix, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，用于将输入的坐标系(input_matrix)转换到绝对坐标系(abs_mx)。
>
> 代码的主要功能包括：
>
> 1. 定义一个静态函数map_matrix2abs，将坐标系转换到绝对坐标系。
> 2. 初始化绝对坐标系(abs_mx)为右手坐标系。
> 3. 获取输入坐标系(input_matrix)的矩阵值，存储在数组csys中。
> 4. 使用NX提供的函数FTN(uf5940)将输入坐标系转换到绝对坐标系，结果存储在数组mx中。
> 5. 检查转换结果是否正确。
> 6. 使用FTN(uf5941)函数将向量vec从输入坐标系转换到绝对坐标系。
> 7. 最后，将转换后的向量vec减去原坐标系的原点csys，得到其在绝对坐标系中的位置。
>
> 整体来说，这段代码实现了NX坐标系之间的转换功能，为后续的建模、装配等操作提供了基础。
>
