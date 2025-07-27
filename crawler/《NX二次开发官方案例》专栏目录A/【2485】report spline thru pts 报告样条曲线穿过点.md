### 【2485】report spline thru pts 报告样条曲线穿过点

#### 代码

```cpp
    static void report_spline_thru_pts(tag_t spline)  
    {  
        int  
            degree,  
            num_points,  
            periodicity;  
        double  
            *parameters;  
        UF_CURVE_pt_slope_crvatr_t  
            *point_data;  
        if (!UF_CALL(UF_CURVE_ask_spline_thru_pts(spline, &degree, &periodicity,  
            &num_points, &point_data, &parameters)))  
        {  
            ECHO(spline);  
            ECHO(degree);  
            ECHO(periodicity);  
            ECHO(num_points);  
            UF_free(point_data);  
            UF_free(parameters);  
        }  
    }

```

#### 代码解析

> 这是NX二次开发中用于查询样条曲线参数的函数。主要功能包括：
>
> 1. 查询样条曲线的阶数(degree)：表示样条曲线的阶次，如线性样条为1，二次样条为2，三次样条为3等。
> 2. 查询样条曲线的周期性(periodicity)：表示样条曲线是否具有周期性。
> 3. 查询样条曲线的控制点数量(num_points)：表示构成样条曲线的控制点数量。
> 4. 查询样条曲线的控制点数据(point_data)：包含每个控制点的坐标信息。
> 5. 查询样条曲线的参数(parameters)：包含样条曲线的参数信息，如切矢等。
> 6. 释放内存：释放查询过程中动态分配的内存。
> 7. 错误处理：使用UF_CALL宏检查函数返回值，如果失败则不处理错误。
> 8. 输出信息：使用ECHO宏输出样条曲线的基本信息，如句柄、阶数、周期性、控制点数量等。
>
> 总体来说，该函数通过UF_CURVE_ask_spline_thru_pts函数查询样条曲线的关键参数，并释放相关内存，最后输出基本信息。
>
