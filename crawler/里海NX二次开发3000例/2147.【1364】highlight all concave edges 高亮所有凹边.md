### 【1364】highlight all concave edges 高亮所有凹边

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ALL_CONCAVE_EDGES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_eval.h>  
    #include <uf_vec.h>  
    #include <uf_brep.h>  
    #include <uf_undo.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中新增的函数。 */  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_of_type_and_subtype(tag_t part, int type, int subtype,  
        tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype)  
                (UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_all_edges(tag_t part, tag_t **edges)  
    {  
        return ask_all_of_type_and_subtype(part, UF_solid_type,  
            UF_solid_edge_subtype, edges);  
    }  
    static int ask_edge_faces(tag_t edge, tag_t **faces)  
    {  
        uf_list_p_t  
            facelist;  
        UF_CALL(UF_MODL_ask_edge_faces(edge, &facelist));  
        return (make_an_array(&facelist, faces));  
    }  
    static void ask_edge_midpoint(tag_t edge, double point[3])  
    {  
        double  
            limits[2];  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(edge, &eval));  
        UF_CALL(UF_EVAL_ask_limits(eval, limits));  
        UF_CALL(UF_EVAL_evaluate(eval, 0, ((limits[0]+limits[1])/2), point, NULL));  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
            UF_DISP_USE_ACTIVE_PLUS, point, &attrib, UF_DISP_POINT));  
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
    /*  get the sense from the FIN parent of the EDGE 里海译:根据FIN父节点获取EDGE的感觉 */  
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
    static logical is_edge_convex(tag_t edge)  
    {  
        int  
            n_faces;  
        tag_t  
            *faces;  
        double  
            angle,  
            edge_dir[3],  
            normal1[3],  
            normal2[3],  
            point[3];  
        ask_edge_midpoint(edge, point);  
        n_faces = ask_edge_faces(edge, &faces);  
        if (!is_edge_sense_forward_on_face(edge, faces[0]))  
        {  
            ask_face_normal_at_point(faces[0], point, normal1);  
            ask_face_normal_at_point(faces[1], point, normal2);  
        }  
        else  
        {  
            ask_face_normal_at_point(faces[1], point, normal1);  
            ask_face_normal_at_point(faces[0], point, normal2);  
        }  
        ask_curve_direction(edge, point, edge_dir);  
        if (n_faces > 0) UF_free(faces);  
        UF_VEC3_angle_between(normal1, normal2, edge_dir, &angle);  
        if (angle > PI) return TRUE;  
        return FALSE;  
    }  
    static void set_highlight_object_list(uf_list_p_t list, int flag)  
    {  
        uf_list_p_t  
            tmp;  
        for (tmp = list; tmp != NULL; tmp = tmp->next)  
            UF_CALL(UF_DISP_set_highlight(tmp->eid, flag));  
    }  
    static void do_it(void)  
    {  
        logical  
            is_smooth = FALSE;  
        int  
            ii,  
            n_concave,  
            n_edges;  
        tag_t  
            *edges,  
            part = UF_PART_ask_display_part();  
        char  
            msg[MAX_LINE_SIZE+1];  
        uf_list_p_t  
            concave_list;  
        UF_CALL(UF_MODL_create_list(&concave_list));  
        n_edges = ask_all_edges(part, &edges);  
        for (ii = 0; ii < n_edges; ii++)  
        {  
            UF_CALL(UF_MODL_ask_edge_smoothness(edges[ii], 0, &is_smooth));  
            if (!is_smooth && !is_edge_convex(edges[ii]))  
            {  
                UF_CALL(UF_MODL_put_list_item(concave_list, edges[ii]));  
            }  
        }  
        UF_CALL(UF_MODL_ask_list_count(concave_list, &n_concave));  
        if (n_concave > 0)  
        {  
            set_highlight_object_list(concave_list, TRUE);  
            sprintf(msg, "%d concave edges found", n_concave);  
            uc1601(msg, TRUE);  
            set_highlight_object_list(concave_list, FALSE);  
            UF_CALL(UF_MODL_delete_list(&concave_list));  
        }  
        else  
            uc1601("No concave edges found", TRUE);  
        if (n_edges > 0) UF_free(edges);  
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

> 这段代码的主要功能是高亮显示NX模型中的凹边。其关键思路是遍历模型中的所有边，判断每个边是否为凹边，并使用不同颜色高亮显示。具体步骤如下：
>
> 1. 获取当前活动部件的所有边，并存储在数组中。
> 2. 遍历每个边，判断是否为平滑边，如果不是平滑边，则进一步判断是否为凹边。
> 3. 判断边是否为凹边的逻辑是：获取边的两个相邻面，计算这两个面的法线向量与边的方向向量的夹角，如果夹角大于180度，则认为是凹边。
> 4. 将所有凹边放入一个列表中。
> 5. 高亮显示列表中的凹边，并统计数量，显示在信息窗口中。
> 6. 最后，释放所有申请的内存。
>
> 该代码实现了对NX模型凹边的检测与高亮显示功能，通过遍历边并判断是否为凹边，实现了NX二次开发中对凹边的自动识别与可视化。
>
