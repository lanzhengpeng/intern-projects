### 【0594】create arc and concentric hole 创建弧和同心孔

#### 代码

```cpp
    /*HEAD CREATE_ARC_AND_CONCENTRIC_HOLE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_mtx.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog是V18版本新增的。 */  
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
    static tag_t the_arc;  /* Yucky global variable 译:Yucky global variable的翻译是：令人讨厌的全局变量。 */  
    static tag_t ask_feat_first_edge(tag_t feat)  
    {  
        int  
            n;  
        tag_t  
            edge;  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_ask_feat_edges(feat, &edge_list));  
        UF_CALL(UF_MODL_ask_list_count(edge_list, &n));  
        if (n > 0)  
        {  
            UF_CALL(UF_MODL_ask_list_item(edge_list, 0, &edge));  
            UF_CALL(UF_MODL_delete_list(&edge_list));  
            return edge;  
        }  
        else  
            return NULL_TAG;  
    }  
    static int rpo_hole(tag_t feat)  
    {  
        tag_t  
            target[1],  
            tool[1];  
        int  
            target_qual[1] = { UF_MODL_ARC_CENTER },  
            tool_qual[1] = { UF_MODL_ARC_CENTER };  
        char  
            *constraints_array[1] = { "PARA_DIST_PARMS"},  
            *constraints_value[1] = { "0.0" };  
        target[0] = the_arc;  
        tool[0] = ask_feat_first_edge(feat);  
    /*  In V18 - this returns an error 670178 "Unable to reference edge."  
        See PR 4359393  
    */  
        return (UF_MODL_create_rpo_constraints(feat, NULL_TAG, NULL_TAG,  
            target, target_qual, tool, tool_qual, constraints_value,  
            constraints_array, 1));  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp,  
            &face, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(face, 0));  
            map_abs2view(cp);  
            lp.start_point[0] = cp[0];  
            lp.start_point[1] = cp[1];  
            lp.start_point[2] = cp[2] + 1000;  
            lp.end_point[0] = cp[0];  
            lp.end_point[1] = cp[1];  
            lp.end_point[2] = cp[2] - 1000;  
            map_view2abs(lp.start_point);  
            map_view2abs(lp.end_point);  
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
    static void ask_face_dirs(double loc[3], tag_t face, double xyz[9])  
    {  
        double  
            parm[2],  
            pnt[3],  
            dirU2[3],  
            dirV2[3],  
            radii[2];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, &xyz[0], &xyz[3], dirU2,  
            dirV2, &xyz[6], radii));  
        UF_VEC3_negate(&xyz[6], &xyz[6]);  
    }  
    static void map_arc_center_from_abs(UF_CURVE_arc_t *adata)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12];  
        UF_CALL(UF_CSYS_ask_matrix_values(adata->matrix_tag, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        FTN(uf5941)(adata->arc_center, mx);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            hole;  
        double  
            dir[3],  
            loc[3],  
            dest_csys[9];  
        UF_CURVE_arc_t  
            arc_data = { NULL_TAG, 0.0, TWOPI, { 0,0,0 }, 0.5 };  
        UF_CALL(UF_MODL_register_rpo_routine(rpo_hole));  
        while ((face = select_point_on_face("Place hole", loc)) != NULL_TAG)  
        {  
            UF_VEC3_copy(loc, arc_data.arc_center);  
            ask_face_dirs(loc, face, dest_csys);  
            UF_MTX3_ortho_normalize(dest_csys);  
            UF_CALL(UF_CSYS_create_matrix(dest_csys, &arc_data.matrix_tag));  
            map_arc_center_from_abs(&arc_data);  
            UF_CALL(UF_CURVE_create_arc(&arc_data, &the_arc));  
            UF_VEC3_negate(&dest_csys[6], dir);  
            UF_CALL(UF_MODL_create_simple_hole(loc, dir, "0.25", "1.5", "118",  
                face, NULL_TAG, &hole));  
        }  
        UF_CALL(UF_MODL_unregister_rpo_routine());  
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
    /*  Note:  can NOT be unloaded - calls UF_MODL_register_rpo_routine.  
        return (UF_UNLOAD_IMMEDIATELY);  
    */  
        return (UF_UNLOAD_UG_TERMINATE);  
    }

```

#### 代码解析

> 这段代码是NX Open C++的一个二次开发示例，用于在NX中创建一个圆弧和同心孔特征。
>
> 代码的主要功能包括：
>
> 1. 定义了一个全局变量the_arc，用于存储创建的圆弧对象。
> 2. ask_feat_first_edge函数用于获取特征对象上的第一个边。
> 3. rpo_hole函数用于创建一个约束，使得创建的孔特征与圆弧同心。
> 4. map_abs2view和map_view2abs函数用于将绝对坐标系坐标转换到视图坐标系和相反。
> 5. mask_for_faces函数用于设置选择面时只选择实体类型的面。
> 6. select_point_on_face函数用于选择一个面上的点，并返回该面的tag。
> 7. ask_face_dirs函数用于获取面上某点的方向。
> 8. map_arc_center_from_abs函数用于将圆弧中心从绝对坐标系转换到视图坐标系。
> 9. do_it函数是主函数，用于创建圆弧和同心孔。它通过循环选择面，获取面的方向，创建圆弧，然后创建同心孔。
> 10. ufusr函数是NX调用的入口函数，用于初始化、调用do_it函数，然后终止。
> 11. ufusr_ask_unload函数用于处理卸载时的情况。
>
> 总体来说，这段代码实现了在NX中通过选择面，创建与其同心的圆弧和孔的功能。代码结构清晰，各函数功能明确，是学习NX二次开发的一个很好的示例。
>
