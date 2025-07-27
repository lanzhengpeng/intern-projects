### 【1074】display colored conehead 显示彩色圆锥头

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

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要实现了以下功能：
>
> 1. 定义了一个名为display_colored_conehead的静态函数，用于在NX视图中绘制一个彩色圆锥头。
> 2. 函数接受三个参数：origin表示圆锥头的原点坐标，vec表示圆锥头的方向向量，color表示圆锥头的颜色。
> 3. 函数首先获取当前圆锥头的属性attrb，并修改其颜色属性为参数color指定的颜色。
> 4. 调用UF_DISP_conehead函数，在所有激活的视图中绘制圆锥头，参数0表示圆锥头是关闭的。
> 5. 在绘制完成后，调用UF_DISP_reset_conehead_attrb()函数将圆锥头的属性重置为默认值。
> 6. 该函数封装了绘制彩色圆锥头的流程，便于在其他函数中调用。通过改变color参数，可以绘制不同颜色的圆锥头。
> 7. 使用静态函数的目的是隐藏实现细节，使其他代码更简洁。
> 8. 该函数利用NX Open C++ API提供的显示功能，实现NX视图中的图形绘制。
>
