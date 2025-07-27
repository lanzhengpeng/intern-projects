### 【0908】create smart point on selected curve 在选择的曲线上创建智能点

#### 代码

```cpp
    /*HEAD CREATE_SMART_POINT_ON_SELECTED_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_trns.h>  
    #include <uf_so.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数，用于打印系统日志。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
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
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void do_it(void)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            curve,  
            scalar,  
            smart_point,  
            t_curve;  
        double  
            cp[3],  
            parm;  
        char  
            messg[133];  
        while ((curve=select_point_on_curve("Select curve point", cp))!=NULL_TAG)  
        {  
            display_temporary_point(cp);  
            UF_CALL(UF_OBJ_ask_type_and_subtype(curve, &type, &subtype));  
            if (type == UF_solid_type)  
            {  
                UF_CALL(UF_MODL_create_curve_from_edge(curve, &t_curve));  
                UF_CALL(UF_MODL_ask_curve_parm(t_curve, cp, &parm, cp));  
                UF_CALL(UF_OBJ_delete_object(t_curve));  
            }  
            else  
                UF_CALL(UF_MODL_ask_curve_parm(curve, cp, &parm, cp));  
            UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
                        parm, &scalar));  
            UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
                curve, scalar, &smart_point));  
            UF_CALL(UF_SO_set_visibility_option(smart_point, UF_SO_visible));  
            sprintf(messg, "%f", parm);  
            uc1601(messg, TRUE);  
        }  
    }  
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

> 这段代码是用于NX的二次开发，其主要功能是在用户选择的曲线上创建一个智能点。以下是代码的主要内容和功能：
>
> 1. 包含必要的NX头文件，并定义了错误报告函数report_error，用于打印错误信息。
> 2. 定义了视图坐标系到绝对坐标系的转换函数map_view2abs和绝对坐标系到视图坐标系的转换函数map_abs2view。
> 3. 定义了ask_pos_on_obj函数，用于在给定的曲线上创建一个临时直线，并计算点到曲线的最短距离。
> 4. 定义了mask_for_curves函数，用于设置选择过滤条件，只允许选择线、圆、圆锥曲线、样条线等。
> 5. 定义了select_point_on_curve函数，用于弹出选择对话框，让用户选择曲线上的一个点，并返回该点的坐标。
> 6. 定义了display_temporary_point函数，用于显示一个临时点。
> 7. 定义了do_it函数，是主逻辑函数。循环调用select_point_on_curve获取用户选择的点，然后在该点上创建一个智能点，其中智能点的参数是曲线上的参数。
> 8. ufusr函数是NX调用的主函数，调用do_it执行主要逻辑。
> 9. ufusr_ask_unload函数用于设置当用户卸载该应用程序时的行为。
>
> 总体而言，该代码实现了在用户选择的曲线上创建智能点的功能，用户可以通过选择曲线上的一个点来创建一个智能点。
>
