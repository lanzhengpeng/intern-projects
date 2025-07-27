### 【0199】ask face dir 询问面方向

#### 代码

```cpp
    static void ask_face_dir(double *loc, tag_t face, double *z_dir)  
    {  
        double  
            parm[2],  
            pnt[3],  
            dirU2[3],  
            dirV1[3],  
            dirV2[3],  
            radii[2],  
            x_dir[3];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, x_dir, dirV1, dirU2,  
            dirV2, z_dir, radii));  
        UF_VEC3_negate(z_dir, z_dir);  
    }

```

#### 代码解析

> 这段代码是一个用于NX二次开发的函数，主要用于获取NX中一个面的法线方向。具体来说，代码的功能如下：
>
> 1. 定义了函数ask_face_dir，它接受面标签face和面的位置loc作为参数，并返回该面的法线方向z_dir。
> 2. 在函数内部，定义了多个临时变量，用于存储面的参数、点的坐标、U和V方向的向量以及两个半径等。
> 3. 使用UF_MODL_ask_face_parm函数获取面的参数和点的坐标。
> 4. 使用UF_MODL_ask_face_props函数获取面的X方向、两个V方向和一个U方向向量，以及法线方向和两个半径。
> 5. 最后，对法线方向向量取反，因为在NX中，面的法线方向默认是指向面外侧，但通常需要指向面内侧，所以取反后得到正确的法线方向。
> 6. 最终，将计算出的法线方向z_dir返回给调用者。
>
> 综上所述，这个函数通过NX的API，实现了获取指定位置的面法线方向的功能，为后续的几何计算和建模提供了基础支持。
>
