### 【0567】create alive normal abs csys 创建一个活着的标准绝对坐标系

#### 代码

```cpp
    static tag_t create_alive_normal_abs_csys(void)  
    {  
        tag_t  
            matrix,  
            new_csys;  
        double  
            origin[3] = { 0,0,0 },  
            values[9] = { 1,0,0, 0,1,0, 0,0,1 };  
        UF_CALL(UF_CSYS_create_matrix(values, &matrix));  
        UF_CALL(UF_CSYS_create_csys(origin, matrix, &new_csys));  
        return new_csys;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的示例，用于创建一个活着的普通绝对坐标系。
>
> 代码中首先定义了一个名为create_alive_normal_abs_csys的静态函数，该函数用于创建一个活着的普通绝对坐标系。
>
> 接着，定义了两个tag_t类型的变量matrix和new_csys，分别用于存储变换矩阵和坐标系。
>
> 然后，定义了两个数组origin和values，分别用于存储坐标原点和变换矩阵的值。
>
> 接下来，调用UF_CSYS_create_matrix函数，使用values数组创建一个变换矩阵，并将该矩阵存储在matrix变量中。
>
> 然后，调用UF_CSYS_create_csys函数，使用origin数组和matrix变量创建一个坐标系，并将该坐标系存储在new_csys变量中。
>
> 最后，返回new_csys变量，即所创建的活着的普通绝对坐标系。
>
> 这个函数通过NX Open C++ API中的UF_CSYS_create_matrix和UF_CSYS_create_csys函数，实现了活着的普通绝对坐标系的创建，并返回了该坐标系的tag。
>
