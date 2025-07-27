### 【2896】snip surface 剪裁曲面

#### 代码

```cpp
    /*HEAD SNIP_SURFACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的功能。请仅回答翻译，不要添加任何额外内容。 */  
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
    /* qq3123197280 */  
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
            return face;  
        }  
        else return NULL_TAG;  
    }  
    static void ask_pos_on_obj(tag_t obj, double loc[3], tag_t view)  
    {  
        tag_t  
            line;  
        double  
            cp[3],  
            dist;  
        UF_CURVE_line_t  
            lp;  
        map_abs2view(loc, view);  
        lp.start_point[0] = loc[0];  
        lp.start_point[1] = loc[1];  
        lp.start_point[2] = loc[2] + 1000;  
        lp.end_point[0] = loc[0];  
        lp.end_point[1] = loc[1];  
        lp.end_point[2] = loc[2] - 1000;  
        map_view2abs(lp.start_point, view);  
        map_view2abs(lp.end_point, view);  
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_CURVE_create_line(&lp, &line));  
        UF_CALL(UF_MODL_ask_minimum_dist(obj, line, 0, cp, 0, cp, &dist, loc, cp));  
        UF_CALL(UF_OBJ_delete_object(line));  
        UF_CALL(UF_DISP_set_display(TRUE));  
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
        if (dist[0] < dist[1]) return UF_MODL_CURVE_START_FROM_BEGIN;  
        else return UF_MODL_CURVE_START_FROM_END;  
    }  
    /* qq3123197280 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_curve_end(char *prompt, int *which_end)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            pos[3];  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve end", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos, view);  
            *which_end = ask_curve_close_end(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_string(char *which, UF_STRING_t *string)  
    {  
        int  
            dirs[50],  
            jj = 0,  
            kk = -1;  
        char  
            prompt[81];  
        tag_t  
            curves[50];  
        do  
        {  
            if (kk > 0)  
            {  
                sprintf(prompt, "OK to finish selecting %s string", which);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else  
                UF_CALL(UF_UI_set_status(""));  
            kk++;  
            sprintf(prompt, "Select %s String - curve #%d", which, kk+1);  
            curves[kk] = select_a_curve_end(prompt, &dirs[kk]);  
            if (curves[kk] != NULL_TAG)  
                UF_CALL(UF_DISP_set_highlight(curves[kk], TRUE));  
        } while ((curves[kk] != NULL_TAG) && (kk < 50));  
        UF_CALL(UF_UI_set_status(""));  
        if (kk > 0)  
        {  
            UF_MODL_init_string_list(string);  
            UF_MODL_create_string_list(1, kk, string);  
            string->string[0] = kk;  
            string->dir[0] = dirs[0];  
            for (jj = 0; jj < kk; jj++)  
            {  
                string->id[jj] = curves[jj];  
                UF_CALL(UF_DISP_set_highlight(curves[jj], FALSE));  
            }  
            return TRUE;  
        }  
        return FALSE;  
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
    static void ask_face_normal_at_point(tag_t face, double loc[3],  
        double normal[3])  
    {  
        double  
            junk[3],  
            parm[2],  
            pt[3];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, junk, junk, junk, junk, junk,  
            normal, junk));  
    }  
    static tag_t ask_body_first_feat(tag_t body)  
    {  
        tag_t  
            feat;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
        UF_CALL(UF_MODL_ask_list_item(feat_list, 0, &feat));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
        return feat;  
    }  
    static void delete_a_feature(tag_t feature)  
    {  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        UF_CALL(UF_MODL_put_list_item(feat_list, feature));  
        UF_CALL(UF_MODL_delete_feature(feat_list));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
    }  
    static void delete_body_parameters(tag_t body)  
    {  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        UF_CALL(UF_MODL_put_list_item(body_list, body));  
        UF_CALL(UF_MODL_delete_body_parms(body_list));  
        UF_CALL(UF_MODL_delete_list(&body_list));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body,  
            old_feat,  
            snip_feat;  
        double  
            cp[3];  
        UF_MODL_snipsrf_feature_data_t  
            feat_data;  
        UF_UNDO_mark_id_t  
            mark;  
        if (((feat_data.edit_face =  
            select_point_on_face("Target face for Snipping", cp)) != NULL_TAG) &&  
            specify_string("Bounding object for Snipping",  
                &feat_data.snip_cv_string))  
        {  
            feat_data.boundary_type = UF_MODL_snipsurf_boundary_type_curves;  
            feat_data.snipping_plane = NULL_TAG;  
            ask_face_normal_at_point(feat_data.edit_face, cp,  
                feat_data.project_vector);  
            feat_data.snip_or_divide = 1;  
            ask_face_normalized_parms(feat_data.edit_face, cp,  
                feat_data.region_point_uv);  
            feat_data.project_method = 0; // 0- along surface normal  
            feat_data.refit_method = UF_MODL_snipsurf_refit_method_none;  
            feat_data.refit_degree = 3;  
            feat_data.refit_patches = 33;  
            feat_data.tols[0] = 0.001;  
            UF_MODL_ask_distance_tolerance(&feat_data.tols[1]);  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Snip Surface", &mark));  
            if (!UF_CALL(UF_MODL_create_snip_surface_feature(&feat_data,  
                &snip_feat)))  
            {  
            /*  The snip feature creates a new sheet body.  To match the  
                interactive functionality, use these additional steps.  
            */  
                UF_CALL(UF_MODL_ask_face_body(feat_data.edit_face, &body));  
                delete_body_parameters(body);  
                old_feat = ask_body_first_feat(body);  
                UF_CALL(UF_MODL_ask_feat_body(snip_feat, &body));  
                UF_CALL(UF_MODL_replace_body_data(old_feat, body));  
                delete_a_feature(snip_feat);  
            }  
            UF_MODL_free_string_list(&feat_data.snip_cv_string);  
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

> 这段代码是用于NX的二次开发代码，其主要功能是创建一个Snip Surface特征。代码流程如下：
>
> 1. 包含了NX二次开发所需的头文件。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了两个坐标转换函数map_abs2view和map_view2abs，用于视图坐标系和绝对坐标系之间的转换。
> 4. 定义了选择面和曲线的辅助函数，包括mask_for_faces、select_point_on_face、mask_for_curves和select_a_curve_end等。
> 5. 定义了获取面参数、法向量、曲线端点等信息的辅助函数。
> 6. 定义了创建Snip Surface特征所需的feat_data结构体。
> 7. 在do_it函数中，首先选择目标面和边界曲线，然后设置feat_data的各个参数，接着创建Snip Surface特征。
> 8. 在ufusr函数中，初始化NX，调用do_it函数，最后终止NX。
> 9. 定义了卸载函数ufusr_ask_unload。
>
> 该代码实现了选择面、选择边界曲线、创建Snip Surface特征等功能，通过二次开发扩展了NX的功能。
>
