### 【1598】map matrix2matrix 将参考矩阵转换为目标矩阵的映射

#### 代码

```cpp
    static void map_matrix2matrix(double *ref_mx, double *dest_mx, double loc[3])  
    {  
        int  
            ii,  
            irc;  
        double  
            d_csys[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 },  
            mx[16],  
            r_csys[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 };  
        for (ii = 0; ii < 9; ii++)  
        {  
            r_csys[ii+3] = ref_mx[ii];  
            d_csys[ii+3] = dest_mx[ii];  
        }  
        FTN(uf5940)(r_csys, d_csys, mx, &irc);  
        FTN(uf5941)(loc, mx);  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD系统中进行二次开发的，其功能是进行两个坐标系之间的转换。
>
> 代码的主要逻辑包括以下几个步骤：
>
> 1. 定义两个12元素的数组r_csys和d_csys，用于存储参考坐标系和目标坐标系的信息。其中，前3个元素存储原点信息，后9个元素存储旋转矩阵信息。
> 2. 将输入的参考坐标系矩阵ref_mx和目标坐标系矩阵dest_mx分别赋值给r_csys和d_csys的后9个元素。
> 3. 调用uf5940函数，根据r_csys和d_csys计算出一个4x4的转换矩阵mx。
> 4. 调用uf5941函数，使用mx将输入的位置向量loc从参考坐标系转换到目标坐标系。
> 5. 最后，通过mx计算出的转换结果，可以得到loc在目标坐标系中的位置。
>
> 这段代码实现了两个坐标系之间的转换，并可以应用于NX CAD系统中的坐标系管理和坐标转换。
>
