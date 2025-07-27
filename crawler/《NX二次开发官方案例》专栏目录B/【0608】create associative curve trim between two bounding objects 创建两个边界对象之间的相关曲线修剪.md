### 【0608】create associative curve trim between two bounding objects 创建两个边界对象之间的相关曲线修剪

#### 代码

```cpp
    /*HEAD CREATE_ASSOCIATIVE_CURVE_TRIM_BETWEEN_TWO_BOUNDING_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：请注意，UF_print_syslog 是 V18 新增的函数。 */  
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
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
        if (dist[0] < dist[1]) return UF_CURVE_TRIM_EXTEND_START;  
        else return UF_CURVE_TRIM_EXTEND_END;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            trim_feat;  
        UF_CURVE_trim_t  
            trim_info;  
        UF_CURVE_trim_mult_t  
            out_info;  
        UF_MODL_init_string_list(&trim_info.string_to_trim);  
        UF_MODL_create_string_list(1, 1, &trim_info.string_to_trim);  
        trim_info.string_to_trim.string[0] = 1;  
        trim_info.string_to_trim.dir[0] = 1;  
        trim_info.string_to_trim.id[0] = NULL_TAG;  
        trim_info.spline_extend_opt = UF_CURVE_EXTEND_NATURAL;  
        trim_info.trim_type = UF_CURVE_TRIM_TO_TWO_BOUND;  
        UF_MODL_ask_distance_tolerance(&trim_info.tolerances[0]);  
        UF_MODL_ask_angle_tolerance(&trim_info.tolerances[1]);  
        UF_MODL_init_string_list(&trim_info.trim_to.two_bound.bound1.string);  
        UF_MODL_create_string_list(1,1,&trim_info.trim_to.two_bound.bound1.string);  
        trim_info.trim_to.two_bound.bound1.string.string[0] = 1;  
        trim_info.trim_to.two_bound.bound1.string.dir[0] = 1;  
        trim_info.trim_to.two_bound.bound1.string.id[0] = NULL_TAG;  
        trim_info.trim_to.two_bound.bound1.object = NULL_TAG;  
        trim_info.trim_to.two_bound.bound1.use_suggested = TRUE;  
        UF_MODL_init_string_list(&trim_info.trim_to.two_bound.bound2.string);  
        UF_MODL_create_string_list(1,1,&trim_info.trim_to.two_bound.bound2.string);  
        trim_info.trim_to.two_bound.bound2.string.string[0] = 1;  
        trim_info.trim_to.two_bound.bound2.string.dir[0] = 1;  
        trim_info.trim_to.two_bound.bound2.string.id[0] = NULL_TAG;  
        trim_info.trim_to.two_bound.bound2.object = NULL_TAG;  
        trim_info.trim_to.two_bound.bound2.use_suggested = TRUE;  
        trim_info.trim_to.two_bound.string_trim_option = UF_CURVE_TRIM_INSIDE;  
        trim_info.trim_to.two_bound.trim_bound = FALSE;  
        trim_info.trim_to.two_bound.view = NULL;  
        while (((trim_info.string_to_trim.id[0] =  
                    select_a_curve("Select curve to trim")) != NULL_TAG) &&  
                ((trim_info.trim_to.two_bound.bound1.string.id[0] =  
                    select_point_on_curve("Select first bounding curve",  
                    trim_info.trim_to.two_bound.bound1.suggested_point))  
                        != NULL_TAG) &&  
                ((trim_info.trim_to.two_bound.bound2.string.id[0] =  
                    select_point_on_curve("Select second bounding curve",  
                    trim_info.trim_to.two_bound.bound2.suggested_point))  
                        != NULL_TAG))  
        {  
            trim_info.trim_to.two_bound.string_extend_end = ask_curve_close_end(  
                trim_info.string_to_trim.id[0],  
                trim_info.trim_to.two_bound.bound1.suggested_point);  
            if (UF_CALL(UF_CURVE_create_trim(&trim_info, &out_info, &trim_feat))  
                == UF_CURVE_TRIM_MULT_PTS)  
            {  
                if (out_info.num_bound1_pts > 0) UF_free(out_info.bound1_pts);  
                if (out_info.num_bound2_pts > 0) UF_free(out_info.bound2_pts);  
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

> 这段代码是一个NX Open C++应用程序，用于在两个边界对象之间创建关联曲线修剪特征。
>
> 主要功能包括：
>
> 1. 提供错误报告函数report_error，用于在发生UF函数调用错误时打印错误信息。
> 2. 提供视图坐标系到绝对坐标系的转换函数map_view2abs和绝对坐标系到视图坐标系的转换函数map_abs2view。
> 3. 提供ask_pos_on_obj函数，用于在给定对象上查询与指定点最近的点。
> 4. 提供mask_for_curves函数，用于设置选择遮罩，以便只选择线、圆、圆锥、样条线和实体边线。
> 5. 提供select_point_on_curve函数，用于让用户选择曲线上的点。
> 6. 提供select_a_curve函数，用于让用户选择一条曲线。
> 7. 提供ask_curve_close_end函数，用于判断曲线端点距离指定点更近。
> 8. 在do_it函数中，循环提示用户选择待修剪曲线、第一个边界曲线上的点、第二个边界曲线上的点，然后创建修剪特征。
> 9. 提供ufusr函数，在NX启动时调用do_it函数。
> 10. 提供ufusr_ask_unload函数，在NX卸载应用程序时返回立即卸载。
>
> 总体而言，该应用程序实现了在两个边界对象之间创建关联曲线修剪特征的功能，通过交互式选择曲线和点来实现。
>
