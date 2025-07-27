### 【2484】report spline thru points 报告样条曲线通过点

#### 代码

```cpp
    static void report_spline_thru_points(tag_t spline)  
    {  
        int  
            degree,  
            ii,  
            num_points,  
            periodicity;  
        double  
            *parameters;  
        UF_CURVE_pt_slope_crvatr_t  
            *point_data;  
        if (!UF_CALL(UF_CURVE_ask_spline_thru_pts(spline, &degree, &periodicity,  
            &num_points, &point_data, &parameters)))  
        {  
            WRITE_D(spline);  
            WRITE_D(degree);  
            WRITE_D(periodicity);  
            WRITE_D(num_points);  
            for (ii = 0; ii < num_points + periodicity; ii++)  
            {  
                WRITE_D(ii);  
                WRITE3F(point_data[ii].point);  
                WRITE_D(point_data[ii].slope_type);  
                WRITE_F(parameters[ii]);  
            }  
            UF_free(point_data);  
            UF_free(parameters);  
        }  
    }

```

#### 代码解析

> 这段NX Open C++代码的功能是报告通过给定样条曲线的控制点信息。其主要步骤包括：
>
> 1. 获取样条曲线的度数(degree)、周期性(periodicity)、控制点数量(num_points)以及控制点数据(point_data)和参数(parameters)。
> 2. 若获取成功，则输出样条曲线的度数、周期性、控制点数量等信息。
> 3. 遍历控制点数据，输出每个控制点的索引、坐标、斜率类型以及对应的参数值。
> 4. 释放控制点数据和参数的内存空间。
>
> 该代码通过UF_CURVE_ask_spline_thru_pts函数获取样条曲线的控制点信息，并输出这些信息。它有助于了解给定样条曲线的具体控制点情况，为样条曲线的分析和修改提供了重要参考。
>
