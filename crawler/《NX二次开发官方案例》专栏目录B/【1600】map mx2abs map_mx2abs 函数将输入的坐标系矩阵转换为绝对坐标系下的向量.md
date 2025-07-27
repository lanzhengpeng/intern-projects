### 【1600】map mx2abs map_mx2abs 函数将输入的坐标系矩阵转换为绝对坐标系下的向量

#### 代码

```cpp
    static void map_mx2abs(tag_t input_mx, double origin[3], double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        UF_VEC3_copy(origin, csys);  
        UF_CALL(UF_CSYS_ask_matrix_values(input_mx, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        FTN(uf5941)(vec, mx);  
    }

```

#### 代码解析

> 这段NX Open API代码的功能是计算从用户坐标系(UCS)到绝对坐标系(ABS)的变换矩阵，并将一个向量从用户坐标系变换到绝对坐标系。具体步骤如下：
>
> 1. 定义一个静态方法map_mx2abs，它接受输入的用户坐标系tag、原点坐标和向量。
> 2. 定义一个绝对坐标系到用户坐标系的变换矩阵abs_mx，这里假设用户坐标系与绝对坐标系重合，因此变换矩阵为单位矩阵。
> 3. 定义一个临时数组csys用来存储用户坐标系的矩阵值，前三个元素为原点坐标，后九个元素为旋转矩阵。
> 4. 调用UF_CSYS_ask_matrix_values获取用户坐标系的矩阵值，并存入csys的后九个元素。
> 5. 通过Fortran接口调用FTN(uf5940)函数，计算从用户坐标系到绝对坐标系的变换矩阵，存入mx。
> 6. 调用FTN(uf5941)函数，将输入的向量vec通过mx变换到绝对坐标系。
> 7. 返回变换后的向量。
>
> 综上，这段代码实现了从用户坐标系到绝对坐标系的变换，并可用于处理NX中的坐标系转换问题。
>
