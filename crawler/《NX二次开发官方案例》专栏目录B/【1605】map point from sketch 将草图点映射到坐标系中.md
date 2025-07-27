### 【1605】map point from sketch 将草图点映射到坐标系中

#### 代码

```cpp
    static void map_point_from_sketch(tag_t sketch, double *pos)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            mx[12],  
            sketch_mx[9];  
        UF_SKET_info_t  
            sket_info;  
        UF_CALL(UF_SKET_ask_sketch_info(sketch, &sket_info));  
        UF_VEC3_copy(&sket_info.csys[9], &sketch_mx[0]);  
        UF_VEC3_copy(&sket_info.csys[0], &sketch_mx[3]);  
        UF_VEC3_copy(&sket_info.csys[3], &sketch_mx[6]);  
        FTN(uf5940)(sketch_mx, abs_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }

```

#### 代码解析

> 这段代码的主要功能是从NX的草图对象中获取一个点的位置，并将其转换到绝对坐标系中。以下是代码的详细解释：
>
> 1. 定义了一个名为map_point_from_sketch的静态函数，它接受一个NX草图对象和一个指向双精度数组的指针作为参数。这个数组用于存储点的位置。
> 2. 定义了一个名为sketch的NX草图对象和一个名为pos的double数组，用于存储点的位置。
> 3. 定义了一个9元素的双精度数组abs_mx，它表示绝对坐标系的变换矩阵。
> 4. 定义了一个12元素的double数组mx，它用于存储从草图坐标系到绝对坐标系的变换矩阵。
> 5. 定义了一个UF_SKET_info_t结构体sket_info，用于存储NX草图的信息。
> 6. 使用UF_SKET_ask_sketch_info函数获取草图的信息，并将草图坐标系转换到绝对坐标系。
> 7. 使用UF_VEC3_copy函数将草图坐标系的信息复制到sketch_mx数组。
> 8. 调用Fortran子程序FTN(uf5940)计算从草图坐标系到绝对坐标系的变换矩阵，并将其存储到mx数组。
> 9. 调用Fortran子程序FTN(uf5941)将点的位置从草图坐标系转换到绝对坐标系，并将结果存储到pos数组。
> 10. 整个函数通过NX提供的API，实现了从草图对象中获取点的位置，并将其转换到绝对坐标系的功能。
>
