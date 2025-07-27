### 【2900】sort into strings 将曲线按顺序连接成字符串

#### 代码

```cpp
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            limits[2];  
        UF_EVAL_p_t  
            evaluator;  
        UF_CALL(UF_EVAL_initialize(curve, &evaluator));  
        UF_CALL(UF_EVAL_ask_limits(evaluator, limits));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[0], start, NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[1], end, NULL));  
        UF_CALL(UF_EVAL_free(evaluator));  
    }  
    static void sort_into_strings(tag_t *curves, int n_curves, UF_STRING_p_t string)  
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
                    UF_VEC3_copy(this_start, free_end);  
                    found = TRUE;  
                    break;  
                }  
            }  
            if (found == FALSE)  
            {  
                n_strings++;  
                n_segs[n_strings] = 1;  
                dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
                start = ii+1;  
                if (ii+1 < n_curves)  
                    ask_curve_ends(curves[ii+1], free_start, free_end);  
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

> 这段代码实现了NX中的曲线拼接功能，其主要逻辑如下：
>
> 1. 获取曲线端点：首先定义了一个函数ask_curve_ends，用于获取曲线的起点和终点。
> 2. 排序曲线：然后定义了一个函数sort_into_strings，用于将输入的曲线数组排序拼接成多个连续的字符串。具体步骤如下：初始化参数，包括曲线段数、方向等。遍历每条曲线，尝试与已排序的曲线端点拼接，如果端点距离小于公差，则调整曲线顺序，并更新端点。如果无法拼接，则增加一个新的字符串，并继续拼接下一条曲线。最终输出一个字符串数组，包含每个字符串的段数、方向和曲线ID。
> 3. 初始化参数，包括曲线段数、方向等。
> 4. 遍历每条曲线，尝试与已排序的曲线端点拼接，如果端点距离小于公差，则调整曲线顺序，并更新端点。
> 5. 如果无法拼接，则增加一个新的字符串，并继续拼接下一条曲线。
> 6. 最终输出一个字符串数组，包含每个字符串的段数、方向和曲线ID。
> 7. 拼接成字符串：代码最后遍历曲线数组，初始化字符串列表，创建字符串列表，并设置每个字符串的段数、方向和包含的曲线ID。
> 8. 主要逻辑：这段代码的主要逻辑是遍历曲线，根据端点距离将连续的曲线拼接成字符串，以便后续创建实体等操作。
>
> 通过以上步骤，实现了NX中的曲线拼接功能。
>
