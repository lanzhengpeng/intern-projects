### 【1601】map obj2abs 将对象坐标映射到绝对坐标系

#### 代码

```cpp
    static void map_obj2abs(tag_t obj, double pos[3])  
    {  
        tag_t  
            obj_mx;  
        double  
            mx[9];  
        UF_CALL(UF_CSYS_ask_matrix_of_object(obj, &obj_mx));  
        UF_CALL(UF_CSYS_ask_matrix_values(obj_mx, mx));  
        UF_MTX3_vec_multiply_t(pos, mx, pos);  
    }

```

#### 代码解析

> 这段代码是NX Open的二次开发代码，实现了从对象坐标系到绝对坐标系的变换。
>
> 代码首先调用UF_CSYS_ask_matrix_of_object()函数，获取对象在绝对坐标系中的变换矩阵。然后调用UF_CSYS_ask_matrix_values()函数，获取变换矩阵的具体值。最后调用UF_MTX3_vec_multiply_t()函数，将对象坐标系下的位置向量与变换矩阵相乘，得到其在绝对坐标系下的位置向量。
>
> 关键点包括：
>
> 1. 使用UF_CSYS_ask_matrix_of_object()函数获取对象在绝对坐标系中的变换矩阵。
> 2. 使用UF_CSYS_ask_matrix_values()函数获取变换矩阵的具体值。
> 3. 使用UF_MTX3_vec_multiply_t()函数进行向量与矩阵的乘法运算，实现从对象坐标系到绝对坐标系的变换。
>
> 整体上，这段代码实现了对象坐标系到绝对坐标系的变换，为后续的几何运算提供了基础。
>
