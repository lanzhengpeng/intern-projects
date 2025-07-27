### 【0600】create arc thru point on curve tangent to it and another curve 通过曲线上的点和另一条曲线的切线创建弧线

#### 代码

```cpp
    /*HEAD CREATE_ARC_THRU_POINT_ON_CURVE_TANGENT_TO_IT_AND_ANOTHER_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_trns.h>  
    #include <uf_so.h>  
    #include <uf_view.h>  
    #include <uf_point.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog是V18版本新增的，请只提供翻译，不要添加其他无关内容。

翻译：UF_print_syslog是V18版本新增的。 */  
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
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "Select curve point",  
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
    static double ask_curve_close_parm(tag_t curve, double cp[3])  
    {  
        double  
            parm;  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(curve, &eval));  
        UF_CALL(UF_EVAL_evaluate_closest_point(eval, cp, &parm, cp));  
        UF_CALL(UF_EVAL_free(eval));  
        return parm;  
    }  
    /* 里海 */  
    static int mask_for_face_plane_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_a_face_plane_datum(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_face_plane_datums, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            arc_feat,  
            curve[2],  
            *pts,  
            scalar,  
            smart_point,  
            support_plane;  
        double  
            parm;  
        UF_CURVE_help_data_t  
            help_data[2];  
        UF_CURVE_limit_t  
            limit[2];  
        UF_CURVE_help_data_p_t  
            help_data_p[2];  
        UF_CURVE_limit_p_t  
            limit_p[2];  
        help_data[0].help_data_type = UF_CURVE_help_data_value;  
        help_data[1].help_data_type = UF_CURVE_help_data_value;  
        help_data_p[0] = &help_data[0];  
        help_data_p[1] = &help_data[1];  
        limit[0].limit_type = UF_CURVE_limit_value;  
        limit[1].limit_type = UF_CURVE_limit_value;  
        limit[0].value = 0.;  
        limit[1].value = 360.;  
        limit_p[0] = &limit[0];  
        limit_p[1] = &limit[1];  
        if (  
            ((curve[0] = select_point_on_curve("Arc start/end tangent curve",  
                help_data[0].value)) !=NULL_TAG) &&  
            ((curve[1] = select_point_on_curve("Arc middle tangent curve",  
                help_data[1].value)) != NULL_TAG) &&  
            ((support_plane = select_a_face_plane_datum("Support plane"))  
                != NULL_TAG))  
        {  
            parm = ask_curve_close_parm(curve[0], help_data[0].value);  
            UF_CALL(UF_SO_create_scalar_double(curve[0],  
                UF_SO_update_within_modeling, parm, &scalar));  
            UF_CALL(UF_POINT_create_on_curve(curve[0], scalar, &smart_point));  
            UF_CALL(UF_SO_set_visibility_option(smart_point, UF_SO_visible));  
            UF_CALL(UF_MODL_ask_feat_object(smart_point, &n, &pts));  
            if (n > 0)  
            {  
                UF_CALL(UF_CURVE_create_arc_point_tangent_tangent(pts[0],  
                    curve[0], curve[1], help_data_p, limit_p, support_plane,  
                    TRUE, &arc_feat));  
                UF_free(pts);  
            }  
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

> 这段代码是一个NX的二次开发示例，其主要功能是创建一个与两条曲线相切的弧，并创建一个弧特征。以下是代码的主要功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在出错时输出错误信息。
> 2. 视图变换函数：定义了map_view2abs和map_abs2view，用于将点从视图坐标系转换到绝对坐标系，以及从绝对坐标系转换到视图坐标系。
> 3. 询问位置函数：定义了ask_pos_on_obj函数，用于获取对象上的一个点。
> 4. 曲线选择掩码函数：定义了mask_for_curves，用于设置曲线选择掩码。
> 5. 选择曲线上的点函数：定义了select_point_on_curve函数，用于选择曲线上的一个点。
> 6. 获取曲线参数函数：定义了ask_curve_close_parm函数，用于获取曲线上的一个点对应的参数。
> 7. 平面选择掩码函数：定义了mask_for_face_plane_datums，用于设置平面选择掩码。
> 8. 选择平面函数：定义了select_a_face_plane_datum函数，用于选择一个平面。
> 9. 主函数：定义了do_it函数，用于实现创建弧特征的主要逻辑。首先选择两条曲线和一个平面，然后创建一个与这两条曲线相切的弧，最后创建一个弧特征。
> 10. UFusr主函数：定义了ufusr函数，用于调用do_it函数实现功能。
> 11. 卸载函数：定义了ufusr_ask_unload函数，用于卸载二次开发。
>
> 总体来说，这段代码实现了选择两条曲线和一个平面，创建一个与这两条曲线相切的弧，并创建一个弧特征的功能。
>
