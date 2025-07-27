### 【1143】edit dimension associativity 修改尺寸关联性

#### 代码

```cpp
    /*HEAD EDIT_DIMENSION_ASSOCIATIVITY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_obj.h>  
    #include <uf_undo.h>  
    #include <uf_eval.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的功能。

文档提到：

"翻译 NOTE: UF_print_syslog is new in V18"

翻译为：

"翻译注意：UF_print_syslog是V18版本新增的功能。" */  
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
    /* qq3123197280 */  
    static int mask_for_dim_or_draft(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_drafting_entity_type, UF_all_subtype, 0 },  
                        { UF_dimension_type, UF_all_subtype, 0 },  
                        { UF_tabular_note_type, UF_all_subtype, 0 },  
                        { UF_tol_feature_instance_type, 0, 0} };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dim_or_draft(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select dimension or drafting aid",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dim_or_draft, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    /* qq3123197280 */  
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
    static tag_t select_drf_curve_endpoint(char *prompt, tag_t *view, double pos[3])  
    {  
        int  
            subtype,  
            resp,  
            type;  
        tag_t  
            object;  
        UF_CALL(UF_UI_set_cursor_view(FALSE));  
        UF_CALL(UF_UI_select_with_single_dialog("Select curve end", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_drf_curves, NULL, &resp,  
            &object, pos, view));  
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
    static void do_it(void)  
    {  
        tag_t  
            dim;  
        double  
            new_leader_position[3],  
            old_leader_position[3];  
        UF_DRF_object_t  
            object;  
        UF_UNDO_mark_id_t  
            mark_id;  
        while (((dim = select_a_dim_or_draft("Reassociate", old_leader_position))  
                != NULL_TAG) &&  
               ((object.object_tag = select_drf_curve_endpoint("New endpoint",  
                &object.object_view_tag, new_leader_position)) != NULL_TAG))  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Reassociate Dimension",  
                &mark_id));  
            UF_CALL(UF_DRF_edit_dim_assoc(dim, old_leader_position,  
                new_leader_position, 1, &object));  
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

> 根据代码，这是用于NX的二次开发代码，主要功能是重新关联尺寸的引线到新位置。以下是代码的主要内容和功能：
>
> 1. 错误处理：定义了report_error函数用于输出错误信息。
> 2. 选择尺寸或草图：使用select_a_dim_or_draft函数，让用户选择一个尺寸或草图对象，并获取其引线位置。
> 3. 选择曲线端点：使用select_drf_curve_endpoint函数，让用户选择一个曲线的端点，并获取其位置。
> 4. 重新关联尺寸引线：在主循环中，连续让用户选择尺寸和曲线端点，然后调用UF_DRF_edit_dim_assoc函数将尺寸的引线重新关联到新选择的曲线端点位置。
> 5. 撤销功能：使用UF_UNDO_set_mark函数记录撤销点，以便用户可以撤销重新关联操作。
> 6. 初始化和终止：ufusr函数用于初始化NX环境，调用do_it函数执行主功能，然后终止NX环境。
> 7. 卸载功能：ufusr_ask_unload函数用于在二次开发应用程序卸载时进行清理工作。
>
> 综上所述，这段代码的主要功能是让用户选择一个尺寸对象和一个曲线端点，然后重新关联尺寸的引线到新的曲线端点位置。同时，代码提供了撤销功能和错误处理机制。
>
