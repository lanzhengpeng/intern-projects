### 【0670】create dplane at curve parameter 在曲线参数处创建D平面

#### 代码

```cpp
    /*HEAD CREATE_DPLANE_AT_CURVE_PARAMETER CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是一个在 V18 版本中新增的函数，用于打印系统日志。文档要求只提供翻译，不添加任何其他无关内容。 */  
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
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_ASTERISK));  
    }  
    static void do_it(void)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            curve,  
            dplane,  
            scalar,  
            smart_dirr,  
            smart_point,  
            t_curve;  
        double  
            cp[3],  
            parm;  
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
            if (prompt_for_a_number("Modify Point", "Parameter", &parm))  
            {  
                UF_CALL(UF_SO_create_scalar_double(curve,  
                    UF_SO_update_within_modeling, parm, &scalar));  
                UF_CALL(UF_SO_create_point_on_curve(curve,  
                    UF_SO_update_within_modeling, curve, scalar, &smart_point));  
                UF_CALL(UF_SO_set_visibility_option(smart_point, UF_SO_invisible));  
                UF_CALL(UF_SO_create_dirr_on_curve(curve,  
                    UF_SO_update_within_modeling, curve, scalar,  
                    UF_SO_dirr_on_curve_tangent, FALSE, &smart_dirr));  
                UF_CALL(UF_SO_set_visibility_option(smart_dirr, UF_SO_invisible));  
                UF_CALL(UF_MODL_create_point_dirr_dplane(smart_point, smart_dirr,  
                    &dplane));  
            }  
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

> 这段代码是一个NX二次开发示例，用于创建一个平面在曲线参数位置的功能。具体来说：
>
> 1. 错误报告函数：定义了report_error函数，用于在发生UF函数调用错误时打印错误信息。
> 2. 坐标变换函数：提供了map_view2abs和map_abs2view两个函数，用于视图坐标系和绝对坐标系之间的转换。
> 3. 交互式选择曲线点：定义了ask_pos_on_obj函数，用于获取对象上的一个点。
> 4. 曲线选择过滤：mask_for_curves函数用于过滤选择对话框中的曲线类型。
> 5. 交互式选择曲线点：select_point_on_curve函数用于交互式选择曲线上的一个点。
> 6. 提示输入参数：prompt_for_a_number函数用于提示用户修改参数值。
> 7. 临时显示点：display_temporary_point函数用于临时显示一个点。
> 8. 创建参数平面：do_it函数用于创建参数平面，并包含创建参数、参数点、参数方向等智能对象。
> 9. 初始化和结束函数：ufusr函数用于初始化和结束UF。
> 10. 卸载请求：ufusr_ask_unload函数用于返回卸载请求。
>
> 总体来看，该代码通过用户交互式选择曲线上的点，并提示修改参数值，最终创建一个平面在指定曲线参数位置。
>
