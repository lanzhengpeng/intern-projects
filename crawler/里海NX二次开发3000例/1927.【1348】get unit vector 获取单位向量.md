### 【1348】get unit vector 获取单位向量

#### 代码

```cpp
    static void get_unit_vector(tag_t p1, tag_t p2, double vctr[3])  
    {  
        double  
            p1_nums[3],  
            p2_nums[3],  
            non_unit_vec[3],  
            magnitude;  
        UF_CALL( UF_CURVE_ask_point_data( p1, p1_nums ));  
        UF_CALL( UF_CURVE_ask_point_data( p2, p2_nums ));  
        UF_VEC3_subtract( p2_nums, p1_nums, non_unit_vec);  
        UF_VEC3_unitize( non_unit_vec, 0.0001, &magnitude, vctr );  
    }

```

#### 代码解析

> 这段NX Open C++代码用于计算两个曲线点之间的单位向量。以下是代码的主要功能：
>
> 1. 定义了get_unit_vector函数，它接受两个曲线点的tag，并返回它们之间的单位向量。
> 2. 使用UF_CURVE_ask_point_data函数获取两个点的坐标。
> 3. 通过UF_VEC3_subtract函数计算两点之间的非单位向量。
> 4. 使用UF_VEC3_unitize函数将非单位向量转换为长度为1的单位向量。
> 5. 单位向量结果存储在vctr数组中。
> 6. magnitude变量用于存储非单位向量的长度。
> 7. 0.0001是UF_VEC3_unitize函数的容差参数。
>
> 总的来说，这段代码实现了NX中两个曲线点之间的单位向量的计算。
>
