### 【0756】create lines normal to selected face at selected endpoint 创建与选定面在选定端点处垂直的线条

#### 代码

```cpp
    /*HEAD CREATE_LINES_NORMAL_TO_SELECTED_FACE_AT_SELECTED_ENDPOINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_csys.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_eval.h>  
    #include <uf_modl.h>  
    #include <uf_mtx.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_trns.h>  
    #include <uf_ui.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog在V18版本中是新增的，它用于打印系统日志。 */  
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
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            limits[2];  
        UF_EVAL_p_t  
            evaluator;  
        UF_CALL(UF_EVAL_initialize(curve, &evaluator));  
        UF_CALL(UF_EVAL_ask_limits(evaluator, limits));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[0], start, NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[1], end, NULL));  
        UF_CALL(UF_EVAL_free(evaluator));  
    }  
    static void ask_curve_close_endpoint(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            ends[6];  
        ask_curve_ends(curve, &ends[0], &ends[3]);  
        UF_VEC3_distance(pos, ends, &dist[0]);  
        UF_VEC3_distance(pos, &ends[3], &dist[1]);  
        if (dist[0] < dist[1]) UF_VEC3_copy(ends, pos);  
        else UF_VEC3_copy(&ends[3], pos);  
    }  
    static void ask_edge_close_endpoint(tag_t edge, double pos[3])  
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
             UF_VEC3_copy(start, pos);  
        else UF_VEC3_copy(end, pos);  
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
    static tag_t select_a_curve_endpoint(char *prompt, double pos[3])  
    {  
        int  
            subtype,  
            resp,  
            type;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve endpoint", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if (type == UF_solid_type) ask_edge_close_endpoint(object, pos);  
            else ask_curve_close_endpoint(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face = NULL_TAG,  
            curve = NULL_TAG,  
            new_normal_line = NULL_TAG,  
            orig_csys = NULL_TAG,  
            new_csys_id = NULL_TAG,  
            matrix_id = NULL_TAG;  
        double  
            ep[3] = { 0., 0., 0. },  
            end[3] = { 1., 0., 0. },  
            abs_end[3] = { 1., 0., 0. },  
            parm[2] = { 0., 0. },  
            pnt[3] = { 0., 0., 0. },  
            dirU1[3] = { 0., 0., 0. },  
            dirU2[3] = { 0., 0., 0. },  
            dirV1[3] = { 0., 0., 0. },  
            dirV2[3] = { 0., 0., 0. },  
            norm_dir[3] = { 0., 0., 0. },  
            radii[2] = { 0., 0. },  
            mtx[ 9 ] = { 0., 0., 0.,  0., 0., 0.,  0., 0., 0. } ;  
        UF_CURVE_line_t  
            line_pts;  
        UF_CALL( UF_CSYS_ask_wcs( &orig_csys ));  
        while(  
             ((face = select_a_face("Face:")) != NULL_TAG) &&  
             ((curve = select_a_curve_endpoint("Curve Endpoint:", ep)) != NULL_TAG)  
              )  
        {      
            UF_CALL(UF_MODL_ask_face_parm(face, ep, parm, pnt));  
            UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, dirU1, dirV1, dirU2,  
                    dirV2, norm_dir, radii));  
            UF_VEC3_copy ( pnt, line_pts.start_point );  
            UF_CALL( UF_MTX3_initialize_x( norm_dir, mtx ));  
            UF_CALL( UF_CSYS_create_matrix( mtx, &matrix_id ));  
            UF_CALL( UF_CSYS_create_csys( pnt, matrix_id, &new_csys_id ));   
            UF_CALL( UF_CSYS_set_wcs( new_csys_id ));   
            UF_CALL( UF_CSYS_map_point( UF_CSYS_ROOT_WCS_COORDS, end,  
                                        UF_CSYS_ROOT_COORDS, abs_end ));  
            UF_VEC3_copy ( abs_end, line_pts.end_point );  
            UF_CALL(UF_CURVE_create_line( &line_pts, &new_normal_line ));  
            UF_CALL( UF_CSYS_set_wcs( orig_csys ));   
            UF_OBJ_delete_object( new_csys_id );  
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

> 这段代码是用于NX二次开发的，主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 视图坐标转换：实现了视图坐标与绝对坐标之间的转换函数map_abs2view和map_view2abs。
> 3. 查询对象上的点：定义了ask_pos_on_obj函数，用于查询指定对象上的点。
> 4. 查询曲线端点：定义了ask_curve_ends和ask_curve_close_endpoint函数，用于查询曲线的端点。
> 5. 查询边的端点：定义了ask_edge_close_endpoint函数，用于查询边的端点。
> 6. 曲线选择：定义了mask_for_curves函数，用于设置曲线选择模式。
> 7. 选择曲线端点：定义了select_a_curve_endpoint函数，用于选择一个曲线端点。
> 8. 面选择：定义了mask_for_faces函数，用于设置面选择模式。
> 9. 选择面：定义了select_a_face函数，用于选择一个面。
> 10. 执行主功能：定义了do_it函数，用于实现创建与选定的面法线平行的线段，直到用户选择退出。
> 11. 入口函数：定义了ufusr函数，作为二次开发的入口函数。
> 12. 卸载函数：定义了ufusr_ask_unload函数，用于处理卸载操作。
>
> 总的来说，这段代码实现了在NX中通过选择面和曲线端点来创建与面法线平行的线段的功能。
>
