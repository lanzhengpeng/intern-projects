### 【2869】show curve flip direction 显示曲线翻转方向

#### 代码

```cpp
    static void show_curve_flip_direction(tag_t curve)  
    {  
        double  
            junk[3],  
            end[3],  
            flip_dir[3],  
            tangent[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, end, tangent, junk, junk,  
                junk, junk));  
        UF_VEC3_negate(tangent, flip_dir);  
        display_colored_conehead(end, flip_dir, UF_OBJ_RED);  
    }

```

#### 代码解析

> 这段NX Open API代码的功能是显示曲线上指定位置的翻转方向。具体来说：
>
> 1. 首先，调用UF_MODL_ask_curve_props函数获取曲线在参数为1.0的位置处的端点坐标、切线方向等属性，并存储在相应的数组中。
> 2. 接着，使用UF_VEC3_negate函数计算切线的相反方向，即翻转方向，并存储在flip_dir数组中。
> 3. 最后，调用display_colored_conehead函数在曲线端点位置显示一个红色的圆锥头，圆锥头的方向即为翻转方向。
>
> 通过这个函数，我们可以直观地看到曲线在该点的翻转方向，对于理解和调试曲线形状非常有帮助。
>
