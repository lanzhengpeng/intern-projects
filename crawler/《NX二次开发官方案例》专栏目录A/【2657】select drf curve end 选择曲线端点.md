### 【2657】select drf curve end 选择曲线端点

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
    static int ask_which_curve_endpoint(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            ends[6];  
        ask_curve_ends(curve, &ends[0], &ends[3]);  
        UF_VEC3_distance(pos, ends, &dist[0]);  
        UF_VEC3_distance(pos, &ends[3], &dist[1]);  
        if (dist[0] < dist[1]) return UF_DRF_first_end_point;  
        else return UF_DRF_last_end_point;  
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
    static tag_t select_drf_curve_end(char *prompt, tag_t *view, int *assoc_mod)  
    {  
        int  
            resp;  
        double  
            pos[3];  
        tag_t  
            object;  
        UF_CALL(UF_UI_set_cursor_view(FALSE));  
        UF_CALL(UF_UI_select_with_single_dialog("Select curve end", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_drf_curves, NULL, &resp,  
            &object, pos, view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            *assoc_mod = ask_which_curve_endpoint(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open API二次开发代码，主要包含以下功能：
>
> 1. 视图变换相关函数：map_abs2view()和map_view2abs()用于将绝对坐标系下的点转换到当前视图坐标系，以及将当前视图坐标系下的点转换到绝对坐标系。
> 2. 位置相关函数：ask_pos_on_obj()用于在指定曲面上查询与给定位置最近的点。
> 3. 曲线端点相关函数：ask_curve_ends()用于获取曲线的两个端点坐标，ask_which_curve_endpoint()用于判断给定位置距离曲线的哪个端点更近。
> 4. 选择相关函数：mask_for_drf_curves()用于设置选择过滤条件，只允许选择线、圆、圆锥曲线、样条曲线、实体边、实体轮廓线、截面线等。select_drf_curve_end()用于提示用户选择一个曲线端点，并返回选择结果。
>
> 总体来说，这段代码提供了视图变换、位置查询、曲线端点处理、选择过滤等二次开发功能，可以用于NX的交互式二次开发。
>
