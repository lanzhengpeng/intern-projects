### 【2915】specify string 指定曲线串

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
    static int ask_curve_close_end(tag_t curve, double pos[3])  
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
        if (dist[0] < dist[1]) return UF_MODL_CURVE_START_FROM_BEGIN;  
        else return UF_MODL_CURVE_START_FROM_END;  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            *which_end = ask_curve_close_end(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_string(char *which, UF_STRING_t *string)  
    {  
        int  
            dirs[50],  
            jj = 0,  
            kk = -1;  
        char  
            prompt[81];  
        tag_t  
            curves[50];  
        do  
        {  
            if (kk > 0)  
            {  
                sprintf(prompt, "OK to finish selecting %s string", which);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else  
                UF_CALL(UF_UI_set_status(""));  
            kk++;  
            sprintf(prompt, "Select %s String - curve #%d", which, kk+1);  
            curves[kk] = select_a_curve_end(prompt, &dirs[kk]);  
            if (curves[kk] != NULL_TAG)  
                UF_CALL(UF_DISP_set_highlight(curves[kk], TRUE));  
        } while ((curves[kk] != NULL_TAG) && (kk < 50));  
        UF_CALL(UF_UI_set_status(""));  
        if (kk > 0)  
        {  
            UF_MODL_init_string_list(string);  
            UF_MODL_create_string_list(1, kk, string);  
            string->string[0] = kk;  
            string->dir[0] = dirs[0];  
            for (jj = 0; jj < kk; jj++)  
            {  
                string->id[jj] = curves[jj];  
                UF_CALL(UF_DISP_set_highlight(curves[jj], FALSE));  
            }  
            return TRUE;  
        }  
        return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于在NX中实现曲线串选择和处理的二次开发代码，主要包括以下内容：
>
> 1. 坐标系转换函数：map_abs2view和map_view2abs用于将绝对坐标系转换为视图坐标系，以及进行相反的转换。
> 2. 查询位置函数：ask_pos_on_obj用于在指定曲线上查询与给定位置最近的点。
> 3. 查询曲线端点函数：ask_curve_close_end用于判断给定位置距离曲线的起始端点还是终止端点更近。
> 4. 曲线选择函数：mask_for_curves用于设置选择掩码，仅允许选择直线、圆弧、圆锥曲线和样条曲线。
> 5. 端点选择函数：select_a_curve_end用于提示用户选择一个曲线的端点，并判断是起始端点还是终止端点。
> 6. 字符串指定函数：specify_string用于提示用户选择一个起始字符串或终止字符串，并返回字符串列表。
> 7. 提示用户选择起始字符串：提示用户选择起始字符串，并判断是起始端点还是终止端点。
> 8. 提示用户选择终止字符串：提示用户选择终止字符串，并判断是起始端点还是终止端点。
> 9. 创建字符串列表：创建字符串列表，并设置字符串的起始方向。
> 10. 设置字符串的起始方向：根据选择的端点设置字符串的起始方向。
>
> 综上所述，这段代码实现了在NX中通过交互方式选择曲线串的功能，包括选择起始曲线、终止曲线，以及设置起始方向等。
>
