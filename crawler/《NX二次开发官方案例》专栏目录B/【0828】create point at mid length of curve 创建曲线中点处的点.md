### 【0828】create point at mid length of curve 创建曲线中点处的点

#### 代码

```cpp
    static void create_point_at_mid_length_of_curve(tag_t curve)  
    {  
        tag_t  
            last,  
            offset,  
            start;  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.0, &start));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
            curve, start, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_invisible));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.5, &offset));  
        UF_CALL(UF_SO_create_point_along_curve(curve, UF_SO_update_after_modeling,  
            curve, last, offset, UF_SO_point_along_curve_percent, FALSE, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
    }

```

#### 代码解析

> 这段代码是在NX中创建曲线中点的二次开发代码，其流程如下：
>
> 1. 首先，使用UF_SO_create_scalar_double()函数创建一个参数为0.0的标量，并将其设置为曲线的开始点。
> 2. 然后，通过UF_SO_create_point_on_curve()函数在曲线上创建一个点，这个点位于曲线的开始处。
> 3. 接着，使用UF_SO_set_visibility_option()函数将这个点的可见性设置为不可见。
> 4. 使用UF_SO_create_scalar_double()函数创建一个参数为0.5的标量，表示曲线长度的50%。
> 5. 通过UF_SO_create_point_along_curve()函数在曲线上创建一个点，这个点位于曲线长度的50%处。
> 6. 最后，使用UF_SO_set_visibility_option()函数将这个点的可见性设置为可见。
>
> 总的来说，这段代码的目的是在NX的曲线上创建一个位于曲线长度50%处的点，并将其显示出来。
>
