### 【2888】show which side of selected edge the material of the selected face is on 展示所选边的所选面对应材料位于哪一侧

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_brep.h>  
    #include <uf_eval.h>  
    #include <uf_undo.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的函数。 */  
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
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical is_item_on_list(uf_list_p_t object_list, tag_t item)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) return TRUE;  
        return FALSE;  
    }  
    /* qq3123197280 */  
    static int filter_list_objects(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        uf_list_p_t  
            the_list = (uf_list_p_t)data;  
        if (is_item_on_list(the_list, object))  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* qq3123197280 */  
    static int mask_for_face_in_list(UF_UI_selection_p_t select, void *data)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_list_objects, NULL, data)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_face_in_list(char *prompt, uf_list_p_t list)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select list face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_face_in_list, list, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static tag_t select_face_of_edge(tag_t edge)  
    {  
        tag_t  
            face;  
        uf_list_p_t  
            facelist;  
        UF_CALL(UF_MODL_ask_edge_faces(edge, &facelist));  
        face = select_face_in_list("Select face of edge", facelist);  
        UF_CALL(UF_MODL_delete_list(&facelist));  
        return face;  
    }  
    static logical ask_edge_vs_face_sense(tag_t edge,  
        UF_BREP_topology_t *topo, UF_BREP_orientation_t *geom_sense)  
    {  
        logical  
            found = false;  
        int  
            ii,  
            nstates;  
        UF_BREP_orientation_t  
            edge_sense,  
            sp_sense;  
        UF_BREP_geometry_t  
            geometry;  
        UF_BREP_state_t  
            *states;  
    /*  get the sense from the FIN parent of the EDGE 里海译:根据FIN父节点的信息，获取EDGE的含义。 */  
        for (ii = 0; ii < topo->num_children; ii++)  
            if (topo->children[ii].child->tag == edge)  
            {  
                UF_CALL(UF_BREP_ask_geometry(topo, NULL, NULL, &geometry,  
                    &sp_sense, &nstates, &states));  
                if (nstates > 0) UF_free(states);  
                UF_CALL(UF_BREP_free_geometry_data(&geometry, NULL));  
                UF_CALL(UF_BREP_ask_geometry(topo->children[ii].child, NULL,  
                    NULL, &geometry, &edge_sense, &nstates, &states));  
                if (nstates > 0) UF_free(states);  
                UF_CALL(UF_BREP_free_geometry_data(&geometry, NULL));  
                if (sp_sense == edge_sense)  
                    *geom_sense = UF_BREP_ORIENTATION_FORWARD;  
                else  
                    *geom_sense = UF_BREP_ORIENTATION_REVERSE;  
                return TRUE;  
            }  
        for (ii = 0; ii < topo->num_children; ii++)  
        {  
            found = ask_edge_vs_face_sense(edge, topo->children[ii].child,  
                geom_sense);  
            if (found) break;  
        }  
        return found;  
    }  
    static logical is_edge_sense_forward_on_face(tag_t edge, tag_t face)  
    {  
        int  
            nstates;  
        UF_BREP_topology_p_t  
            topo;  
        UF_BREP_state_p_t  
            states;  
        UF_BREP_orientation_t  
            edge_sense;  
        UF_CALL(UF_BREP_ask_topology(face, NULL, &topo, &nstates, &states));  
        if (nstates > 0) UF_free(states);  
        ask_edge_vs_face_sense(edge, topo, &edge_sense);  
        UF_CALL(UF_BREP_release_topology(topo, NULL));  
        if (edge_sense == UF_BREP_ORIENTATION_FORWARD) return TRUE;  
        return FALSE;  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void display_colored_conehead(double origin[3], double vec[3], int color)  
    {  
        UF_DISP_conehead_attrb_s  
            attrb;  
        UF_DISP_get_conehead_attrb(&attrb);  
        attrb.color = color;  
        UF_DISP_set_conehead_attrb(&attrb);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_DISP_reset_conehead_attrb();  
    }  
    static void ask_curve_midpoint(tag_t curve, double midpoint[3])  
    {  
        tag_t  
            last,  
            offset,  
            start;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.0, &start));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
            curve, start, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_invisible));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.5, &offset));  
        UF_CALL(UF_SO_create_point_along_curve(curve, UF_SO_update_after_modeling,  
            curve, last, offset, UF_SO_point_along_curve_percent, FALSE, &last));  
        UF_CALL(UF_CURVE_ask_point_data(last, midpoint));  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
    }  
    static void ask_curve_direction(tag_t curve, double loc[3], double dir[3])  
    {  
        double  
            binormal[3],  
            normal[3],  
            parm,  
            point[3];  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(curve, &eval));  
        UF_CALL(UF_EVAL_evaluate_closest_point(eval, loc, &parm, point));  
        UF_CALL(UF_EVAL_evaluate_unit_vectors(eval, parm, point, dir, normal,  
            binormal));  
        UF_CALL(UF_EVAL_free(eval));  
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
    static void do_it(void)  
    {  
        tag_t  
            edge,  
            face;  
        double  
            material[3],  
            normal[3],  
            tangent[3],  
            where[3];  
        while ((edge = select_an_edge("Verify sense")) != NULL_TAG)  
        {  
            ask_curve_midpoint(edge, where);  
            ask_curve_direction(edge, where, tangent);  
            UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, where, tangent, 0,  
                "Edge Direction");  
            while ((face = select_face_of_edge(edge)) != NULL_TAG)  
            {  
                ask_face_normal_at_point(face, where, normal);  
                UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, where, normal,  
                    0, "Face Direction");  
                UF_VEC3_cross(normal, tangent, material);  
                if (!is_edge_sense_forward_on_face(edge, face))  
                    UF_VEC3_negate(material, material);  
                UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, where, material,  
                    0, "Material Side");  
            }  
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

> 这段代码主要用于NX的二次开发，主要实现了以下功能：
>
> 1. 选择边和面：使用UI选择器，允许用户选择边和面，并显示选择的边和面的方向。
> 2. 判断边的方向：通过比较边的方向和面的法线方向，来判断边的方向是否与面的法线方向一致。
> 3. 显示材料方向：通过叉乘计算边切线和面法线的叉积，来显示材料方向，同时根据边的方向调整材料方向的显示。
> 4. 错误处理和日志打印：提供了错误处理函数report_error，用于打印错误信息，并且使用UF_print_syslog函数在系统日志中打印信息。
> 5. 清理和卸载：在代码结束前调用UF_terminate来清理资源，并实现了卸载函数ufusr_ask_unload。
> 6. 函数注释：代码中包含对函数的详细注释，有助于理解代码逻辑。
>
> 总体而言，这段代码实现了选择边面、判断边方向、显示材料方向等几何分析功能，并提供了完善的错误处理和日志打印功能。代码注释清晰，逻辑完整，是NX二次开发的良好示例。
>
