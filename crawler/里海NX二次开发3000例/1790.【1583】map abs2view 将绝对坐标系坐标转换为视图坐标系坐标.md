### 【1583】map abs2view 将绝对坐标系坐标转换为视图坐标系坐标

#### 代码

```cpp
    static void map_abs2view(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[16] = { 0,0,0 },  
            mx[16];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，主要实现了从绝对坐标系到视图坐标系的转换。
>
> 关键点包括：
>
> 1. 通过UF_OBJ_ask_name函数获取视图名称。
> 2. 通过uc6433函数获取视图的视图坐标系。
> 3. 通过uf5940函数将绝对坐标系转换为视图坐标系。
> 4. 最后通过uf5941函数将给定的点从绝对坐标系转换到视图坐标系。
>
> 总的来说，这段代码通过NX Open C++ API实现了从绝对坐标系到视图坐标系的转换，其中涉及获取视图名称、视图坐标系、坐标转换等操作。
>
