### 【1603】map plane2abs 将平面上的一点映射到绝对坐标系中

#### 代码

```cpp
    static void map_plane2abs(double origin[3], double xyz[9], double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        UF_VEC3_copy(origin, csys);  
        UF_MTX3_copy(xyz, &csys[3]);  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        FTN(uf5941)(vec, mx);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于将局部坐标系下的向量转换到绝对坐标系下。代码的主要步骤如下：
>
> 1. 定义绝对坐标系下的变换矩阵abs_mx，这里设置为绕Z轴旋转了90度，即X轴向上。
> 2. 定义局部坐标系csys，包括原点origin和方向矩阵xyz。
> 3. 使用UF_MTX3_copy函数将局部坐标系的方向矩阵xyz复制到csys的第4个元素开始的数组中。
> 4. 调用UF_MTX3_transform函数，将局部坐标系csys转换到绝对坐标系下，结果存储在mx数组中。
> 5. 调用UF_VEC3_transform函数，将局部坐标系下的向量vec根据mx变换到绝对坐标系下。
> 6. 在调用UF函数时，使用FTN宏进行正确的函数名转换。
> 7. 参数irc存储了函数调用的返回码。
> 8. 参数vec为局部坐标系下的3维向量，函数结束后将被转换到绝对坐标系下。
>
> 总的来说，这段代码实现了从局部坐标系到绝对坐标系的转换，其中使用了NX Open提供的矩阵和向量变换函数。
>
