### 【0715】create horizontal dimension from curve end to midpoint 创建从曲线端点到中点的水平尺寸

#### 代码

```cpp
    /*HEAD CREATE_HORIZONTAL_DIMENSION_FROM_CURVE_END_TO_MIDPOINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_trns.h>  
    #include <uf_vec.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文中内容，UF_print_syslog 是 V18 版本新增的函数，用于打印系统日志。 */  
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
    static void ask_curve_ends(tag_t curve, double ends[6])  
    {  
        double  
            junk[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, &ends[0], junk, junk, junk,  
            junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, &ends[3], junk, junk, junk,  
            junk, junk));  
    }  
    static int ask_which_curve_endpoint(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            ends[6];  
        ask_curve_ends(curve, ends);  
        UF_VEC3_distance(pos, ends, &dist[0]);  
        UF_VEC3_distance(pos, &ends[3], &dist[1]);  
        if (dist[0] < dist[1]) return UF_DRF_first_end_point;  
        else return UF_DRF_last_end_point;  
    }  
    /* 里海 */  
    static int mask_for_drf_curves(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            mask[7] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, UF_all_subtype },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_solid_silhouette_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 7, mask)))  
            return (UF_UI_SEL_FAILURE);  
        else  
            return (UF_UI_SEL_SUCCESS);  
    }  
    static tag_t select_drf_curve_end(char *prompt, tag_t *view, int *assoc_mod)  
    {  
        int  
            resp;  
        double  
            pos[3];  
        tag_t  
            object;  
        UF_CALL(UF_UI_set_cursor_view(FALSE));  
        UF_CALL(UF_UI_select_with_single_dialog("Select curve end", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_drf_curves, NULL, &resp,  
            &object, pos, view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            *assoc_mod = ask_which_curve_endpoint(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_position(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void create_smart_point_at_midpoint(tag_t curve, tag_t *smart_point)  
    {  
        tag_t  
            scalar;  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.5, &scalar));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
            curve, scalar, smart_point));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            dim;  
        double  
            origin[3];  
        UF_DRF_object_t  
            object[2];  
        UF_DRF_text_t  
            text = { NULL, 0, NULL };  
        UF_CALL(UF_DRF_init_object_structure(&object[0]));  
        UF_CALL(UF_DRF_init_object_structure(&object[1]));  
        while (((object[0].object_tag =  
            select_drf_curve_end("Select curve to dimension",  
            &object[0].object_view_tag,  
            &object[0].object_assoc_modifier)) != NULL_TAG) &&  
            specify_position("Specify dimension origin", origin))  
        {  
            UF_CALL(UF_VIEW_expand_view(object[0].object_view_tag));  
            create_smart_point_at_midpoint(object[0].object_tag,  
                &object[1].object_tag);  
            UF_CALL(UF_VIEW_unexpand_work_view());  
            object[1].object_view_tag = object[0].object_view_tag;  
            object[1].object_assoc_type = UF_DRF_end_point;  
            object[1].object_assoc_modifier = 0;  
            UF_CALL(UF_DRF_create_horizontal_dim(&object[0], &object[1], &text,  
                origin, &dim));  
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

> 这段代码是一个NX的二次开发程序，主要用于创建一个从曲线端点到中点的水平尺寸。以下是代码的主要功能和步骤：
>
> 1. 功能介绍：该代码通过用户交互选择一个曲线，并指定一个原点位置，然后在该曲线的中点创建一个智能点，并创建一个从曲线端点到中点的水平尺寸。
> 2. 主要步骤：初始化UF模块。在用户交互中选择曲线，并确定尺寸的放置原点。扩展视图以适应选择的曲线。在曲线的中点创建一个智能点。收缩视图回到初始状态。使用选择的曲线和智能点创建水平尺寸。循环，直到用户取消选择。终止UF模块。
> 3. 初始化UF模块。
> 4. 在用户交互中选择曲线，并确定尺寸的放置原点。
> 5. 扩展视图以适应选择的曲线。
> 6. 在曲线的中点创建一个智能点。
> 7. 收缩视图回到初始状态。
> 8. 使用选择的曲线和智能点创建水平尺寸。
> 9. 循环，直到用户取消选择。
> 10. 终止UF模块。
> 11. 关键函数：select_drf_curve_end：用于选择曲线端点。ask_which_curve_endpoint：确定用户选择的是曲线的哪个端点。create_smart_point_at_midpoint：在曲线的中点创建一个智能点。UF_DRF_create_horizontal_dim：创建水平尺寸。
> 12. select_drf_curve_end：用于选择曲线端点。
> 13. ask_which_curve_endpoint：确定用户选择的是曲线的哪个端点。
> 14. create_smart_point_at_midpoint：在曲线的中点创建一个智能点。
> 15. UF_DRF_create_horizontal_dim：创建水平尺寸。
> 16. 注意事项：使用了UF库的视图、曲线、尺寸等相关函数。通过UF的错误处理机制来处理UF函数的调用错误。使用了UF的对话框来进行用户交互。通过UF的视图变换来处理模型空间和视图空间之间的转换。
> 17. 使用了UF库的视图、曲线、尺寸等相关函数。
> 18. 通过UF的错误处理机制来处理UF函数的调用错误。
> 19. 使用了UF的对话框来进行用户交互。
> 20. 通过UF的视图变换来处理模型空间和视图空间之间的转换。
>
> 综上所述，这段代码实现了在NX中创建曲线端点到中点的水平尺寸的功能，同时采用了NX提供的UF库来进行二次开发。
>
