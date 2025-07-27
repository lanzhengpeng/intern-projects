### 【2866】show curve direction 显示曲线方向

#### 代码

```cpp
    static void display_colored_conehead(double origin[3], double vec[3], int color)  
    {  
        UF_DISP_conehead_attrb_s  
            attrb;  
        UF_DISP_get_conehead_attrb(&attrb);  
        attrb.color = color;  
        UF_DISP_set_conehead_attrb(&attrb);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_DISP_reset_conehead_attrb();  
    }  
    static void show_curve_direction(tag_t curve)  
    {  
        double  
            junk[3],  
            start[3],  
            tangent[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, tangent, junk, junk,  
                junk, junk));  
        display_colored_conehead(start, tangent, UF_OBJ_RED);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. display_colored_conehead函数：用于在NX中显示一个带有颜色的圆锥头，以表示曲线的方向。函数接受圆锥头的原点坐标、方向向量以及颜色参数，并使用NX的UF_DISP接口在当前激活的视图中显示圆锥头。
> 2. show_curve_direction函数：接受一个曲线的tag，获取曲线的起始点坐标和切线方向，并调用display_colored_conehead函数显示圆锥头，以表示该曲线的方向。圆锥头以红色显示。
>
> 这段代码的主要目的是为了在NX中直观地显示曲线的方向，通过在曲线的起点处显示一个红色圆锥头，表示曲线的切线方向。这有助于更好地理解曲线的形状和方向。
>
