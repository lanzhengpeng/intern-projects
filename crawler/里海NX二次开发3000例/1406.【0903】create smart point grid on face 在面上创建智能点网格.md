### 【0903】create smart point grid on face 在面上创建智能点网格

#### 代码

```cpp
    static void create_smart_point_grid_on_face(tag_t face, int nu, int nv)  
    {  
        int  
            ii, jj;  
        tag_t  
            point,  
            u_tag,  
            v_tag;  
        double  
            du, dv,  
            u, v;  
        set_undo_mark("Grid of points");  
        du = (1.0 / (nu - 1));  
        dv = (1.0 / (nv - 1));  
        for (ii = 0, u = 0; ii < nu; ii++, u = u + du)  
        {  
            for (jj = 0, v = 0; jj < nv; jj++, v = v + dv)  
            {  
                UF_CALL(UF_SO_create_scalar_double(face,  
                    UF_SO_update_within_modeling, u, &u_tag));  
                UF_CALL(UF_SO_create_scalar_double(face,  
                    UF_SO_update_within_modeling, v, &v_tag));  
                 UF_CALL(UF_POINT_create_on_surface(face, u_tag, v_tag,  
                    &point));  
            }  
        }  
    }

```

#### 代码解析

> 这段代码实现了在NX中的指定面上创建一个智能点网格的功能。
>
> 代码主要执行以下步骤：
>
> 1. 设置一个撤销标记，用于后续可能的撤销操作。
> 2. 计算U向和V向的参数步长du和dv。
> 3. 通过嵌套循环遍历U向和V向的参数，创建参数点。
> 4. 对于每个参数点，调用UF_SO_create_scalar_double函数创建两个标量，分别表示U和V参数值。
> 5. 调用UF_POINT_create_on_surface函数在面上根据参数创建点。
> 6. 循环结束后，完成整个智能点网格的创建。
>
> 总体而言，该代码通过参数化方式，在NX的面上创建了一个规则的点网格。这个功能在二次开发中非常有用，例如用于网格划分或形状分析等。
>
