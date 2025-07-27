### 【2398】report true angle between selected curves 报告所选曲线之间的真实角度

#### 代码

```cpp
    /*HEAD REPORT_TRUE_ANGLE_BETWEEN_SELECTED_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_eval.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog在V18中是新增的。请仅提供翻译，不要添加其他无关内容。 */  
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
    /* qq3123197280 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve or edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void ask_curve_direction(tag_t curve, double loc[3], double dir[3])  
    {  
        double  
            binormal[3],  
            normal[3],  
            parm,  
            point[3];  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(curve, &eval));  
        UF_CALL(UF_EVAL_evaluate_closest_point(eval, loc, &parm, point));  
        UF_CALL(UF_EVAL_evaluate_unit_vectors(eval, parm, point, dir,  
            normal, binormal));  
        UF_CALL(UF_EVAL_free(eval));  
    }  
    static int ask_curve_close_end(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            parm;  
        UF_CALL(UF_MODL_ask_curve_parm(curve, pos, &parm, pos));  
        UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, parm, UF_MODL_UNITS_PART,  
            &dist[0]));  
        UF_CALL(UF_CURVE_ask_arc_length(curve, parm, 1.0, UF_MODL_UNITS_PART,  
            &dist[1]));  
        if (dist[0] < dist[1]) return 1;  
        else return 2;  
    }  
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve1,  
            curve2;  
        double  
            angle,  
            ccw[3],  
            dist,  
            loc1[3],  
            loc2[3],  
            dir1[3],  
            dir2[3];  
        char  
            msg[UF_UI_MAX_STRING_LEN];  
        while (((curve1 = select_a_curve_or_edge("Select first curve"))  
                    != NULL_TAG) &&  
               ((curve2 = select_a_curve_or_edge("Select second curve"))  
                    != NULL_TAG))  
        {  
            UF_DISP_refresh();  
            if (UF_CALL(UF_MODL_ask_minimum_dist(curve1, curve2, FALSE, NULL,  
                FALSE, NULL, &dist, loc1, loc2))) continue;  
            ask_curve_direction(curve1, loc1, dir1);  
            if (ask_curve_close_end(curve1, loc1) == 2)  
                UF_VEC3_negate(dir1, dir1);  
            display_conehead(loc1, dir1);  
            ask_curve_direction(curve2, loc2, dir2);  
            if (ask_curve_close_end(curve2, loc2) == 2)  
                UF_VEC3_negate(dir2, dir2);  
            display_conehead(loc2, dir2);  
            UF_VEC3_cross(dir1, dir2, ccw);  
            UF_VEC3_angle_between(dir1, dir2, ccw, &angle);  
            sprintf(msg, "The angle between the curves is %f\n", angle * RADEG);  
            uc1601(msg, TRUE);  
        }  
    }  
    /* qq3123197280 */  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 错误报告功能：定义了report_error函数，用于打印出错误信息，包括文件名、行号、函数调用和错误码。
> 2. 选择曲线或边：定义了select_a_curve_or_edge函数，用于提示用户选择一个曲线或边，并返回选择对象的tag。
> 3. 获取曲线方向：定义了ask_curve_direction函数，用于获取曲线在指定点的方向向量。
> 4. 确定曲线端点：定义了ask_curve_close_end函数，用于判断指定点是否为曲线的近端点。
> 5. 显示圆锥头：定义了display_conehead函数，用于在指定位置和方向显示圆锥头。
> 6. 主要功能实现：在do_it函数中，首先选择两个曲线，然后获取它们的最小距离点，并计算这两个点的方向向量，最后显示圆锥头并计算两向量之间的角度。
> 7. 初始化和终止：在ufusr函数中，进行初始化，调用do_it函数实现主要功能，然后终止。
> 8. 卸载函数：定义了ufusr_ask_unload函数，用于立即卸载用户自定义函数。
>
> 总的来说，这段代码实现了选择两个曲线，计算它们之间的最小距离，显示两个点的方向向量，并计算这两向量之间的角度。
>
