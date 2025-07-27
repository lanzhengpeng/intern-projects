### 【1465】invert transform 反转变换

#### 代码

```cpp
    static void invert_transform(double transform[16])  
    {  
        double  
            negation[3],  
            old_rotation[9],  
            old_translation[3],  
            rotation[9],  
            translation[3];  
    /*  
        The inverse is just the transpose of the rotation, and the negated  
        translation with the new rotation applied to it.  
    */  
        UF_MTX4_ask_rotation((const double *)transform, old_rotation);  
        UF_MTX3_transpose(old_rotation, rotation);  
        UF_MTX4_ask_translation((const double *)transform, old_translation);  
        UF_VEC3_negate(old_translation, negation);  
        UF_MTX3_vec_multiply(negation, rotation, translation);  
        UF_MTX4_initialize(1.0, translation, rotation, transform);  
    }

```

#### 代码解析

> 这段代码实现了一个名为invert_transform的函数，用于对NX中的4x4变换矩阵进行逆变换。
>
> 主要步骤包括：
>
> 1. 获取原变换矩阵中的旋转部分和平移部分。
> 2. 计算旋转部分的转置，得到逆变换的旋转部分。
> 3. 获取原变换矩阵中的平移部分，取反后与逆变换的旋转部分相乘，得到逆变换的平移部分。
> 4. 使用逆变换的旋转和平移部分重新初始化原变换矩阵。
>
> 通过这个函数，我们可以快速得到一个4x4变换矩阵的逆变换，从而实现NX中的物体或组件的逆变换操作。
>
