### 【1574】make brep half cylinder sheet 创建半个圆柱面实体

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_brep.h>  
    #include <uf_obj.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void report_brep_topology(UF_BREP_topology_t *topology)  
    {  
        int unique_id;  
        UF_UI_open_listing_window();  
        WRITE("topology->type = ");  
        switch (topology->type)  
        {  
            case UF_BREP_SOLID_TOPO:  
                WRITE_D( UF_BREP_SOLID_TOPO );  
                break;  
            case UF_BREP_SHEET_TOPO:  
                WRITE_D( UF_BREP_SHEET_TOPO );  
                break;  
            case UF_BREP_TRIMSURF_TOPO:  
                WRITE_D( UF_BREP_TRIMSURF_TOPO );  
                break;  
            case UF_BREP_SHELL_TOPO:  
                WRITE_D( UF_BREP_SHELL_TOPO );  
                break;  
            case UF_BREP_FACE_TOPO:  
                WRITE_D( UF_BREP_FACE_TOPO );  
                break;  
            case UF_BREP_LOOP_TOPO:  
                WRITE_D( UF_BREP_LOOP_TOPO );  
                break;  
            case UF_BREP_FIN_TOPO:  
                WRITE_D( UF_BREP_FIN_TOPO );  
                break;  
            case UF_BREP_EDGE_TOPO:  
                WRITE_D( UF_BREP_EDGE_TOPO );  
                break;  
            case UF_BREP_VERTEX_TOPO:  
                WRITE_D( UF_BREP_VERTEX_TOPO );  
                break;  
            default:  
                WRITE("Unknown topology type - ");  
                WRITE_D(topology->type);  
                break;  
        }  
        WRITE_D(topology->tag);  
        WRITE_D(topology->num_children);  
        UF_BREP_ask_identifier(topology, &unique_id);  
        WRITE_D(unique_id);  
    }  
    static void report_brep_states(int nstates, UF_BREP_state_t *states)  
    {  
        int  
            ii;  
        if (nstates > 0)  
        {  
            UF_UI_open_listing_window();  
            WRITE_D(nstates);  
            for (ii = 0; ii < nstates; ii++)  
            {  
                WRITE_D(ii);  
                WRITE("states[ii].state_code = ");  
                switch (states[ii].state_code)  
                {  
                    case UF_BREP_STATE_unknown_child:  
                        WRITE_D( UF_BREP_STATE_unknown_child);  
                        break;  
                    case UF_BREP_STATE_duplicate_child:  
                        WRITE_D( UF_BREP_STATE_duplicate_child);  
                        break;  
                    case UF_BREP_STATE_wrong_child:  
                        WRITE_D( UF_BREP_STATE_wrong_child);  
                        break;  
                    case UF_BREP_STATE_missing_child:  
                        WRITE_D( UF_BREP_STATE_missing_child);  
                        break;  
                    case UF_BREP_STATE_extra_child:  
                        WRITE_D( UF_BREP_STATE_extra_child);  
                        break;  
                    case UF_BREP_STATE_wrong_parent:  
                        WRITE_D( UF_BREP_STATE_wrong_parent);  
                        break;  
                    case UF_BREP_STATE_missing_parent:  
                        WRITE_D( UF_BREP_STATE_missing_parent);  
                        break;  
                    case UF_BREP_STATE_extra_parent:  
                        WRITE_D( UF_BREP_STATE_extra_parent);  
                        break;  
                    case UF_BREP_STATE_bad_loop:  
                        WRITE_D( UF_BREP_STATE_bad_loop);  
                        break;  
                    case UF_BREP_STATE_disjoint_shell:  
                        WRITE_D( UF_BREP_STATE_disjoint_shell);  
                        break;  
                    case UF_BREP_STATE_non_manifold:  
                        WRITE_D( UF_BREP_STATE_non_manifold);  
                        break;  
                    case UF_BREP_STATE_topology_error:  
                        WRITE_D( UF_BREP_STATE_topology_error);  
                        break;  
                    case UF_BREP_STATE_unknown_body_fault:  
                        WRITE_D( UF_BREP_STATE_unknown_body_fault);  
                        break;  
                    case UF_BREP_STATE_body_inside_out:  
                        WRITE_D( UF_BREP_STATE_body_inside_out);  
                        break;  
                    case UF_BREP_STATE_self_intersecting:  
                        WRITE_D( UF_BREP_STATE_self_intersecting);  
                        break;  
                    case UF_BREP_STATE_degenerate_geometry:  
                        WRITE_D( UF_BREP_STATE_degenerate_geometry);  
                        break;  
                    case UF_BREP_STATE_invalid_geometry:  
                        WRITE_D( UF_BREP_STATE_invalid_geometry);  
                        break;  
                    case UF_BREP_STATE_face_face_inconsistency:  
                        WRITE_D( UF_BREP_STATE_face_face_inconsistency);  
                        break;  
                    case UF_BREP_STATE_face_error:  
                        WRITE_D( UF_BREP_STATE_face_error);  
                        break;  
                    case UF_BREP_STATE_vertex_not_on_curve:  
                        WRITE_D( UF_BREP_STATE_vertex_not_on_curve);  
                        break;  
                    case UF_BREP_STATE_edge_reversed:  
                        WRITE_D( UF_BREP_STATE_edge_reversed);  
                        break;  
                    case UF_BREP_STATE_vertex_not_on_surface:  
                        WRITE_D( UF_BREP_STATE_vertex_not_on_surface);  
                        break;  
                    case UF_BREP_STATE_edge_not_on_surface:  
                        WRITE_D( UF_BREP_STATE_edge_not_on_surface);  
                        break;  
                    case UF_BREP_STATE_loop_inconsistent:  
                        WRITE_D( UF_BREP_STATE_loop_inconsistent);  
                        break;  
                    case UF_BREP_STATE_face_or_edge_non_g1:  
                        WRITE_D( UF_BREP_STATE_face_or_edge_non_g1);  
                        break;  
                    case UF_BREP_STATE_invalid_entity_ids:  
                        WRITE_D( UF_BREP_STATE_invalid_entity_ids);  
                        break;  
                    case UF_BREP_STATE_edge_tol_too_big:  
                        WRITE_D( UF_BREP_STATE_edge_tol_too_big);  
                        break;  
                    case UF_BREP_STATE_inconsistent_geom:  
                        WRITE_D( UF_BREP_STATE_inconsistent_geom);  
                        break;  
                    case UF_BREP_STATE_invalid_geom:  
                        WRITE_D( UF_BREP_STATE_invalid_geom);  
                        break;  
                    case UF_BREP_STATE_unknown_geom_fault:  
                        WRITE_D( UF_BREP_STATE_unknown_geom_fault);  
                        break;  
                    case UF_BREP_STATE_bad_child_count:  
                        WRITE_D( UF_BREP_STATE_bad_child_count);  
                        break;  
                    case UF_BREP_STATE_null_child_array:  
                        WRITE_D( UF_BREP_STATE_null_child_array);  
                        break;  
                    case UF_BREP_STATE_null_child_pointer:  
                        WRITE_D( UF_BREP_STATE_null_child_pointer);  
                        break;  
                    case UF_BREP_STATE_multi_vertex_loop:  
                        WRITE_D( UF_BREP_STATE_multi_vertex_loop);  
                        break;  
                    default:  
                        WRITE("Unknown state code - ");  
                        WRITE_D(states[ii].state_code);  
                        break;  
                }  
                WRITE_D(states[ii].flag);  
                WRITE3F(states[ii].value);  
                if (states[ii].item != NULL) report_brep_topology(states[ii].item);  
                WRITE_D(states[ii].ug_tag);  
                WRITE_D(states[ii].misc);  
            }  
            UF_free(states);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            irc,  
            nstates;  
        tag_t  
            body,  
            wcs;  
        UF_BREP_oriented_child_t  
            *face_kid,  
            *loop_kids;  
        UF_BREP_topology_t  
            *edges[4],  
            *face,  
            *loop;  
        UF_BREP_geometry_t  
            geom;  
        UF_BREP_options_t  
            options = { 0, NULL, NULL };  
        UF_BREP_state_t  
            *states;  
        loop_kids = UF_allocate_memory(4 * sizeof(UF_BREP_oriented_child_t), &irc);  
        if (irc) UF_CALL(irc);  
        for (ii = 0; ii < 4; ii++)  
        {  
            edges[ii] = UF_allocate_memory(sizeof(UF_BREP_topology_t), &irc);  
            if (irc) UF_CALL(irc);  
            UF_BREP_INITIALIZE_TOPOLOGY(edges[ii], NULL, 0, UF_BREP_EDGE_TOPO);  
        }  
        geom.type = UF_BREP_ARC_GEOM;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_matrix_of_object(wcs, &geom.geom.arc.matrix_tag));  
        geom.geom.arc.start_angle = PI;  
        geom.geom.arc.end_angle = TWOPI;  
        geom.geom.arc.arc_center[0] = 0;  
        geom.geom.arc.arc_center[1] = 0;  
        geom.geom.arc.arc_center[2] = 0;  
        geom.geom.arc.radius = 2;  
        UF_CALL(UF_BREP_attach_geometry( edges[0], &geom,  
                UF_BREP_ORIENTATION_FORWARD, &options, &nstates, &states));  
        report_brep_states(nstates, states);  
        loop_kids[0].child = edges[0];  
        loop_kids[0].orientation = UF_BREP_ORIENTATION_FORWARD;  
        geom.geom.arc.arc_center[2] = 2;  
        UF_CALL(UF_BREP_attach_geometry( edges[2], &geom,  
                UF_BREP_ORIENTATION_REVERSE, &options, &nstates, &states));  
        report_brep_states(nstates, states);  
        loop_kids[2].child = edges[2];  
        loop_kids[2].orientation = UF_BREP_ORIENTATION_REVERSE;  
        geom.type = UF_BREP_LINE_GEOM;  
        geom.geom.line.start_point[0] = 2;  
        geom.geom.line.start_point[1] = 0;  
        geom.geom.line.start_point[2] = 0;  
        geom.geom.line.end_point[0] = 2;  
        geom.geom.line.end_point[1] = 0;  
        geom.geom.line.end_point[2] = 2;  
        UF_CALL(UF_BREP_attach_geometry( edges[3], &geom,  
                UF_BREP_ORIENTATION_REVERSE, &options, &nstates, &states));  
        report_brep_states(nstates, states);  
        loop_kids[3].child = edges[3];  
        loop_kids[3].orientation = UF_BREP_ORIENTATION_REVERSE;  
        geom.geom.line.start_point[0] = -2;  
        geom.geom.line.start_point[1] = 0;  
        geom.geom.line.start_point[2] = 2;  
        geom.geom.line.end_point[0] = -2;  
        geom.geom.line.end_point[1] = 0;  
        geom.geom.line.end_point[2] = 0;  
        UF_CALL(UF_BREP_attach_geometry( edges[1], &geom,  
                UF_BREP_ORIENTATION_REVERSE, &options, &nstates, &states));  
        report_brep_states(nstates, states);  
        loop_kids[1].child = edges[1];  
        loop_kids[1].orientation = UF_BREP_ORIENTATION_REVERSE;  
        loop = UF_allocate_memory(sizeof(UF_BREP_topology_t), &irc);  
        if (irc) UF_CALL(irc);  
        UF_BREP_INITIALIZE_TOPOLOGY(loop, loop_kids, 4, UF_BREP_LOOP_TOPO);  
        face_kid = UF_allocate_memory(sizeof(UF_BREP_oriented_child_t), &irc);  
        face_kid->child = loop;  
        face_kid->orientation = UF_BREP_ORIENTATION_FORWARD;  
        face = UF_allocate_memory(sizeof(UF_BREP_topology_t), &irc);  
        if (irc) UF_CALL(irc);  
        UF_BREP_INITIALIZE_TOPOLOGY(face, face_kid, 1, UF_BREP_FACE_TOPO);  
        geom.type = UF_BREP_CYLINDER_GEOM;  
        geom.geom.cylinder.pt_on_axis[0] = 0.0;  
        geom.geom.cylinder.pt_on_axis[1] = 0.0;  
        geom.geom.cylinder.pt_on_axis[2] = 0.0;  
        geom.geom.cylinder.axis[0] = 0.0;  
        geom.geom.cylinder.axis[1] = 0.0;  
        geom.geom.cylinder.axis[2] = 1.0;  
        geom.geom.cylinder.u0_ref_direc[0] = 0.0;  
        geom.geom.cylinder.u0_ref_direc[1] = 1.0;  
        geom.geom.cylinder.u0_ref_direc[2] = 0.0;  
        geom.geom.cylinder.radius = 2.0;  
        UF_CALL(UF_BREP_attach_geometry( face, &geom,  
            UF_BREP_ORIENTATION_FORWARD, &options, &nstates, &states));  
        report_brep_states(nstates, states);  
        UF_CALL(UF_BREP_validate_topology(face, &nstates, &states));  
        report_brep_states(nstates, states);  
        UF_CALL(UF_BREP_make_body(face, &body, &nstates, &states));  
        report_brep_states(nstates, states);  
        UF_CALL(UF_BREP_release_topology(face, NULL));  
        UF_CALL(UF_DISP_add_item_to_display(body));  
        UF_CALL(UF_MODL_update());  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一些宏，用于错误报告和信息输出。
> 2. 定义了一个函数report_brep_topology，用于报告BREP拓扑结构的信息。
> 3. 定义了一个函数report_brep_states，用于报告BREP的状态信息。
> 4. 定义了一个函数do_it，用于创建一个简单的BREP模型：创建了4个边，分别赋予圆弧和线段的几何。创建一个环，包含这4个边。创建一个面，包含这个环。给这个面赋予圆柱的几何。验证面的拓扑结构。生成一个体，并添加到显示中。
> 5. 创建了4个边，分别赋予圆弧和线段的几何。
> 6. 创建一个环，包含这4个边。
> 7. 创建一个面，包含这个环。
> 8. 给这个面赋予圆柱的几何。
> 9. 验证面的拓扑结构。
> 10. 生成一个体，并添加到显示中。
> 11. 定义了ufusr函数，作为NX的入口函数，调用do_it函数来执行上述功能。
> 12. 定义了ufusr_ask_unload函数，用于卸载该程序。
>
> 总体来说，这段代码提供了一个简单的NX BREP模型创建的示例，涵盖了BREP边、环、面和体的创建以及验证过程，适合作为NX二次开发的入门示例。
>
