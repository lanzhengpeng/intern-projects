### 【0716】create horizontal dimension with appended text 创建水平尺寸并附加文本

#### 代码

```cpp
    /*HEAD CREATE_HORIZONTAL_DIMENSION_WITH_APPENDED_TEXT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本中的新功能。 */  
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
            mpi_old[100],  
            mpi[100];  
        tag_t  
            dim;  
        double  
            mpr[70],  
            origin[3];  
        char  
            apptxt[1][132+1] = { "TYP" },  
            diameter[7],  
            radius[7];  
        UF_DRF_object_t  
            object[2];  
        UF_DRF_text_t  
            text;  
        text.user_dim_text = NULL;  
        text.lines_app_text = 1;  
        text.appended_text = (char (*)[])apptxt;  
    /*  the type casting above is necessary prior to NX 2.0 - see PR 4659240 译:上面提到的类型转换在NX 2.0之前是必要的 - 参见PR 4659240。 */  
        UF_CALL(UF_DRF_init_object_structure(&object[0]));  
        UF_CALL(UF_DRF_init_object_structure(&object[1]));  
        UF_CALL(UF_DRF_ask_preferences(mpi_old, mpr, radius, diameter));  
        UF_CALL(UF_DRF_ask_preferences(mpi, mpr, radius, diameter));  
        mpi[7] = 2; /* AUTOMATIC TEXT and APPENDED TEXT 译:AUTOMATIC TEXT: 自动文本
APPENDED TEXT: 附加文本 */  
        mpi[8] = 4; /* Appended text BEFORE 译:根据文中内容，翻译为：

在正文之前附加的文本。 */  
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
        UF_CALL(UF_DRF_set_preferences(mpi_old, mpr, radius, diameter));  
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

> 这段NX二次开发代码的主要功能是创建水平尺寸标注，并在标注文本后追加自定义文本。主要步骤包括：
>
> 1. 包含必要的NX Open头文件，并定义了错误报告函数report_error()，用于打印错误信息。
> 2. 定义了两个坐标转换函数map_abs2view()和map_view2abs()，用于将绝对坐标转换为视图坐标，以及将视图坐标转换回绝对坐标。
> 3. 定义了两个询问函数ask_pos_on_obj()和ask_curve_ends()，用于获取对象上的指定位置和曲线的端点。
> 4. 定义了两个函数ask_curve_assoc_endpoint()和ask_edge_assoc_endpoint()，用于确定曲线或边线的指定位置更接近哪个端点。
> 5. 定义了选择函数mask_for_drf_curves()和select_drf_curve_endpoint()，用于选择曲线或边线的端点。
> 6. 定义了select_pos()函数，用于选择标注原点。
> 7. 在do_it()函数中，先获取并保存当前标注参数，然后设置自动生成文本和追加文本的参数，进入循环，依次选择两个端点和标注原点，然后调用UF_DRF_create_horizontal_dim()创建水平标注，最后恢复原始标注参数。
> 8. 在ufusr()函数中初始化并调用do_it()，在ufusr_ask_unload()中设置立即卸载。
>
> 总体来说，这段代码实现了创建带自定义文本的水平尺寸标注的功能。
>
