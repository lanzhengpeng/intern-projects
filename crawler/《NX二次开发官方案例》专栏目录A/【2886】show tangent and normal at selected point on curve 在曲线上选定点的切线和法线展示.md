### 【2886】show tangent and normal at selected point on curve 在曲线上选定点的切线和法线展示

#### 代码

```cpp
    /*HEAD SHOW_TANGENT_AND_NORMAL_AT_SELECTED_POINT_ON_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_eval.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_trns.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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

注意：UF_print_syslog 是 V18 新增的。

只回答译文，不要废话。 */  
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
    /* qq3123197280 */  
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
    static tag_t select_point_on_curve(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, cp);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_colored_conehead(double origin[3], double vec[3], int color)  
    {  
        UF_DISP_conehead_attrb_s  
            attrb;  
        UF_DISP_get_conehead_attrb(&attrb);  
        attrb.color = color;  
        UF_DISP_set_conehead_attrb(&attrb);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_DISP_reset_conehead_attrb();  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve;  
        double  
            binormal[3],  
            loc[3],  
            normal[3],  
            parm,  
            point[3],  
            tangent[3];  
        UF_EVAL_p_t  
            eval;  
        while ((curve = select_point_on_curve("Show normal", loc)) != NULL_TAG)  
        {  
            UF_CALL(UF_EVAL_initialize(curve, &eval));  
            UF_CALL(UF_EVAL_evaluate_closest_point(eval, loc, &parm, point));  
            UF_CALL(UF_EVAL_evaluate_unit_vectors(eval, parm, point, tangent,  
                normal, binormal));  
            display_colored_conehead(point, tangent, UF_OBJ_GREEN);  
            display_colored_conehead(point, normal, UF_OBJ_RED);  
            UF_CALL(UF_EVAL_free(eval));  
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

> 这段NX二次开发代码的主要功能是显示用户选择的曲线上的点处的切线和法线。下面是代码的主要步骤：
>
> 1. 包含必要的NX Open头文件。
> 2. 定义错误报告函数report_error，用于打印出函数调用失败的错误信息。
> 3. 定义坐标系转换函数map_view2abs和map_abs2view，用于将视图坐标系下的点转换到绝对坐标系，以及相反的转换。
> 4. 定义ask_pos_on_obj函数，用于在指定曲线上找到一个离给定点最近的点。
> 5. 定义mask_for_curves函数，用于设置选择曲面的类型掩码。
> 6. 定义select_point_on_curve函数，用于提示用户选择一个曲面，并返回选择曲面上离给定点最近的点。
> 7. 定义display_colored_conehead函数，用于显示一个指定颜色和方向的锥头。
> 8. 定义do_it函数，用于执行整个功能：选择曲面、找到最近的点、计算切线和法线、并显示锥头。
> 9. 定义ufusr函数，用于初始化NX Open，执行do_it函数，然后终止NX Open。
> 10. 定义ufusr_ask_unload函数，返回立即卸载标志。
>
> 综上，这段代码实现了NX中显示曲面某点处的切线和法线的功能。通过选择曲面和点，可以直观地看到曲面的切线和法线方向。
>
