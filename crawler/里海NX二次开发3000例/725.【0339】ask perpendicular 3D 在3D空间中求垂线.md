### 【0339】ask perpendicular 3D 在3D空间中求垂线

#### 代码

```cpp
    static void ask_perpendicular_3D(double vec[3], double ccw[3], double p_vec[3])  
    {  
        double  
            mtx[9];  
        UF_CALL(UF_MTX3_initialize_z(ccw, mtx));  
        UF_VEC3_copy(&mtx[3], p_vec);  
        UF_CALL(UF_MTX3_initialize(vec, p_vec, mtx));  
        UF_VEC3_copy(&mtx[3], p_vec);  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，主要实现了以下功能：
>
> 1. 定义函数ask_perpendicular_3D：该函数用于计算一个向量vec的垂直向量p_vec。
> 2. 计算垂直向量：首先使用UF_MTX3_initialize_z函数，根据输入的向量ccw计算一个绕Z轴旋转的旋转矩阵。然后将p_vec作为新坐标系的一个轴，与旋转矩阵相乘，得到一个新的旋转矩阵。
> 3. 计算垂直向量：接着使用UF_MTX3_initialize函数，将输入的向量vec作为新坐标系的一个轴，并与上一步计算出的旋转矩阵相乘，得到一个最终的旋转矩阵。
> 4. 返回垂直向量：最后从最终的旋转矩阵中提取出垂直向量p_vec。
>
> 总的来说，该函数通过计算旋转矩阵，实现了计算输入向量vec的垂直向量p_vec的功能。
>
