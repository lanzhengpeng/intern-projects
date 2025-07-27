### 【1448】report curve parameter 选择曲线上的点并报告曲线参数

#### 代码

```cpp
    /*HEAD REPORT_CURVE_PARAMETER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数，用于打印系统日志。 */  
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
            curve;  
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
                UF_CALL(UF_MODL_create_curve_from_edge(curve, &curve));  
                UF_CALL(UF_MODL_ask_curve_parm(curve, cp, &parm, cp));  
                UF_CALL(UF_OBJ_delete_object(curve));  
            }  
            else  
                UF_CALL(UF_MODL_ask_curve_parm(curve, cp, &parm, cp));  
            sprintf(messg, "%f", parm);  
            uc1601(messg, TRUE);  
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

> 这段NX Open C++代码的主要功能是获取用户选择的曲线上的点，并计算出该点在曲线上的参数值。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义视图坐标系到绝对坐标系的转换函数map_view2abs和绝对坐标系到视图坐标系的转换函数map_abs2view。
> 3. 定义ask_pos_on_obj函数，用于在指定的曲线上获取离给定点最近的点。
> 4. 定义mask_for_curves函数，用于设置选择过滤条件，只允许选择线、圆、圆锥曲线、样条线和实体边。
> 5. 定义select_point_on_curve函数，用于弹出选择对话框，让用户选择一个点，并返回该点在曲线上的最近点。
> 6. 定义display_temporary_point函数，用于高亮显示选择的点。
> 7. 定义do_it函数，主函数，循环让用户选择曲线上的点，然后显示该点，并计算参数值。
> 8. 定义ufusr函数，为NX提供的入口函数，在这里调用do_it函数。
> 9. 定义ufusr_ask_unload函数，用于卸载用户函数。
>
> 通过以上步骤，用户可以方便地选择曲线上的点，并查看该点的参数值。代码结构清晰，注释详细，易于理解和使用。
>
