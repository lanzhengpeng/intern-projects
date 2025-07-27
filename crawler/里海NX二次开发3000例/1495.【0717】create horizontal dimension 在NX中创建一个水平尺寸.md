### 【0717】create horizontal dimension 在NX中创建一个水平尺寸

#### 代码

```cpp
    /*HEAD CREATE_HORIZONTAL_DIMENSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，翻译如下：

UF_print_syslog 是 V18 中新增的函数，用于打印系统日志。该函数接收两个参数：第一个参数是指向日志消息的指针，第二个参数是消息的长度。该函数将日志消息发送到系统日志，以便进行记录和监控。 */  
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
    static void ask_curve_assoc_endpoint(tag_t curve, double pos[3], int *assoc)  
    {  
        double  
            dist[2],  
            ends[6];  
        ask_curve_ends(curve, ends);  
        UF_VEC3_distance(pos, ends, &dist[0]);  
        UF_VEC3_distance(pos, &ends[3], &dist[1]);  
        if (dist[0] < dist[1])  
            *assoc = UF_DRF_first_end_point;  
        else *assoc = UF_DRF_last_end_point;  
    }  
    static void ask_edge_assoc_endpoint(tag_t edge, double pos[3], int *assoc)  
    {  
        int  
            cnt;  
        double  
            parm[3],  
            end[3],  
            start[3];  
        UF_CALL(UF_MODL_ask_edge_verts(edge, start, end, &cnt));  
        UF_CALL(UF_MODL_create_curve_from_edge(edge, &edge));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, start, &parm[0], start));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, pos, &parm[1], pos));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, end, &parm[2], end));  
        UF_CALL(UF_OBJ_delete_object(edge));  
        if (fabs(parm[1] - parm[0]) < fabs(parm[2] - parm[1]))  
            *assoc = UF_DRF_first_end_point;  
        else *assoc = UF_DRF_last_end_point;  
    }  
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
    static tag_t select_drf_curve_endpoint(char *prompt, tag_t *view, int *assoc)  
    {  
        int  
            subtype,  
            resp,  
            type;  
        tag_t  
            object;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(FALSE));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_drf_curves, NULL, &resp,  
            &object, cp, view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, cp);  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if (type == UF_solid_type) ask_edge_assoc_endpoint(object, cp, assoc);  
            else ask_curve_assoc_endpoint(object, cp, assoc);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int resp;  
        UF_OBJ_disp_props_t  
            color = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_UI_set_cursor_view(TRUE));  
        resp = uc1615(prompt, pos);  
        if (resp == 5)  
        {  
            UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_CURSOR,  
                pos, &color, UF_DISP_ASTERISK));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100];  
        tag_t  
            dim;  
        double  
            mpr[70],  
            origin[3];  
        UF_DRF_object_t  
            object[2];  
        UF_DRF_text_t  
            text = { NULL, 0, NULL };  
        char  
            diameter[7],  
            radius[7];  
        UF_CALL(UF_DRF_init_object_structure(&object[0]));  
        UF_CALL(UF_DRF_init_object_structure(&object[1]));  
        UF_CALL(UF_DRF_ask_preferences(mpi, mpr, radius, diameter));  
        mpi[0] = 3; /* Manual placement, arrows out 译:手动放置，箭头朝外。 */  
        mpi[7] = 1; /* Only automatic text 译:Only automatic text. */  
        mpi[30] = 5; /* Mid-center 译:Mid-center翻译为中文为“中位线”。 */  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));  
        while (  
            ((object[0].object_tag =  
            select_drf_curve_endpoint("Select 1st endpoint",  
            &object[0].object_view_tag,  
            &object[0].object_assoc_modifier)) != NULL_TAG) &&  
            ((object[1].object_tag =  
            select_drf_curve_endpoint("Select 1st endpoint",  
            &object[1].object_view_tag,  
            &object[1].object_assoc_modifier)) != NULL_TAG) &&  
            (select_pos("Select origin", origin)))  
        {  
            UF_CALL(UF_DRF_create_horizontal_dim(&object[0], &object[1], &text,  
                origin, &dim));  
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

> 根据代码，这是一段用于在NX中创建水平尺寸标注的二次开发代码。主要功能包括：
>
> 1. 错误报告：定义了report_error函数，用于报告UF函数调用失败时的错误信息。
> 2. 坐标转换：map_abs2view和map_view2abs用于将绝对坐标系坐标转换为视图坐标系坐标，反之亦然。
> 3. 位置询问：ask_pos_on_obj用于在指定对象上询问位置。
> 4. 曲线端点询问：ask_curve_ends和ask_curve_assoc_endpoint用于询问曲线的端点信息。
> 5. 边缘端点询问：ask_edge_assoc_endpoint用于询问边缘的端点信息。
> 6. 选择过滤：mask_for_drf_curves用于设置选择过滤，只允许选择线、圆、圆锥、样条等曲线类型。
> 7. 端点选择：select_drf_curve_endpoint用于选择曲线端点。
> 8. 位置选择：select_pos用于选择位置。
> 9. 尺寸标注：do_it函数中包含创建水平尺寸标注的逻辑，包括设置偏好、选择两个端点和原点，然后调用UF_DRF_create_horizontal_dim创建尺寸标注。
> 10. 初始化和清理：ufusr函数用于初始化和清理。
>
> 总体来说，该代码实现了在NX中通过选择两个曲线端点和原点来自动创建水平尺寸标注的功能，通过二次开发提高了尺寸标注的效率。
>
