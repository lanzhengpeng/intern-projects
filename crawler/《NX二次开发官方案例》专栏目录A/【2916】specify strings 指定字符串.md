### 【2916】specify strings 指定字符串

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
    /* 里海 */  
    static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_curve_end(char *prompt, int *which_end)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            pos[3];  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve end", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            UF_CALL(UF_MODL_get_curve_edge_direction(pos, object, which_end));  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_strings(char *which, UF_STRING_t *string)  
    {  
        int  
            dirs[50],  
            ii = 0,  
            jj = 0,  
            kk = -1,  
            n,  
            n_segs[10];  
        char  
            prompt[81];  
        tag_t  
            curves[50];  
        UF_MODL_init_string_list(string);  
        do  
        {  
            if ((kk > -1) && (curves[kk] == NULL_TAG))  
            {  
                n_segs[ii] = jj-1;  
                ii++;  
                jj = 0;  
                kk--;  
            }  
            if (jj > 0)  
            {  
                sprintf(prompt, "OK to finish selecting string #%d", ii+1);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else if (ii > 0)  
            {  
                sprintf(prompt, "OK to finish selecting %s strings", which);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else  
                UF_CALL(UF_UI_set_status(""));  
            sprintf(prompt, "Select %s String #%d - curve #%d", which, ii+1, ++jj);  
            kk++;  
            curves[kk] = select_a_curve_end(prompt, &dirs[kk]);  
            if (curves[kk] != NULL_TAG)  
                UF_CALL(UF_DISP_set_highlight(curves[kk], TRUE));  
        } while ((curves[kk] != NULL_TAG) || jj > 1);  
        UF_CALL(UF_UI_set_status(""));  
        if (kk > 0)  
        {  
            n_segs[ii] = jj;  
            n = ii;  
            UF_MODL_create_string_list(n, kk, string);  
            kk = 0;  
            for (ii = 0; ii < n; ii++)  
            {  
                string->string[ii] = n_segs[ii];  
                string->dir[ii] = dirs[kk];  
                for (jj = 0; jj < n_segs[ii]; jj++, kk++)  
                {  
                    string->id[kk] = curves[kk];  
                    UF_CALL(UF_DISP_set_highlight(curves[kk], FALSE));  
                }  
            }  
            return TRUE;  
        }  
        return FALSE;  
    }

```

#### 代码解析

> 这段代码主要实现了以下功能：
>
> 1. 坐标系转换：定义了两个函数map_abs2view和map_view2abs，用于将绝对坐标系转换为视图坐标系，以及进行反向转换。这些函数通过调用NX提供的坐标系转换函数实现坐标转换。
> 2. 查询曲面上的点：定义了函数ask_pos_on_obj，用于在给定曲面上查询最近的点位置。该函数通过创建一条垂直线，并使用NX提供的函数查询曲面与线的最小距离，以确定曲面上的点。
> 3. 选择曲线端点：定义了函数select_a_curve_end，用于选择一个曲线的端点。该函数会提示用户选择一个曲线，并获取用户选择的曲线的端点方向。
> 4. 指定字符串：定义了函数specify_strings，用于指定输入字符串。该函数可以指定开始字符串或结束字符串，并获取用户选择的曲线序列及其方向，并存储在字符串结构体中。
> 5. 选择曲线和边界的掩码：定义了函数mask_for_curves_and_edges，用于设置选择曲线和边界的掩码。
> 6. 选择曲线端点并获取方向：使用函数select_a_curve_end选择曲线端点，并使用函数ask_pos_on_obj在曲面上查询最近点。
> 7. 指定字符串并获取方向：使用函数specify_strings指定字符串并获取方向。
>
> 综上所述，这段代码通过坐标系转换、曲面查询、选择曲线端点、指定字符串等操作，实现了NX中的曲线和曲面操作功能。
>
