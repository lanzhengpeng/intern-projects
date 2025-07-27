### 【2877】show objects csys 显示对象坐标系

#### 代码

```cpp
    static void show_objects_csys(tag_t object, double where[3])  
    {  
        tag_t  
            mx_id;  
        double  
            mx[9];  
        if (!UF_CALL(UF_CSYS_ask_matrix_of_object(object, &mx_id)))  
        {  
            UF_CALL(UF_CSYS_ask_matrix_values(mx_id, mx));  
            display_conehead(where, &mx[0]);  
            display_conehead(where, &mx[3]);  
            display_conehead(where, &mx[6]);  
        }  
    }

```

#### 代码解析

> 这段代码是用于NX Open API的二次开发，其功能是在NX模型中显示给定对象坐标系的三个轴。
>
> 代码首先定义了一个名为show_objects_csys的静态函数，该函数接受一个对象标签和一个三维数组作为参数。这个三维数组用于指定坐标系的原点位置。
>
> 在函数内部，首先定义了一个标签变量mx_id，用于存储坐标系矩阵的标签。接着定义了一个长度为9的数组mx，用于存储坐标系矩阵的值。
>
> 然后调用UF_CSYS_ask_matrix_of_object函数，获取给定对象坐标系的矩阵标签，并存储在mx_id中。若获取成功，则调用UF_CSYS_ask_matrix_values函数，获取坐标系矩阵的值，并存储在mx数组中。
>
> 最后，调用display_conehead函数三次，分别在原点位置显示坐标系的三个轴。这个函数可能是自定义的，用于在指定位置绘制一个圆锥头，代表坐标轴。
>
> 总的来说，这段代码实现了在NX模型中显示对象坐标系的功能，通过获取坐标系矩阵并绘制坐标轴来实现。
>
