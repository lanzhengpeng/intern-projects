### 【1635】matrix csys to csys 将参考坐标系转换为目标坐标系的变换矩阵

#### 代码

```cpp
    static void matrix_csys_to_csys(tag_t ref_csys, tag_t dest_csys, double mx[16])  
    {  
        double  
            from_csys[9],  
            from_orig[3],  
            to_csys[9],  
            to_orig[3];  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, from_orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, from_csys));  
        UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, to_orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, to_csys));  
        UF_CALL(UF_MTX4_csys_to_csys(from_orig, from_csys, &from_csys[3],  
            to_orig, to_csys, &to_csys[3], mx));  
    }

```

#### 代码解析

> 这段代码是用于NX Open API的二次开发，用于实现从参考坐标系ref_csys到目标坐标系dest_csys的转换矩阵mx。
>
> 主要步骤包括：
>
> 1. 获取参考坐标系和目标坐标系的信息，包括原点坐标和旋转矩阵。
> 2. 调用UF_MTX4_csys_to_csys函数，将参考坐标系转换到目标坐标系，得到转换矩阵mx。
> 3. 这个转换矩阵mx可以用于将位于参考坐标系中的点转换到目标坐标系中。
> 4. 函数matrix_csys_to_csys封装了坐标系转换的流程，以简化使用。
> 5. 使用NX Open API提供的UF_CSYS和UF_MTX系列函数来实现坐标系转换。
> 6. 通过获取坐标系的原点和旋转矩阵，实现坐标系之间的精确转换。
> 7. 该代码可用于NX的坐标系管理和变换，为后续的几何运算和建模提供基础。
>
> 这段代码简洁高效，利用NX Open API提供的函数实现了坐标系之间的转换，为NX的二次开发提供了便利。
>
