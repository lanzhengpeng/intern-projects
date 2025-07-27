### 【1585】map arc center from abs 映射圆弧中心从绝对坐标系

#### 代码

```cpp
    static void map_arc_center_from_abs(UF_CURVE_arc_t *adata)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[16];  
        UF_CALL(UF_CSYS_ask_matrix_values(adata->matrix_tag, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        FTN(uf5941)(adata->arc_center, mx);  
    }

```

#### 代码解析

> 根据这段代码，它实现了以下功能：
>
> 1. 定义了一个名为map_arc_center_from_abs的静态函数，该函数将NX中的圆弧中心点从绝对坐标系转换到圆弧自身的坐标系。
> 2. 函数参数为指向UF_CURVE_arc_t结构体的指针，该结构体中存储了圆弧的数据，包括圆弧中心点在绝对坐标系下的坐标。
> 3. 定义了几个变量，包括irc表示返回码，abs_mx存储绝对坐标系到圆弧坐标系变换的4x4矩阵，csys存储圆弧的坐标系，mx存储变换后的矩阵。
> 4. 调用UF_CSYS_ask_matrix_values函数，获取圆弧的坐标系csys。
> 5. 通过FTN宏调用uf5940函数，将绝对坐标系到圆弧坐标系的4x4变换矩阵abs_mx与圆弧坐标系csys相乘，得到变换后的16x1矩阵mx。
> 6. 再次通过FTN宏调用uf5941函数，利用变换矩阵mx将圆弧中心点adata->arc_center从绝对坐标系转换到圆弧坐标系。
> 7. 通过调用uf5940和uf5941这两个Fortran函数实现坐标变换。
> 8. 该函数的目的是将圆弧中心点坐标转换到圆弧坐标系下，便于后续的几何计算和处理。
>
> 总之，该代码实现了从绝对坐标系到圆弧坐标系的坐标变换，为后续的几何计算和处理提供了便利。
>
