### 【0500】compare curves 比较曲线

#### 代码

```cpp
    /*HEAD COMPARE_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_eval.h>  
    #include <uf_vec.h>  
    #include <uf_mtx.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 新增的函数。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    extern tag_t select_a_curve_or_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static logical curve_curve_deviation(tag_t curve1, tag_t curve2, int n_pts,  
        double dist_tol, double angl_tol)  
    {  
        logical  
            is_equal,  
            overlays = TRUE;  
        int  
            ii,  
            n = 0,  
            over_dist = 0,  
            over_angl = 0;  
        double  
            a_angl1,  
            a_angl2,  
            angle1[3],  
            angle2[3],  
            avg_dist = 0.0,  
            avg_angl = 0.0,  
            d_angl,  
            d_dist,  
            limits[2],  
            max_angl = 0.0,  
            max_dist = 0.0,  
            min_angl = TWOPI,  
            min_dist = 1000000,  
            mtx[9],  
            p1,  
            p2,  
            p3,  
            p_step,  
            point1[3],  
            point2[3],  
            point3[3],  
            t_angl = 0,  
            t_dist = 0;  
        UF_EVAL_p_t  
            eval_p1,  
            eval_p2;  
        UF_CALL(UF_EVAL_initialize(curve1, &eval_p1));  
        UF_CALL(UF_EVAL_initialize(curve2, &eval_p2));  
        UF_CALL(UF_EVAL_is_equal(eval_p1, eval_p2, &is_equal));  
        if (!is_equal)  
        {  
            UF_CALL(UF_EVAL_ask_limits(eval_p1, limits));  
            p_step = ((limits[1] - limits[0])/(n_pts - 1));  
            for (ii = 0; ii < n_pts; ii++)  
            {  
                p1 = limits[0] + ii * p_step;  
                UF_CALL(UF_EVAL_evaluate(eval_p1, 1, p1, point1, angle1));  
                UF_CALL(UF_EVAL_evaluate_closest_point(eval_p2,point1,&p2,point2));  
            /*  the proximity must be reciprocal to within a half step 译:必须相互靠近半步。 */  
                UF_CALL(UF_EVAL_evaluate_closest_point(eval_p1,point2,&p3,point3));  
                if ((fabs(p1 - p3)) > (p_step/2))  
                {  
                    display_temporary_colored_point(point1, UF_OBJ_YELLOW);  
                    overlays = FALSE;  
                    continue;  
                }  
                n = n + 1;  
                UF_CALL(UF_EVAL_evaluate(eval_p2, 1, p2, point2, angle2));  
                UF_VEC3_distance(point1, point2, &d_dist);  
                t_dist = t_dist + d_dist;  
                if (d_dist > max_dist) max_dist = d_dist;  
                if ((d_dist < min_dist) || (ii == 0)) min_dist = d_dist;  
                if (d_dist > dist_tol) over_dist = over_dist + 1;  
                UF_CALL(UF_MTX3_initialize_x(angle1, mtx));  
                UF_VEC3_angle_between(angle1, angle2, &mtx[6], &d_angl);  
                a_angl1 = fabs(d_angl - PI);  
                a_angl2 = fabs(d_angl - TWOPI);  
                if (a_angl1 < d_angl) d_angl = a_angl1;  
                if (a_angl2 < d_angl) d_angl = a_angl2;  
                d_angl = d_angl * RADEG;  
                t_angl = t_angl + d_angl;  
                if (d_angl > angl_tol) over_angl = over_angl + 1;  
                if (d_angl > max_angl) max_angl = d_angl;  
                if ((d_angl < min_angl) || (ii == 0)) min_angl = d_angl;  
                if ((d_dist > dist_tol) || (d_angl > angl_tol))  
                {  
                    overlays = FALSE;  
                    display_temporary_colored_point(point1, UF_OBJ_RED);  
                    display_temporary_colored_point(point2, UF_OBJ_RED);  
                }  
                else  
                    display_temporary_colored_point(point1, UF_OBJ_GREEN);  
            }  
            avg_dist = t_dist / n;  
            avg_angl = t_angl / n;  
        }  
        WRITE_D(curve1);  
        WRITE_D(curve2);  
        WRITE_D(n);  
        WRITE_F(dist_tol);  
        WRITE_D(over_dist);  
        WRITE_F(min_dist);  
        WRITE_F(max_dist);  
        WRITE_F(avg_dist);  
        WRITE_F(angl_tol);  
        WRITE_D(over_angl);  
        WRITE_F(min_angl);  
        WRITE_F(max_angl);  
        WRITE_F(avg_angl);  
        WRITE_L(overlays);  
        UF_CALL(UF_EVAL_free(eval_p1));  
        UF_CALL(UF_EVAL_free(eval_p2));  
        return overlays;  
    }  
    static logical prompt_for_an_integer_and_two_reals(char *prompt, char *item1,  
        char *item2, char *item3, int *number1, double *number2, double *number3)  
    {  
        int  
            ia[3],  
            ip[3] = { 101, 201, 201 },  
            resp;  
        double  
            da[3];  
        char  
            ca[3][31],  
            menu[3][16];  
        strcpy(&menu[0][0], item1);  
        strcpy(&menu[1][0], item2);  
        strcpy(&menu[2][0], item3);  
        ia[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        resp = uc1613(prompt, menu, 3, ia, da, ca, ip);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = ia[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            total_overlay1,  
            total_overlay2;  
        int  
            n = 50;  
        tag_t  
            first,  
            second;  
        double  
            angl_tol,  
            dist_tol;  
        UF_MODL_ask_distance_tolerance(&dist_tol);  
        UF_MODL_ask_angle_tolerance(&angl_tol);  
        while (((first=select_a_curve_or_edge("Select first object"))!=NULL_TAG) &&  
            ((second = select_a_curve_or_edge("Select second object"))!=NULL_TAG)  
            && prompt_for_an_integer_and_two_reals("Specify parameters?",  
            "Check Points", "Distance Tol", "Angle Tol", &n, &dist_tol, &angl_tol))  
        {  
            total_overlay1 = curve_curve_deviation(first, second, n, dist_tol,  
                angl_tol);  
            if (total_overlay1)  
                uc1601("First overlays second", TRUE);  
            else  
                uc1601("First does not totally overlay second", TRUE);  
            UF_DISP_refresh();  
            total_overlay2 = curve_curve_deviation(second, first, n, dist_tol,  
                angl_tol);  
            if (total_overlay2)  
                uc1601("Second overlays first", TRUE);  
            else  
                uc1601("Second does not totally overlay first", TRUE);  
            UF_DISP_refresh();  
            if (total_overlay1 && total_overlay2)  
                uc1601("Curves are the same", TRUE);  
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

> 这段代码是用于比较两条曲线在距离和角度公差范围内的相似度的NX Open C++二次开发示例代码。
>
> 主要功能包括：
>
> 1. 使用UF_EVAL API评估曲线上的点，并计算这些点之间的距离和角度偏差。
> 2. 提供用户交互界面，让用户选择要比较的两条曲线，并设置检查点的数量、距离公差和角度公差。
> 3. 对比两条曲线是否在设置的公差范围内完全重叠，并在界面上高亮显示偏差超过公差范围的点。
> 4. 如果两条曲线在公差范围内完全重叠，则提示用户"Curves are the same"。
> 5. 提供了错误处理和日志输出。
> 6. 提供了卸载函数。
>
> 总的来说，这段代码实现了比较两条曲线在指定公差范围内的相似度的功能，并提供了良好的用户交互。
>
