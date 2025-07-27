### 【1624】mark gaps in curve list 在曲线列表中标记间隙

#### 代码

```cpp
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
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
    static double mark_gaps_in_curve_list(uf_list_p_t objects)  
    {  
        int  
            ii = 0,  
            small;  
        double  
            biggest_gap = 0.0,  
            distance_tolerance,  
            gap[4],  
            midpt[4][3],  
            last_start[3],  
            last_end[3],  
            this_start[3],  
            this_end[3];  
        char  
            msg[133];  
        uf_list_p_t  
            tmp;  
        UF_MODL_ask_distance_tolerance(&distance_tolerance);  
        for (tmp = objects; tmp != NULL; tmp = tmp->next)  
        {  
            if (tmp != objects)  
            {  
                ask_curve_ends(tmp->eid, this_start, this_end);  
                UF_VEC3_distance(last_start, this_start, &gap[0]);  
                UF_VEC3_midpt(last_start, this_start, midpt[0]);  
                UF_VEC3_distance(last_end, this_start, &gap[1]);  
                UF_VEC3_midpt(last_end, this_start, midpt[1]);  
                UF_VEC3_distance(last_start, this_end, &gap[2]);  
                UF_VEC3_midpt(last_start, this_end, midpt[2]);  
                UF_VEC3_distance(last_end, this_end, &gap[3]);  
                UF_VEC3_midpt(last_end, this_end, midpt[3]);  
                for (ii = 0; ii < 4; ii++)  
                {  
                    if (ii == 0) small = 0;  
                    else if (gap[ii] < gap[small]) small = ii;  
                    if (gap[ii] < distance_tolerance) break;  
                }  
                if (gap[small] > biggest_gap) biggest_gap = gap[small];  
                if (ii == 4)  
                {  
                    sprintf(msg, "gap = %f", gap[small]);  
                    display_temporary_text(msg, midpt[small]);  
                    display_temporary_colored_point(last_start, UF_OBJ_BLUE);  
                    display_temporary_colored_point(last_end, UF_OBJ_BLUE);  
                    display_temporary_colored_point(this_start, UF_OBJ_GREEN);  
                    display_temporary_colored_point(this_end, UF_OBJ_GREEN);  
                }  
            }  
            ask_curve_ends(tmp->eid, last_start, last_end);  
        }  
        return biggest_gap;  
    }

```

#### 代码解析

> 这段代码是一个用于检查曲线之间间隙的NX Open C++函数，主要功能如下：
>
> 1. 定义了显示临时着色点的函数display_temporary_colored_point，用于在指定位置显示临时点，并可以指定颜色。
> 2. 定义了显示临时文本的函数display_temporary_text，用于在指定位置显示临时文本，并可以指定位置和属性。
> 3. 定义了获取曲线端点的函数ask_curve_ends，通过曲线的参数化表达式，计算并获取曲线的起始和结束端点坐标。
> 4. 定义了检查曲线间隙的函数mark_gaps_in_curve_list，通过遍历曲线列表，计算相邻曲线之间的间隙，并找出最大间隙，同时显示间隙信息，高亮显示相关曲线端点。
> 5. 函数首先获取NX的默认距离公差，作为判断间隙是否过大的标准。
> 6. 遍历曲线列表，对于每条曲线，计算与上一条曲线的起始端点和结束端点之间的间隙，并找出最小的间隙。
> 7. 如果最小间隙超过距离公差，则在间隙中点显示间隙大小，同时高亮显示相关曲线端点。
> 8. 函数返回最大间隙值。
>
> 综上，该函数主要用于检测曲线模型中相邻曲线之间的间隙，并通过可视化方式提示设计者进行修改，以提高曲线模型的连续性和质量。
>
