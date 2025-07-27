### 【2899】sort into loops 对曲线进行排序，形成闭环

#### 代码

```cpp
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            junk[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, junk, junk, junk,  
            junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, end, junk, junk, junk,  
            junk, junk));  
    }  
    static void sort_into_loops(tag_t *curves, int n_curves, UF_STRING_p_t string)  
    {  
        logical  
            found;  
        int  
            dirs[100],  
            ii,  
            jj,  
            kk,  
            n_segs[100],  
            n_strings,  
            start;  
        double  
            free_start[3],  
            free_end[3],  
            gap,  
            this_start[3],  
            this_end[3],  
            tol;  
        tag_t  
            temp;  
        UF_MODL_ask_distance_tolerance(&tol);  
        n_strings = 0;  
        n_segs[n_strings] = 1;  
        dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
        ask_curve_ends(curves[0], free_start, free_end);  
        start = 0;  
        for (ii = 0; ii < n_curves; ii++)  
        {  
            found = FALSE;  
            if (!UF_MODL_ask_curve_closed(curves[ii]))  
            {  
                for (jj = ii + 1; jj < n_curves; jj++)  
                {  
                    ask_curve_ends(curves[jj], this_start, this_end);  
                    UF_VEC3_distance(this_start, free_start, &gap);  
                    if (gap <= tol)  
                    {  
                        temp = curves[jj];  
                        for (kk = jj; kk > start; kk--)  
                            curves[kk] = curves[kk-1];  
                        curves[start] = temp;  
                        n_segs[n_strings]++;  
                        dirs[n_strings] = UF_MODL_CURVE_START_FROM_END;  
                        UF_VEC3_copy(this_end, free_start);  
                        found = TRUE;  
                        break;  
                    }  
                    UF_VEC3_distance(this_end, free_start, &gap);  
                    if (gap <= tol)  
                    {  
                        temp = curves[jj];  
                        for (kk = jj; kk > start; kk--)  
                            curves[kk] = curves[kk-1];  
                        curves[start] = temp;  
                        n_segs[n_strings]++;  
                        dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
                        UF_VEC3_copy(this_start, free_start);  
                        found = TRUE;  
                        break;  
                    }  
                    UF_VEC3_distance(this_start, free_end, &gap);  
                    if (gap <= tol)  
                    {  
                        temp = curves[jj];  
                        curves[jj] = curves[ii+1];  
                        curves[ii+1] = temp;  
                        n_segs[n_strings]++;  
                        dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
                        UF_VEC3_copy(this_end, free_end);  
                        found = TRUE;  
                        break;  
                    }  
                    UF_VEC3_distance(this_end, free_end, &gap);  
                    if (gap <= tol)  
                    {  
                        temp = curves[jj];  
                        curves[jj] = curves[ii+1];  
                        curves[ii+1] = temp;  
                        n_segs[n_strings]++;  
                        dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
                        UF_VEC3_copy(this_start, free_end);  
                        found = TRUE;  
                        break;  
                    }  
                }  
                UF_VEC3_distance(free_start, free_end, &gap);  
            }  
            else gap = 0;  
            if (found == FALSE)  
            {  
                if (gap <= tol) n_strings++;  
                else  
                {  
                    n_curves = n_curves - n_segs[n_strings];  
                    ii = ii - n_segs[n_strings];  
                    for (jj = start; jj < n_curves; jj++)  
                        curves[jj] = curves[jj + n_segs[n_strings]];  
                }  
                n_segs[n_strings] = 1;  
                dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
                start = ii+1;  
                if (start < n_curves)  
                    ask_curve_ends(curves[start], free_start, free_end);  
            }  
        }  
        UF_MODL_init_string_list(string);  
        UF_MODL_create_string_list(n_strings, n_curves, string);  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            string->string[ii] = n_segs[ii];  
            string->dir[ii] = dirs[ii];  
        }  
        for (ii = 0; ii < n_curves; ii++)  
            string->id[ii] = curves[ii];  
    }

```

#### 代码解析

> 这段代码的主要功能是将一组曲线按照首尾相连的方式排列成多个环。
>
> 代码步骤如下：
>
> 1. 定义了一个函数ask_curve_ends，用于获取曲线的首尾端点坐标。
> 2. 定义了一个函数sort_into_loops，用于排序曲线。
> 3. 在sort_into_loops函数中，首先获取曲线数量n_curves，并初始化一些变量，如tol（距离公差）、n_strings（环的数量）等。
> 4. 遍历每条曲线，检查是否与其他曲线首尾相连，如果是，则调整曲线的顺序，直到找到所有首尾相连的曲线环。
> 5. 最后，初始化一个字符串列表，并创建每个环的字符串，包括每个环的曲线数量和方向，并保存到字符串列表中。
> 6. 将调整后的曲线ID保存到字符串列表的id数组中。
>
> 总的来说，这段代码实现了将一组曲线排序成多个环的功能，通过调整曲线的顺序，并将每个环的信息保存到字符串列表中。
>
