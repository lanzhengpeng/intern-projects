### 【1618】map vec mx2abs 将向量从初始坐标系转换到绝对坐标系

#### 代码

```cpp
    static void map_vec_mx2abs(tag_t input_mx, double origin[3], double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            i,  
            irc;  
        UF_VEC3_copy(origin, csys);  
        UF_CALL(UF_CSYS_ask_matrix_values(input_mx, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, origin, vec);  
    }

```

#### 代码解析

> 根据代码注释，这是NX Open C++的二次开发代码，主要功能是将输入的坐标系变换到绝对坐标系，并计算向量在绝对坐标系中的表示。
>
> 主要步骤如下：
>
> 1. 定义输入坐标系变换矩阵input_mx和原点origin。
> 2. 定义绝对坐标系变换矩阵abs_mx，设置为绕Z轴旋转矩阵。
> 3. 获取输入坐标系变换矩阵的值。
> 4. 使用UF_CSYS_ask_matrix_values函数获取输入坐标系变换矩阵的值。
> 5. 将输入坐标系变换矩阵转换为绝对坐标系下的变换矩阵。
> 6. 使用UF_VEC3_copy函数将原点origin复制到csys数组。
> 7. 使用UF_VEC3_sub函数计算向量vec在绝对坐标系下的表示。
> 8. 输出向量vec在绝对坐标系下的表示。
>
> 总体来说，该代码实现了从输入坐标系到绝对坐标系的变换，并计算了向量在绝对坐标系下的表示。
>
