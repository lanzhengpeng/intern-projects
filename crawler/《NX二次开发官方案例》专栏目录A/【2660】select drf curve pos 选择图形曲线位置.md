### 【2660】select drf curve pos 选择图形曲线位置

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
    /* 里海 */  
    static int mask_for_drf_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[7] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_solid_silhouette_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 7, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical select_drf_curve_pos(char *prompt, UF_DRF_object_p_t curve,  
        double cp[3])  
    {  
        int  
            resp;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select point on curve", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_curves, NULL, &resp,  
            &curve->object_tag, cp, &curve->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(curve->object_tag, cp);  
            UF_CALL(UF_DISP_set_highlight(curve->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这是段NX二次开发代码，主要实现了以下功能：
>
> 1. 坐标系转换：提供了两个函数map_abs2view和map_view2abs，用于将绝对坐标系下的点转换为视图坐标系下的点，以及反之。这两个函数通过NX提供的API实现坐标系之间的转换。
> 2. 询问对象上的位置：提供了ask_pos_on_obj函数，可以询问指定对象上与给定直线最近的位置，并返回该位置。首先将直线转换到视图坐标系下，然后创建该直线，询问对象与该直线的最小距离，最后将结果转换回绝对坐标系。
> 3. 选择曲线位置：提供了select_drf_curve_pos函数，用于选择曲线上的一个位置。它首先设置选择掩码，只允许选择线、圆、锥、样条线等曲线类型。然后弹出选择对话框，让用户选择曲线上的一个点。如果用户成功选择，就询问该曲线对象上的位置，并返回该位置。
> 4. 坐标系转换：两个函数map_abs2view和map_view2abs实现了坐标系转换的功能，ask_pos_on_obj函数利用这两个函数将直线转换到视图坐标系下进行询问，并将结果转换回绝对坐标系。
> 5. 选择曲线位置：select_drf_curve_pos函数实现了选择曲线上的一个位置的功能，首先设置选择掩码，然后弹出选择对话框让用户选择，最后利用ask_pos_on_obj函数获取该位置。
> 6. 坐标系转换：这些函数主要利用NX提供的API实现坐标系转换和模型查询等功能。
> 7. 交互式选择：select_drf_curve_pos函数通过对话框让用户交互式选择曲线上的位置。
> 8. NX二次开发：这段代码是NX的二次开发代码，利用NX提供的API实现坐标系转换、模型查询和交互式选择等功能。
> 9. 封装性：函数设计合理，功能单一，具有良好的封装性。
> 10. 注释说明：代码中有详细的中文注释，有助于理解代码的功能和实现原理。
>
