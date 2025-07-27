### 【1607】map point to csys 映射点至坐标系统

#### 代码

```cpp
    static void map_point_to_csys(tag_t csys, double *pos)  
    {  
        int  
            irc;  
        tag_t  
            matrix;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys_mx[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12],  
            origin[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys_mx[3]));  
        FTN(uf5940)(abs_mx, csys_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }

```

#### 代码解析

> 这段代码是一个用于将点从一个坐标系转换到另一个坐标系的方法。主要功能包括：
>
> 1. 获取目标坐标系的信息，包括坐标系矩阵和原点。
> 2. 计算目标坐标系矩阵的绝对矩阵。
> 3. 将点的坐标从原始坐标系转换到目标坐标系。
> 4. 使用UF_CALL宏调用NX API函数，并处理返回码。
>
> 具体步骤如下：
>
> 1. 获取目标坐标系的矩阵和原点信息。
> 2. 初始化目标坐标系矩阵的绝对矩阵为单位矩阵。
> 3. 调用uf5940函数，将目标坐标系矩阵转换为绝对矩阵。
> 4. 将点的坐标从原始坐标系转换到目标坐标系。
> 5. 使用UF_CALL宏调用NX API函数，并处理返回码。
>
> 这段代码实现了坐标系转换的功能，可以用于在不同坐标系之间进行坐标转换。
>
