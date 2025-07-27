### 【2931】subdivide sheet body edge Subdivide sheet body edge

#### 代码

```cpp
    /*HEAD SUBDIVIDE_SHEET_BODY_EDGE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        tag_t  
            body;  
        UF_CALL(UF_MODL_ask_edge_body(object, &body));  
        UF_CALL(UF_MODL_ask_body_type(body, &body_type));  
        if (body_type == UF_MODL_SHEET_BODY)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* qq3123197280 */  
    static int mask_for_sheet_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_sheet_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an edge", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_sheet_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void subdivide_edge(tag_t edge, double *loc, tag_t curve[2])  
    {  
        tag_t  
            point;  
        double  
            curve_pnt[3],  
            end[3],  
            junk[3],  
            parm,  
            start[3];  
        UF_CALL(UF_MODL_create_curve_from_edge(edge, &curve[0]));  
        UF_CALL(UF_MODL_create_curve_from_edge(edge, &curve[1]));  
        UF_CALL(UF_CURVE_create_point(loc, &point));  
        UF_CALL(UF_MODL_ask_curve_parm(curve[0], loc, &parm, curve_pnt));  
        UF_CALL(UF_MODL_ask_curve_props(curve[0], parm/2, start, junk, junk,  
            junk, junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve[0], (parm + 1)/2, end, junk, junk,  
            junk, junk, junk));  
        UF_CALL(UF_CURVE_edit_trim_curve(curve[0], point, end, loc,  
            UF_CURVE_NATURAL_SHAPE));  
        UF_CALL(UF_CURVE_edit_trim_curve(curve[1], point, start, loc,  
                UF_CURVE_NATURAL_SHAPE));  
        UF_CALL(UF_OBJ_delete_object(point));  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void display_temporary_asterisk(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_ASTERISK));  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void ask_body_midpoint(tag_t body, double mid_point[3])  
    {  
        int  
            irc;  
        tag_t  
            point;  
        double  
            dist,  
            junk[3],  
            minmaxs[6];  
        FTN(uf6540)(&body, &irc, minmaxs);  
        mid_point[0] = (minmaxs[0] + minmaxs[3]) / 2;  
        mid_point[1] = (minmaxs[1] + minmaxs[4]) / 2;  
        mid_point[2] = (minmaxs[2] + minmaxs[5]) / 2;  
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_CURVE_create_point(mid_point, &point));  
        UF_CALL(UF_MODL_ask_minimum_dist( point, body, FALSE, junk, FALSE,  
            junk, &dist, junk, mid_point));  
        UF_CALL(UF_OBJ_delete_object(point));  
        UF_CALL(UF_DISP_set_display(TRUE));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_edges,  
            n_gaps;  
        tag_t  
            body,  
            curves[2],  
            edge,  
            *trim_curves,  
            trim;  
        double  
            distol,  
            *gap_pts,  
            keep[3],  
            pos[3];  
        uf_list_p_t  
            edge_list,  
            temp;  
        UF_UNDO_mark_id_t  
            mark;  
        UF_MODL_vector_t  
            vector = { 0, UF_MODL_VECTOR_FACE_NORMALS, 0.0 };  
        UF_MODL_ask_distance_tolerance(&distol);  
        while (((edge = select_a_sheet_edge("Select edge to divide")) != NULL_TAG)  
            && specify_point("Select split point", pos))  
        {  
            display_temporary_asterisk(pos);  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Subdivide Edge", &mark));  
            subdivide_edge(edge, pos, curves);  
            UF_CALL(UF_MODL_ask_edge_body(edge, &body));  
            UF_CALL(UF_MODL_ask_body_edges(body, &edge_list));  
            UF_CALL(UF_MODL_ask_list_count(edge_list, &n_edges));  
            UF_CALL(allocate_memory((n_edges + 1) * sizeof(tag_t),  
                (void **)&trim_curves));  
            for (ii = 0, temp = edge_list; temp != NULL; temp = temp->next, ii++)  
            {  
                trim_curves[ii] = temp->eid;  
                if (trim_curves[ii] == edge)  
                {  
                    trim_curves[ii] = curves[0];  
                    ii++;  
                    trim_curves[ii] = curves[1];  
                }  
            }  
            ask_body_midpoint(body, keep);  
            if (UF_CALL(UF_MODL_create_trimmed_sheet(body, n_edges+1, trim_curves,  
                 &vector, 0, 1, keep, distol, &n_gaps, &gap_pts, &trim)))  
            {  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
            }  
            UF_free(trim_curves);  
            UF_CALL(UF_MODL_delete_list(&edge_list));  
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

> 这是用于在NX中细分钣金边界的二次开发代码。主要功能包括：
>
> 1. 提供一个对话框，允许用户选择一个钣金体的边缘。
> 2. 提供一个对话框，允许用户指定一个点作为分割点。
> 3. 在选定的钣金边缘上，根据指定的分割点进行细分。
> 4. 重复上述操作，直到用户选择“取消”。
> 5. 使用NX提供的undo功能，允许用户撤销操作。
>
> 代码的关键点包括：
>
> 总体来说，这段代码实现了在NX中细分钣金边界的功能，并提供了良好的交互性和撤销机制。
>
