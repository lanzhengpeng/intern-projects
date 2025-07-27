### 【0990】curve curve deviation 曲线偏差

#### 代码

```cpp
    static logical curve_curve_deviation(tag_t curve1, tag_t curve2, int n_pts,  
        double dist_tol, double angl_tol)  
    {  
        logical  
            is_equal,  
            overlays = TRUE;  
        int  
            ii,  
            n = 0,  
            over_dist = 0,  
            over_angl = 0;  
        double  
            a_angl1,  
            a_angl2,  
            angle1[3],  
            angle2[3],  
            avg_dist = 0.0,  
            avg_angl = 0.0,  
            d_angl,  
            d_dist,  
            limits[2],  
            max_angl = 0.0,  
            max_dist = 0.0,  
            min_angl = TWOPI,  
            min_dist = 1000000,  
            mtx[9],  
            p1,  
            p2,  
            p3,  
            p_step,  
            point1[3],  
            point2[3],  
            point3[3],  
            t_angl = 0,  
            t_dist = 0;  
        UF_EVAL_p_t  
            eval_p1,  
            eval_p2;  
        UF_CALL(UF_EVAL_initialize(curve1, &eval_p1));  
        UF_CALL(UF_EVAL_initialize(curve2, &eval_p2));  
        UF_CALL(UF_EVAL_is_equal(eval_p1, eval_p2, &is_equal));  
        if (!is_equal)  
        {  
            UF_CALL(UF_EVAL_ask_limits(eval_p1, limits));  
            p_step = ((limits[1] - limits[0])/(n_pts - 1));  
            for (ii = 0; ii < n_pts; ii++)  
            {  
                p1 = limits[0] + ii * p_step;  
                UF_CALL(UF_EVAL_evaluate(eval_p1, 1, p1, point1, angle1));  
                UF_CALL(UF_EVAL_evaluate_closest_point(eval_p2,point1,&p2,point2));  
            /*  the proximity must be reciprocal to within a half step 译:接近必须是半步以内的相互的。 */  
                UF_CALL(UF_EVAL_evaluate_closest_point(eval_p1,point2,&p3,point3));  
                if ((fabs(p1 - p3)) > (p_step/2))  
                {  
                    display_temporary_colored_point(point1, UF_OBJ_YELLOW);  
                    overlays = FALSE;  
                    continue;  
                }  
                n = n + 1;  
                UF_CALL(UF_EVAL_evaluate(eval_p2, 1, p2, point2, angle2));  
                UF_VEC3_distance(point1, point2, &d_dist);  
                t_dist = t_dist + d_dist;  
                if (d_dist > max_dist) max_dist = d_dist;  
                if ((d_dist < min_dist) || (ii == 0)) min_dist = d_dist;  
                if (d_dist > dist_tol) over_dist = over_dist + 1;  
                UF_CALL(UF_MTX3_initialize_x(angle1, mtx));  
                UF_VEC3_angle_between(angle1, angle2, &mtx[6], &d_angl);  
                a_angl1 = fabs(d_angl - PI);  
                a_angl2 = fabs(d_angl - TWOPI);  
                if (a_angl1 < d_angl) d_angl = a_angl1;  
                if (a_angl2 < d_angl) d_angl = a_angl2;  
                d_angl = d_angl * RADEG;  
                t_angl = t_angl + d_angl;  
                if (d_angl > angl_tol) over_angl = over_angl + 1;  
                if (d_angl > max_angl) max_angl = d_angl;  
                if ((d_angl < min_angl) || (ii == 0)) min_angl = d_angl;  
                if ((d_dist > dist_tol) || (d_angl > angl_tol))  
                {  
                    overlays = FALSE;  
                    display_temporary_colored_point(point1, UF_OBJ_RED);  
                    display_temporary_colored_point(point2, UF_OBJ_RED);  
                }  
                else  
                    display_temporary_colored_point(point1, UF_OBJ_GREEN);  
            }  
            avg_dist = t_dist / n;  
            avg_angl = t_angl / n;  
        }  
        ECHO(curve1);  
        ECHO(curve2);  
        ECHO(n);  
        ECHOF(dist_tol);  
        ECHO(over_dist);  
        ECHOF(min_dist);  
        ECHOF(max_dist);  
        ECHOF(avg_dist);  
        ECHOF(angl_tol);  
        ECHO(over_angl);  
        ECHOF(min_angl);  
        ECHOF(max_angl);  
        ECHOF(avg_angl);  
        ECHO(overlays);  
        UF_CALL(UF_EVAL_free(eval_p1));  
        UF_CALL(UF_EVAL_free(eval_p2));  
        return overlays;  
    }

```

#### 代码解析

> 这段代码主要用于比较两条曲线之间的偏差。其主要步骤如下：
>
> 1. 初始化两个曲线的评估器(eval_p1和eval_p2)。
> 2. 检查两条曲线是否完全重合，如果是则直接返回TRUE。
> 3. 如果两条曲线不重合，则计算第一条曲线的参数范围(limits)，并在这个范围内均匀取n_pts个点。
> 4. 对每个点，计算它在第二条曲线上的最近点，并判断两个点的距离是否超过给定的距离公差(dist_tol)。
> 5. 如果超过距离公差，则记录下来，并显示两个点。
> 6. 同时，计算两个点的夹角是否超过给定的角度公差(angl_tol)，如果超过则同样记录下来。
> 7. 统计所有点的最大距离、最小距离、平均距离、最大角度、最小角度、平均角度，以及超过距离公差的点和超过角度公差的点的数量。
> 8. 如果有超过公差的点，则返回FALSE，否则返回TRUE。
> 9. 最后，释放两个曲线的评估器。
>
> 该函数可以用于检测两条曲线之间的偏差，以判断它们是否足够接近，从而可用于曲线匹配等应用。
>
