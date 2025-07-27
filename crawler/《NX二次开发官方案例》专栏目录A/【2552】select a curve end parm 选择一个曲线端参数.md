### 【2552】select a curve end parm 选择一个曲线端参数

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
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_CURVE_create_line(&lp, &line));  
        UF_CALL(UF_MODL_ask_minimum_dist(obj, line, 0, cp, 0, cp, &dist, loc, cp));  
        UF_CALL(UF_OBJ_delete_object(line));  
        UF_CALL(UF_DISP_set_display(TRUE));  
    }  
    static double ask_curve_close_end_parm(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            end[3],  
            junk[3],  
            start[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, junk, junk, junk,  
            junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, end, junk, junk, junk,  
            junk, junk));  
        UF_VEC3_distance(pos, start, &dist[0]);  
        UF_VEC3_distance(pos, end, &dist[1]);  
        if (dist[0] < dist[1]) return 0.0;  
        else return 1.0;  
    }  
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_curve_end_parm(char *prompt, double *parm)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            pos[3];  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve end", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            *parm = ask_curve_close_end_parm(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发代码，主要包括以下功能：
>
> 1. 视图转换函数：提供了绝对坐标系到视图坐标系的转换函数map_abs2view()，以及视图坐标系到绝对坐标系的转换函数map_view2abs()。这些函数通过NX的API来实现坐标转换。
> 2. 查询位置函数：ask_pos_on_obj()函数用于查询指定位置在对象上的最近点位置。
> 3. 查询曲线端参数函数：ask_curve_close_end_parm()函数用于查询指定点在曲线端点参数上的最近点，返回参数值。
> 4. 曲线端点选择函数：select_a_curve_end_parm()函数通过对话框让用户选择一个曲线端点，然后计算该端点的参数值。
> 5. 曲线端点选择掩码函数：mask_for_curves()函数用于设置选择曲线端点时的掩码，只允许选择线、圆、圆锥曲线和样条曲线。
> 6. 交互式选择端点：通过select_a_curve_end_parm()函数，用户可以交互式选择一个曲线端点，并获取该端点的参数值。
> 7. 坐标转换与位置查询：通过坐标转换函数和位置查询函数，可以方便地实现坐标系之间的转换和查询对象上的点位置。
>
> 总的来说，这段代码提供了坐标系转换、位置查询、曲线端点选择和参数获取等功能，方便在NX中进行二次开发。
>
