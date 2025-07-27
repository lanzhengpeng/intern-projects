### 【2658】select drf curve endpoint 选择DRF曲线端点

#### 代码

```cpp
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs_mx, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs_mx, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void ask_pos_on_obj(tag_t obj, double loc[3])  
    {  
        tag_t  
            line;  
        double  
            cp[3],  
            dist;  
        UF_CURVE_line_t  
            lp;  
        map_abs2view(loc);  
        lp.start_point[0] = loc[0];  
        lp.start_point[1] = loc[1];  
        lp.start_point[2] = loc[2] + 1000;  
        lp.end_point[0] = loc[0];  
        lp.end_point[1] = loc[1];  
        lp.end_point[2] = loc[2] - 1000;  
        map_view2abs(lp.start_point);  
        map_view2abs(lp.end_point);  
        UF_CALL(UF_CURVE_create_line(&lp, &line));  
        UF_CALL(UF_MODL_ask_minimum_dist(obj, line, 0, cp, 0, cp, &dist, loc, cp));  
        UF_CALL(UF_OBJ_delete_object(line));  
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
    static void ask_curve_close_endpoint(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            ends[6];  
        ask_curve_ends(curve, &ends[0], &ends[3]);  
        UF_VEC3_distance(pos, ends, &dist[0]);  
        UF_VEC3_distance(pos, &ends[3], &dist[1]);  
        if (dist[0] < dist[1]) UF_VEC3_copy(ends, pos);  
        else UF_VEC3_copy(&ends[3], pos);  
    }  
    static void ask_edge_close_endpoint(tag_t edge, double pos[3])  
    {  
        int  
            cnt;  
        double  
            parm[3],  
            end[3],  
            start[3];  
        UF_CALL(UF_MODL_ask_edge_verts(edge, start, end, &cnt));  
        UF_CALL(UF_MODL_create_curve_from_edge(edge, &edge));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, start, &parm[0], start));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, pos, &parm[1], pos));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, end, &parm[2], end));  
        UF_CALL(UF_OBJ_delete_object(edge));  
        if (fabs(parm[1] - parm[0]) < fabs(parm[2] - parm[1]))  
             UF_VEC3_copy(start, pos);  
        else UF_VEC3_copy(end, pos);  
    }  
    /* 里海 */  
    static int mask_for_drf_curves(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            mask[7] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, UF_all_subtype },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_solid_silhouette_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 7, mask)))  
            return (UF_UI_SEL_FAILURE);  
        else  
            return (UF_UI_SEL_SUCCESS);  
    }  
    static tag_t select_drf_curve_endpoint(char *prompt, tag_t view, double pos[3])  
    {  
        int  
            subtype,  
            resp,  
            type;  
        tag_t  
            object;  
        UF_CALL(UF_UI_set_cursor_view(FALSE));  
        UF_CALL(UF_UI_select_with_single_dialog("Select curve end", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_drf_curves, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if (type == UF_solid_type) ask_edge_close_endpoint(object, pos);  
            else ask_curve_close_endpoint(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是选择曲线的端点并获取其位置。具体包括以下几个关键点：
>
> 1. 包含两个坐标系转换函数map_abs2view和map_view2abs，用于将绝对坐标系转换为视图坐标系，反之亦然。
> 2. ask_pos_on_obj函数用于获取对象上的指定位置点。
> 3. ask_curve_ends函数用于获取曲线的两个端点位置。
> 4. ask_curve_close_endpoint和ask_edge_close_endpoint函数用于获取曲线或边线距离指定点最近的端点。
> 5. mask_for_drf_curves函数用于设置选择过滤条件，只允许选择线、圆、圆锥曲线、样条线、实体边、实体轮廓线和截面线。
> 6. select_drf_curve_endpoint函数是主函数，用于提示用户选择曲线端点，获取选中的对象和端点位置，并根据对象类型调用不同的函数获取最近的端点位置。
>
> 总体来说，这段代码实现了在NX中通过对话框选择曲线端点并获取其位置的功能，适用于NX的二次开发。
>
