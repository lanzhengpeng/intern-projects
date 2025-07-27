### 【2874】show matrix csys 显示矩阵坐标系

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
    static void show_matrix_csys(tag_t matrix, double where[3])  
    {  
        double  
            mx[9];  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, mx));  
        display_colored_conehead(where, &mx[0], UF_OBJ_RED);  
        display_colored_conehead(where, &mx[3], UF_OBJ_GREEN);  
        display_colored_conehead(where, &mx[6], UF_OBJ_BLUE);  
    }

```

#### 代码解析

> 这是段NX二次开发代码，用于显示坐标系。
>
> 代码中定义了两个函数：
>
> 1. display_colored_conehead(): 用于显示一个圆锥头，可以设置圆锥头的颜色和位置。函数参数包括圆锥头的原点坐标origin，方向向量vec，以及颜色color。函数内部首先获取圆锥头的默认属性attrb，然后设置颜色，调用UF_DISP_conehead()显示圆锥头，最后重置圆锥头的属性。
> 2. show_matrix_csys(): 用于显示一个坐标系。函数参数包括坐标系的tag_t矩阵matrix和位置坐标where。函数内部首先获取坐标系的矩阵值mx，然后分别显示三个坐标轴，x轴红色，y轴绿色，z轴蓝色。三个轴的显示通过调用display_colored_conehead()函数实现。
>
> 整体来看，这段代码通过自定义的圆锥头显示函数，实现了NX坐标系的可视化显示。
>
