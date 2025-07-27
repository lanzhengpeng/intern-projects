### 【0825】create point along curve offset from selected curve end 创建曲线末端偏移点

#### 代码

```cpp
    /*HEAD CREATE_POINT_ALONG_CURVE_OFFSET_FROM_SELECTED_CURVE_END CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_view.h>  
    #include <uf_trns.h>  
    #include <uf_eval.h>  
    #include <uf_obj.h>  
    #include <uf_vec.h>  
    #include <uf_so.h>  
    #include <uf_point.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog是V18版本新增的功能，请只提供翻译，不要添加其他无关内容。

翻译：

注意：UF_print_syslog是V18版本新增的功能。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
    static void ask_curve_close_end_point(tag_t curve, double pos[3], int *which)  
    {  
        double  
            dist[2],  
            ends[6];  
        ask_curve_ends(curve, &ends[0], &ends[3]);  
        UF_VEC3_distance(pos, ends, &dist[0]);  
        UF_VEC3_distance(pos, &ends[3], &dist[1]);  
        if (dist[0] < dist[1])  
        {  
            UF_VEC3_copy(ends, pos);  
            *which = 1;  /*  Start 译:开始 */  
        }  
        else  
        {  
            UF_VEC3_copy(&ends[3], pos);  
            *which = -1;  /*  End 译:End */  
        }  
    }  
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
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
    static tag_t select_a_curve_end_point(char *prompt, double pos[3], int *which)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve endpoint", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            ask_curve_close_end_point(object, pos, which);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void display_temporary_shaped_point(double *coords, int shape)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = shape;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, shape));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            flip,  
            direction;  
        tag_t  
            curve,  
            distance,  
            end_pt,  
            point,  
            start;  
        double  
            end_parm,  
            junk[3],  
            offset = 0,  
            parm,  
            pos[3],  
            ref[3],  
            tolerance;  
        UF_MODL_ask_distance_tolerance(&tolerance);  
        while ((curve = select_a_curve_end_point("Offset along curve", ref,  
            &direction)) != NULL_TAG)  
            while (prompt_for_a_number("Offset", "Distance", &offset))  
            {  
                display_temporary_shaped_point(ref, UF_DISP_ASTERISK);  
                WRITE("From curve end (asterisk):\n");  
                WRITE3F(ref);  
            /*  this section may demonstrate PR 2070968 译:This section may demonstrate PR 2070968。 */  
                UF_CALL(UF_MODL_ask_point_along_curve(ref, curve, offset,  
                    direction, tolerance, &parm));  
                UF_CALL(UF_MODL_ask_curve_props(curve, parm, pos, junk, junk, junk,  
                    junk, junk));  
                display_temporary_shaped_point(pos, UF_DISP_CIRCLE);  
                WRITE("UF_MODL_ask_point_along_curve (circle):\n");  
                WRITE3F(pos);  
            /*  End PR 2070968 demo 译:End PR 2070968 demo */  
            /*  this section may demonstrate PR 2070577 译:这一节可能会证明PR 2070577。 */  
                UF_CALL(UF_MODL_ask_point_along_curve_2(ref, curve, offset,  
                    direction, tolerance, pos, &parm));  
                display_temporary_shaped_point(pos, UF_DISP_TRIANGLE_MARKER);  
                WRITE("UF_MODL_ask_point_along_curve_2 (triangle):\n");  
                WRITE3F(pos);  
            /*  End PR 2070577 demo 译:根据PR 2070577的demo翻译如下：

结束 PR 2070577 的 demo。 */  
            /*  This way works in NX 4 译:这个方法适用于NX 4。 */  
                if (direction == 1)  
                {  
                    end_parm = 0;  
                    flip = FALSE;  
                }  
                else  
                {  
                    end_parm = 1;  
                    flip = TRUE;  
                }  
                UF_CALL(UF_SO_create_scalar_double(curve,  
                    UF_SO_update_within_modeling, end_parm, &start));  
                UF_CALL(UF_SO_create_point_on_curve(curve,  
                    UF_SO_update_within_modeling, curve, start, &end_pt));  
                UF_CALL(UF_SO_create_scalar_double(curve,  
                    UF_SO_update_within_modeling, offset, &distance));  
                UF_CALL(UF_SO_create_point_along_curve(curve,  
                    UF_SO_update_within_modeling, curve, end_pt, distance,  
                    UF_SO_point_along_curve_distance, flip, &point));  
                UF_CALL(UF_SO_set_visibility_option(point, UF_SO_visible));  
                UF_CALL(UF_CURVE_ask_point_data(point, pos));  
                display_temporary_shaped_point(pos, UF_DISP_SQUARE);  
                WRITE("UF_SO_create_point_along_curve (square):\n");  
                WRITE3F(pos);  
            /*  This way also works in NX 4 译:NX 4中此方法也适用。 */  
                UF_CALL(UF_POINT_create_along_curve(curve, end_pt, distance,  
                    UF_SO_point_along_curve_distance, flip, &point));  
                UF_CALL(UF_POINT_ask_point_output(point, &point));  
                UF_CALL(UF_CURVE_ask_point_data(point, pos));  
                display_temporary_shaped_point(pos, UF_DISP_DIAMOND);  
                WRITE("UF_POINT_create_along_curve (diamond):\n");  
                WRITE3F(pos);  
                offset = offset + .01;  
            }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，主要功能是根据选择的曲线端点，创建一个沿曲线偏移一定距离的点。以下是代码的主要功能和流程：
>
> 1. 错误报告函数：实现了错误报告函数report_error，用于输出错误信息。
> 2. 坐标系转换函数：实现了坐标系转换函数map_abs2view和map_view2abs，用于将绝对坐标系下的点转换为视图坐标系下的点，反之亦然。
> 3. 曲线端点函数：实现了ask_pos_on_obj函数，用于在选择的曲线上获得一个点；ask_curve_ends函数，用于获取曲线的两个端点；ask_curve_close_end_point函数，用于确定选择的点更接近哪个端点。
> 4. 曲线选择函数：实现了mask_for_curves函数，用于设置曲线选择类型；select_a_curve_end_point函数，用于选择一个曲线的端点。
> 5. 数值提示函数：实现了prompt_for_a_number函数，用于提示用户输入一个数值。
> 6. 显示临时点函数：实现了display_temporary_shaped_point函数，用于在视图上显示一个临时的形状点。
> 7. 主函数：实现了do_it函数，是程序的主逻辑。首先选择曲线的一个端点，然后提示用户输入偏移距离。接着，使用不同方法创建偏移点，并显示出来。
> 8. 入口函数：实现了ufusr函数，是程序的入口函数。
> 9. 卸载函数：实现了ufusr_ask_unload函数，用于卸载程序。
>
> 总体来看，该代码实现了根据曲线端点创建偏移点的基本功能，并通过不同的方法来展示该功能。代码结构清晰，具有很好的参考价值。
>
