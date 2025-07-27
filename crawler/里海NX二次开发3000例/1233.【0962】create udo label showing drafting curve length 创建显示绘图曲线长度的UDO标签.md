### 【0962】create udo label showing drafting curve length 创建显示绘图曲线长度的UDO标签

#### 代码

```cpp
    /*HEAD CREATE_UDO_LABEL_SHOWING_DRAFTING_CURVE_LENGTH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_exit.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_trns.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，UF_print_syslog是V18版本新增的函数。 */  
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
    static double ask_curve_length(tag_t curve)  
    {  
        double  
            length;  
        UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, 1.0, UF_MODL_UNITS_PART,  
            &length));  
        return length;  
    }  
    /* qq3123197280 */  
    DllExport extern void update_func(tag_t update_udo,  
        UF_UDOBJ_link_p_t update_cause)  
    {  
        int  
            n;  
        tag_t  
            *owned;  
        char  
            text[1][133];  
        if (UF_CALL(UF_initialize())) return;  
        sprintf(&text[0][0], "%.2f", ask_curve_length(update_cause->assoc_ug_tag));  
        UF_CALL(UF_UDOBJ_ask_owned_objects(update_udo, &n, &owned));  
        if (n > 0)  
        {  
            uc5566(owned[0], 1, 1, text);  
            UF_free(owned);  
        }  
        UF_terminate();  
    }  
    static void register_udo_class()  
    {  
        UF_UDOBJ_class_t  
            class_id;  
        UF_CALL(UF_UDOBJ_create_class("MY_test", "my UDOs", &class_id));  
        UF_CALL(UF_UDOBJ_set_query_class_id(class_id,  
            UF_UDOBJ_ALLOW_QUERY_CLASS_ID));  
        UF_CALL(UF_UDOBJ_register_update_cb(class_id, update_func));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        register_udo_class();  
        UF_terminate();  
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
    /* qq3123197280 */  
    static int mask_for_drf_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[7] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_solid_silhouette_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 7, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical select_drf_curve_pos(char *prompt, UF_DRF_object_p_t curve,  
        double cp[3])  
    {  
        int  
            resp;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select point on curve", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_curves, NULL, &resp,  
            &curve->object_tag, cp, &curve->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(curve->object_tag, cp);  
            UF_CALL(UF_DISP_set_highlight(curve->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            label,  
            link_udo;  
        double  
            pos[3],  
            origin[3];  
        char  
            text[1][MAX_LINE_LENGTH + 1];  
        UF_UDOBJ_class_t  
            class_id;  
        UF_UDOBJ_link_t  
            link_def = { 3, NULL_TAG, FALSE };  
        UF_DRF_object_t  
            object;  
        UF_CALL(UF_UDOBJ_ask_class_id_of_name("MY_test", &class_id));  
        UF_CALL(UF_DRF_init_object_structure(&object));  
        while (select_drf_curve_pos("Link to udo", &object, pos))  
        {  
            UF_CALL(UF_UDOBJ_create_udo(class_id, &link_udo));  
            link_def.assoc_ug_tag = object.object_tag;  
            UF_CALL(UF_UDOBJ_add_links(link_udo, 1, &link_def));  
            sprintf(&text[0][0], "%.2f", ask_curve_length(object.object_tag));  
            UF_CALL(UF_VIEW_map_model_to_drawing(object.object_view_tag, pos,  
                origin));  
            origin[0] = origin[0] + 1.0;  
            origin[1] = origin[1] + 0.5;  
            origin[2] = pos[2];  
            UF_CALL(UF_DRF_create_label(1, text, origin,  
                UF_DRF_leader_attach_object, &object, pos, &label));  
            UF_CALL(UF_UDOBJ_add_owning_links(link_udo, 1, &label));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要实现以下功能：
>
> 1. 创建一个自定义数据对象(UDO)类，用于关联曲线和显示曲线长度。
> 2. 用户可以交互选择曲线，为每条选择的曲线创建一个UDO对象，并将曲线长度显示在曲线附近。
> 3. 当曲线长度改变时，UDO对象会自动更新显示的长度。
> 4. 使用NX的制图功能在视图窗口中绘制曲线长度标签。
> 5. 定义了自定义数据对象类的更新回调函数，当关联的曲线长度改变时，会调用该函数，更新显示的长度。
> 6. 提供交互函数选择曲线，计算曲线长度，并在视图窗口中绘制长度标签。
> 7. 使用NX的视图变换功能，将模型坐标转换为视图坐标。
> 8. 定义选择掩码，只允许选择线、圆、圆锥、样条曲线等曲线类型。
> 9. 使用NX的制图功能，将曲线长度标签附着到曲线上。
> 10. 使用NX的图形高亮功能，选择曲线时高亮显示。
>
> 总体来说，这段代码通过NX的二次开发接口，实现了与NX交互，动态显示曲线长度的功能。
>
