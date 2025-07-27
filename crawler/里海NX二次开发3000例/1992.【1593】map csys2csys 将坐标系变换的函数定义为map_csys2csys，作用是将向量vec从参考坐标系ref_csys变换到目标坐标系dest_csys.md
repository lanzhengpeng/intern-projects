### 【1593】map csys2csys 将坐标系变换的函数定义为map_csys2csys，作用是将向量vec从参考坐标系ref_csys变换到目标坐标系dest_csys

#### 代码

```cpp
    static void map_csys2csys(tag_t ref_csys, tag_t dest_csys, double vec[3])  
    {  
        double  
            d_csys[12],  
            mx[16],  
            r_csys[12];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, r_csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &r_csys[3]));  
        UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, d_csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &d_csys[3]));  
        FTN(uf5940)(r_csys, d_csys, mx, &irc);  
        FTN(uf5941)(vec, mx);  
    }

```

#### 代码解析

> 这段代码用于NX的二次开发，其主要功能是将一个坐标系中的向量转换到另一个坐标系中。
>
> 具体来说，这段代码实现了以下功能：
>
> 1. 获取参考坐标系ref_csys和目标坐标系dest_csys的信息，包括原点和旋转矩阵。
> 2. 将向量vec从参考坐标系转换到目标坐标系。
> 3. 使用NX提供的Fortran接口函数FTN(uf5940)计算两个坐标系之间的转换矩阵。
> 4. 使用NX提供的Fortran接口函数FTN(uf5941)将向量vec通过转换矩阵转换到目标坐标系。
> 5. 函数map_csys2csys封装了坐标系转换的逻辑，通过输入两个坐标系和向量，实现了不同坐标系之间的向量转换。
>
> 总的来说，这段代码通过NX提供的坐标系管理接口，实现了坐标系之间的向量转换功能，为NX的二次开发提供了坐标变换的基础支持。
>
