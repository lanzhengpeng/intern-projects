### 【0598】create arc thru 3 points on surface 在曲面上通过3点创建圆弧

#### 代码

```cpp
    /*HEAD CREATE_ARC_THRU_3_POINTS_ON_SURFACE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog在V18中是新增的，仅提供译文，无需额外评论。

UF_print_syslog在V18中是新增的。 */  
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
            msg[133];  
        for (ii = 0; ii < 3; ii++)  
        {  
            sprintf(msg, "Pick point on face %d", ii+1);  
            face[ii] = select_point_on_face(msg, &cp[ii][0]);  
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
        UF_CALL(UF_SO_create_arc_three_points(face[0],  
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

> 这段代码是一个NX Open C++的二次开发示例，用于创建通过三个点在曲面上的圆弧。主要功能包括：
>
> 1. 定义错误报告函数report_error，用于在发生UF函数调用错误时输出错误信息。
> 2. 定义坐标系转换函数map_abs2view和map_view2abs，用于将绝对坐标系下的点转换到视图坐标系，以及将视图坐标系下的点转换回绝对坐标系。
> 3. 定义面选择掩码函数mask_for_faces，用于设置选择面时需要考虑的面类型。
> 4. 定义点选择函数select_point_on_face，用于让用户选择一个面上的点，并计算该点到面中心的垂直交点。
> 5. 定义参数计算函数ask_face_normalized_parms，用于计算选择点在面参数空间中的参数值。
> 6. 定义主体函数do_it，用于：依次选择三个面并获取对应的点计算每个点在面参数空间中的参数值在每个面上根据参数值创建点使用这三个点创建圆弧
> 7. 依次选择三个面并获取对应的点
> 8. 计算每个点在面参数空间中的参数值
> 9. 在每个面上根据参数值创建点
> 10. 使用这三个点创建圆弧
> 11. 定义ufusr函数，作为程序的入口，用于初始化和执行do_it函数，并在结束时进行清理。
> 12. 定义卸载函数ufusr_ask_unload，用于设置卸载模式。
>
> 通过以上功能，这段代码实现了在三个面上选择点，并根据这些点创建通过它们在曲面上的圆弧的功能。
>
