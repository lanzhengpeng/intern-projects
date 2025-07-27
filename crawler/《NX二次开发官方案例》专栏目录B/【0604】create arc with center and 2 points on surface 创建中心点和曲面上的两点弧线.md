### 【0604】create arc with center and 2 points on surface 创建中心点和曲面上的两点弧线

#### 代码

```cpp
    /*HEAD CREATE_ARC_WITH_CENTER_AND_2_POINTS_ON_SURFACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <uf_disp.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18中新增的功能。 */  
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
    static void map_abs2view(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    /* 里海 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_point_on_face(char *prompt, double cp[3])  
    {  
        int  
            n,  
            resp;  
        tag_t  
            face,  
            line,  
            view;  
        double  
            tol;  
        UF_CURVE_line_t  
            lp;  
        UF_MODL_intersect_info_p_t  
            *ints;  
        UF_OBJ_disp_props_t  
            attrib = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &face, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(face, 0));  
            map_abs2view(cp, view);  
            lp.start_point[0] = cp[0];  
            lp.start_point[1] = cp[1];  
            lp.start_point[2] = cp[2] + 1000;  
            lp.end_point[0] = cp[0];  
            lp.end_point[1] = cp[1];  
            lp.end_point[2] = cp[2] - 1000;  
            map_view2abs(lp.start_point, view);  
            map_view2abs(lp.end_point, view);  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            UF_MODL_ask_distance_tolerance(&tol);  
            UF_CALL(UF_MODL_intersect_objects(face, line, tol, &n, &ints));  
            if (n > 0)  
            {  
                UF_VEC3_copy(ints[0]->intersect.point.coords, cp);  
                UF_free(ints);  
            }  
            UF_CALL(UF_OBJ_delete_object(line));  
            UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
                UF_DISP_USE_ACTIVE_PLUS, cp, &attrib, UF_DISP_ASTERISK));  
            return face;  
        }  
        else return NULL_TAG;  
    }  
    static void ask_face_normalized_parms(tag_t face, double *pos, double *fparms)  
    {  
        double  
            minmax[4],  
            parms[2];  
        UF_CALL(UF_MODL_ask_face_parm(face, pos, parms, pos));  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, minmax));  
        fparms[0] = (parms[0] - minmax[0]) / (minmax[1] - minmax[0]);  
        fparms[1] = (parms[1] - minmax[2]) / (minmax[3] - minmax[2]);  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            arc,  
            face[3],  
            point[3],  
            u,  
            v;  
        double  
            cp[3][3],  
            parm[2];  
        char  
            *msg[3] = { "Pick arc center",  
                        "Pick arc start",  
                        "Pick arc end" };  
        for (ii = 0; ii < 3; ii++)  
        {  
            face[ii] = select_point_on_face(msg[ii], &cp[ii][0]);  
            if (face[ii] == NULL_TAG) ii--;  
            if (ii < 0) return;  
        }  
        for (ii = 0; ii < 3; ii++)  
        {  
            ask_face_normalized_parms(face[ii], &cp[ii][0], parm);  
            UF_CALL(UF_SO_create_scalar_double(face[ii],  
                UF_SO_update_within_modeling, parm[0], &u));  
            UF_CALL(UF_SO_create_scalar_double(face[ii],  
                UF_SO_update_within_modeling, parm[1], &v));  
            UF_CALL(UF_SO_create_point_on_surface(face[ii],  
                UF_SO_update_within_modeling, face[ii], u, v, &point[ii]));  
            UF_CALL(UF_SO_set_visibility_option(point[ii], UF_SO_visible));  
        }  
        UF_CALL(UF_SO_create_arc_center_2_pnts(face[0],  
            UF_SO_update_within_modeling, point, &arc));  
        UF_CALL(UF_SO_set_visibility_option(arc, UF_SO_visible));  
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

> 这段代码是一个NX Open的UF客户端程序，用于在NX中创建一个圆弧，该圆弧以用户选择的面上的三个点为中心和端点。
>
> 主要步骤包括：
>
> 1. 错误报告函数：通过自定义错误报告函数来打印错误信息。
> 2. 坐标转换函数：定义了两个坐标转换函数，用于在绝对坐标系和视图坐标系之间进行转换。
> 3. 选择面函数：使用UI接口选择面，并获取该面上的一个点作为圆弧中心点。
> 4. 获取面参数函数：获取选择的面的参数，并将其归一化。
> 5. 创建圆弧函数：使用SO接口创建圆弧，以三个面上的点为中心和端点。
> 6. UF初始化：在UF初始化和终止函数中调用do_it函数，用于执行创建圆弧的逻辑。
> 7. 卸载询问函数：定义了卸载询问函数，以立即卸载客户端程序。
>
> 这段代码利用了NX提供的API，实现了用户交互选择点并创建圆弧的功能。
>
