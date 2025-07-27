### 【1586】map arc center2abs 将弧心从局部坐标系映射到绝对坐标系

#### 代码

```cpp
    static void map_arc_center2abs(UF_CURVE_arc_t *arc_data)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[16] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[16];  
        UF_CALL(UF_CSYS_ask_matrix_values(arc_data->matrix_tag, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        FTN(uf5941)(arc_data->arc_center, mx);  
    }

```

#### 代码解析

> 这段代码是用于NX Open API的二次开发，其主要功能是将弧线中心点从局部坐标系转换到绝对坐标系。
>
> 具体步骤如下：
>
> 1. 定义了两个矩阵：abs_mx表示绝对坐标系，csys表示当前弧线的局部坐标系。
> 2. 使用UF_CSYS_ask_matrix_values函数获取弧线局部坐标系的矩阵值，并赋值给csys。
> 3. 调用Fortran函数FTN(uf5940)将局部坐标系csys转换到绝对坐标系abs_mx，得到转换矩阵mx。
> 4. 调用Fortran函数FTN(uf5941)使用转换矩阵mx，将弧线的中心点arc_data->arc_center从局部坐标系转换到绝对坐标系。
> 5. map_arc_center2abs函数实现了弧线中心点坐标系的转换，将局部坐标系转换到绝对坐标系，以便后续操作。
>
> 这段代码利用NX Open API提供的坐标系转换接口，实现了弧线中心点坐标系的转换，是进行后续几何运算的基础。
>
