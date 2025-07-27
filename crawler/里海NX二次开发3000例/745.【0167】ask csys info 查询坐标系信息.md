### 【0167】ask csys info 查询坐标系信息

#### 代码

```cpp
    static void ask_csys_info(tag_t csys, double origin[3], double axes[9])  
    {  
        tag_t  
            matrix;  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, axes));  
    }

```

#### 代码解析

> 这段代码是NX二次开发中获取坐标系信息的函数，主要功能如下：
>
> 1. 函数定义：定义了一个名为ask_csys_info的静态函数，用于查询坐标系的信息。
> 2. 参数说明：接受两个参数csys和origin，其中csys是坐标系标签，origin用于存储原点坐标。
> 3. 获取矩阵：使用UF_CSYS_ask_csys_info函数获取指定坐标系的矩阵。
> 4. 获取轴信息：使用UF_CSYS_ask_matrix_values函数获取矩阵中的轴信息，并将其存储在axes数组中。
> 5. 错误处理：通过UF_CALL宏调用NX提供的函数，并在出错时输出错误信息。
> 6. 功能总结：该函数主要用于获取指定坐标系的原点坐标和轴信息，将坐标系信息存储在传入的数组中，方便后续使用。
>
> 通过调用该函数，可以方便地获取坐标系的相关信息，从而进行坐标系的相关操作。
>
