### 【0340】ask perpendicular 3D 在三维空间中求垂直向量

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

> 这是NX Open C++的二次开发代码，主要功能是计算向量vec在向量ccw上的垂直向量p_vec。
>
> 代码的主要逻辑如下：
>
> 1. 使用UF_MTX3_initialize_z函数，将向量ccw转换为一个3x3矩阵mtx。该矩阵的第1列为ccw向量，第2列为ccw的垂直向量，第3列为ccw的垂直向量。
> 2. 将p_vec向量赋值给矩阵mtx的第2列和第3列。
> 3. 使用UF_MTX3_initialize函数，计算向量vec在矩阵mtx作用下的变换结果。变换后，向量vec的方向将调整为垂直于向量ccw，长度不变。
> 4. 最后，将变换后的向量赋值给p_vec。
>
> 通过这段代码，可以快速计算向量vec在向量ccw上的垂直向量p_vec，为后续的建模、装配等操作提供支持。
>
