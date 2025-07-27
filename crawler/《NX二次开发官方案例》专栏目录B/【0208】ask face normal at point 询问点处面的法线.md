### 【0208】ask face normal at point 询问点处面的法线

#### 代码

```cpp
    static void ask_face_normal_at_point(tag_t face, double loc[3],  
        double normal[3])  
    {  
        double  
            junk[3],  
            parm[2],  
            pt[3];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, junk, junk, junk, junk, junk,  
            normal, junk));  
    }

```

#### 代码解析

> 这是一段NX Open C++ API编写的代码，用于在NX中获取指定面上某点的法向量。
>
> 函数ask_face_normal_at_point()接收三个参数：
>
> 函数内部首先调用UF_MODL_ask_face_parm()，通过给定的点loc，反求该点在面上的参数坐标parm。然后调用UF_MODL_ask_face_props()，根据参数坐标获取该点处的法向量，并存储在normal数组中。
>
> 整体来看，这段代码实现了在NX中获取指定面上任意点的法向量，对于需要进行几何运算的二次开发场景非常有用。
>
