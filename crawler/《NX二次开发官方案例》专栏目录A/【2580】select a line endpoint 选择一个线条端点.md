### 【2580】select a line endpoint 选择一个线条端点

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
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
    static void ask_line_close_endpoint(tag_t line, double pos[3])  
    {  
        double  
            parm[3],  
        UF_CURVE_line_t  
            line_coords;  
        UF_CALL(UF_CURVE_ask_line_data(line, &line_coords));  
        UF_CALL(UF_MODL_ask_curve_parm(line, line_coords.start_point, &parm[0],  
            line_coords.start_point));  
        UF_CALL(UF_MODL_ask_curve_parm(line, pos, &parm[1], pos));  
        UF_CALL(UF_MODL_ask_curve_parm(line, line_coords.end_point, &parm[2],  
            line_coords.end_point));  
        if (fabs(parm[1] - parm[0]) < fabs(parm[2] - parm[1]))  
             UF_VEC3_copy(line_coords.start_point, pos);  
        else UF_VEC3_copy(line_coords.end_point, pos);  
    }  
    static tag_t select_a_line_endpoint(char *prompt, double pos[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select line endpoint", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            ask_line_close_endpoint(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open C++的二次开发示例，主要实现了以下功能：
>
> 1. 设置选择掩码：mask_for_lines函数设置了选择掩码，只允许选择线类型。
> 2. 视图坐标转换：map_abs2view和map_view2abs实现了绝对坐标系和视图坐标系之间的转换。
> 3. 查询点到对象的最短距离：ask_pos_on_obj函数在指定的对象上查找与给定点最近的位置。
> 4. 查询线段端点：ask_line_close_endpoint函数判断给定点是线段的哪个端点，并返回对应的端点坐标。
> 5. 选择线段端点：select_a_line_endpoint函数通过对话框让用户选择线段的端点，并返回端点坐标。
> 6. 端点选择：在选择端点后，代码会计算该端点在指定对象上的投影点，并高亮显示选中的线段。
>
> 总体来说，这段代码实现了在NX中通过对话框选择线段端点，并获取该端点在指定对象上的投影位置的功能，为二次开发提供了方便。
>
