### 【0200】ask face dirs 询问面的方向

#### 代码

```cpp
    static void ask_face_dirs(double loc[3], tag_t face, double xyz[9])  
    {  
        double  
            parm[2],  
            pnt[3],  
            dirU2[3],  
            dirV2[3],  
            radii[2];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, &xyz[0], &xyz[3], dirU2,  
            dirV2, &xyz[6], radii));  
        UF_VEC3_negate(&xyz[6], &xyz[6]);  
    }

```

#### 代码解析

> 这段代码是一个函数，用于在NX中获取一个面的参数和属性。
>
> 具体来说：
>
> 1. 函数名为ask_face_dirs，接受面标签face和位置loc，返回面的方向xyz。
> 2. loc是一个长度为3的数组，表示面内的一个点。
> 3. face是面的标签。
> 4. xyz是一个长度为9的数组，用于存储面的3个方向。
> 5. 函数首先使用UF_MODL_ask_face_parm获取面的参数值parm和点的位置pnt。
> 6. 然后使用UF_MODL_ask_face_props获取面的3个方向和曲率等属性。
> 7. 最后，由于第三个方向是面的法线方向，需要取反，所以使用UF_VEC3_negate函数对其进行取反。
>
> 总的来说，这个函数实现了在NX中获取一个面的参数和属性的功能。
>
