### 【1614】map abs2csys 将绝对坐标系转换为指定坐标系

#### 代码

```cpp
    static void map_abs2csys(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[16];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }

```

#### 代码解析

> 这段NX Open C++代码实现了从绝对坐标系转换到输入坐标系的功能。具体步骤如下：
>
> 1. 定义一个从绝对坐标系到输入坐标系的转换矩阵abs_mx。
> 2. 获取输入坐标系的名称和变换矩阵csys。
> 3. 使用函数FTN(uf5940)计算从绝对坐标系到输入坐标系的变换矩阵mx。
> 4. 使用函数FTN(uf5941)将向量vec从绝对坐标系转换到输入坐标系。
> 5. 最后，从向量vec中减去输入坐标系的坐标，得到最终的转换结果。
>
> 代码的关键在于计算两个坐标系之间的变换矩阵，并将向量进行转换。通过调用NX提供的底层函数，实现了坐标转换的功能。
>
