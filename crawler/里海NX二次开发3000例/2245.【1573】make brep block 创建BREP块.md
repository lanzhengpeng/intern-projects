### 【1573】make brep block 创建BREP块

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_brep.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
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
    static void print_topology_tree(UF_BREP_topology_t *a_topology, int depth)  
    {  
        int  
            ii,  
            unique_id;  
        char  
            buf1[133] = { "" },  
            buf2[133];  
        if (depth == 0) UF_UI_open_listing_window();  
        for (ii = 0; ii < depth; ii++) strcat(buf1, "  ");  
        switch (a_topology->type)  
        {  
            case UF_BREP_SOLID_TOPO:    strcat(buf1, "Solid");       break;  
            case UF_BREP_SHEET_TOPO:    strcat(buf1, "Sheet");       break;  
            case UF_BREP_TRIMSURF_TOPO: strcat(buf1, "Trimsurf");    break;  
            case UF_BREP_SHELL_TOPO:    strcat(buf1, "Shell");       break;  
            case UF_BREP_FACE_TOPO:     strcat(buf1, "Face");        break;  
            case UF_BREP_LOOP_TOPO:     strcat(buf1, "Loop");        break;  
            case UF_BREP_FIN_TOPO:      strcat(buf1, "Fin");         break;  
            case UF_BREP_EDGE_TOPO:     strcat(buf1, "Edge");        break;  
            case UF_BREP_VERTEX_TOPO:   strcat(buf1, "Vertex");      break;  
            default:                    strcat(buf1, "Unknown");     return;  
        }  
        UF_BREP_ask_identifier(a_topology, &unique_id);  
        sprintf(buf2,"%s:  UGtag=%d, id=%d\n", buf1, a_topology->tag, unique_id);  
        UF_UI_write_listing_window( buf2 );  
        if (depth >= 0)  
        for (ii = 0; ii < a_topology->num_children; ii++)  
            print_topology_tree(a_topology->children[ii].child, depth+1);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
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
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
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
                WRITE_D(states[ii].state_code);  
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
                        break;  
                }  
                WRITE_D(states[ii].flag);  
                WRITE3F(states[ii].value);  
                if (states[ii].item != NULL)  
                    print_topology_tree(states[ii].item, 0);  
                WRITE_D(states[ii].ug_tag);  
                WRITE_D(states[ii].misc);  
            }  
            UF_free(states);  
        }  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void do_it(void)  
    {  
        int  
            edge_map[12][2] = { {1,2}, {2,3}, {3,4}, {4,1},  
                                {1,5}, {2,6}, {3,7}, {4,8},  
                                {5,6}, {6,7}, {7,8}, {8,5} },  
            loop_map[6][4] = { {-4, -3, -2, -1},  
                                {1, 6,  -9, -5},  
                                {2, 7, -10, -6},  
                                {3, 8, -11, -7},  
                                {4, 5, -12, -8},  
                                {9, 10, 11, 12 }},  
            ii,  
            nstates;  
        tag_t  
            body;  
        double  
            corners[8][3]  = { {0,0,0}, {2,0,0}, {2,1,0}, {0,1,0},  
                               {0,0,3}, {2,0,3}, {2,1,3}, {0,1,3} },  
            mag,  
            normals[6][3] = { { 0.0, 0.0, -1.0 },  
                              { 0.0, -1.0, 0.0 },  
                              { 1.0, 0.0, 0.0 },  
                              { 0.0, 1.0, 0.0 },  
                              { -1.0, 0.0, 0.0 },  
                              { 0.0, 0.0, 1.0 } },  
            pt_on_planes[6][3] = { { 1.0, 0.5, 0.0 },  
                                   { 1.0, 0.0, 1.5 },  
                                   { 2.0, 0.5, 1.5 },  
                                   { 1.0, 1.0, 1.5 },  
                                   { 0.0, 0.5, 1.5 },  
                                   { 1.0, 0.5, 3.0 } },  
            u0_ref_direc[6][3] = { { 1.0, 0.0, 0.0 },  
                                   { 1.0, 0.0, 0.0 },  
                                   { 0.0, 1.0, 0.0 },  
                                   { 1.0, 0.0, 0.0 },  
                                   { 0.0, 1.0, 0.0 },  
                                   { 1.0, 0.0, 0.0 } };  
        UF_BREP_oriented_child_t  
            *edge_kids[12],  
            *face_kids[6],  
            *loop_kids[6],  
            *shell_kids,  
            *solid_kid;  
        UF_BREP_topology_t  
            *edge[12],  
            *face[6],  
            *loop[6],  
            *shell,  
            *solid,  
            *vertex[8];  
        UF_BREP_geometry_t  
            geom;  
        UF_BREP_options_t  
            options = { 0, NULL, NULL };  
        UF_BREP_state_t  
            *states;  
        geom.type = UF_BREP_POINT_GEOM;  
        for (ii = 0; ii < 8; ii++)  
        {  
            UF_CALL(allocate_memory(sizeof(UF_BREP_topology_t),  
                (void *)&vertex[ii]));  
            UF_BREP_INITIALIZE_TOPOLOGY(vertex[ii], NULL, 0,  
                UF_BREP_VERTEX_TOPO);  
            UF_VEC3_copy(corners[ii], geom.geom.point);  
            UF_CALL(UF_BREP_attach_geometry( vertex[ii], &geom,  
                UF_BREP_ORIENTATION_NONE, &options, &nstates, &states));  
            report_brep_states(nstates, states);  
        }  
        geom.type = UF_BREP_LINE_GEOM;  
        for (ii = 0; ii < 12; ii++)  
        {  
            UF_CALL(allocate_memory(sizeof(UF_BREP_topology_t),  
                (void *)&edge[ii]));  
            UF_CALL(allocate_memory(2 * sizeof(UF_BREP_oriented_child_t),  
                (void *)&edge_kids[ii]));  
            UF_BREP_INITIALIZE_TOPOLOGY(edge[ii], edge_kids[ii], 2,  
                UF_BREP_EDGE_TOPO);  
            edge_kids[ii][0].child = vertex[edge_map[ii][0]-1];  
            edge_kids[ii][1].child = vertex[edge_map[ii][1]-1];  
            UF_VEC3_copy(corners[edge_map[ii][0]-1], geom.geom.line.start_point);  
            UF_VEC3_copy(corners[edge_map[ii][1]-1], geom.geom.line.end_point);  
            UF_CALL(UF_BREP_attach_geometry(edge[ii], &geom,  
                UF_BREP_ORIENTATION_FORWARD, &options, &nstates, &states));  
            report_brep_states(nstates, states);  
        }  
        UF_CALL(allocate_memory(sizeof(UF_BREP_topology_t),  
            (void *)&shell));  
        UF_CALL(allocate_memory(6 * sizeof(UF_BREP_oriented_child_t),  
            (void *)&shell_kids));  
        UF_BREP_INITIALIZE_TOPOLOGY(shell, shell_kids, 6, UF_BREP_SHELL_TOPO);  
        geom.type = UF_BREP_PLANE_GEOM;  
        for (ii = 0; ii < 6; ii++)  
        {  
            UF_CALL(allocate_memory(sizeof(UF_BREP_topology_t),  
                (void *)&loop[ii]));  
            UF_CALL(allocate_memory(4 * sizeof(UF_BREP_oriented_child_t),  
                (void *)&loop_kids[ii]));  
            UF_BREP_INITIALIZE_TOPOLOGY(loop[ii], loop_kids[ii], 4,  
                UF_BREP_LOOP_TOPO);  
            loop_kids[ii][0].child = edge[abs(loop_map[ii][0])-1];  
            if (loop_map[ii][0] < 0)  
                loop_kids[ii][0].orientation = UF_BREP_ORIENTATION_REVERSE;  
            else  
                loop_kids[ii][0].orientation = UF_BREP_ORIENTATION_FORWARD;  
            loop_kids[ii][1].child = edge[abs(loop_map[ii][1])-1];  
            if (loop_map[ii][1] < 0)  
                loop_kids[ii][1].orientation = UF_BREP_ORIENTATION_REVERSE;  
            else  
                loop_kids[ii][1].orientation = UF_BREP_ORIENTATION_FORWARD;  
            loop_kids[ii][2].child = edge[abs(loop_map[ii][2])-1];  
            if (loop_map[ii][2] < 0)  
                loop_kids[ii][2].orientation = UF_BREP_ORIENTATION_REVERSE;  
            else  
                loop_kids[ii][2].orientation = UF_BREP_ORIENTATION_FORWARD;  
            loop_kids[ii][3].child = edge[abs(loop_map[ii][3])-1];  
            if (loop_map[ii][3] < 0)  
                loop_kids[ii][3].orientation = UF_BREP_ORIENTATION_REVERSE;  
            else  
                loop_kids[ii][3].orientation = UF_BREP_ORIENTATION_FORWARD;  
            UF_CALL(allocate_memory(sizeof(UF_BREP_topology_t),  
                (void *)&face[ii]));  
            UF_CALL(allocate_memory(sizeof(UF_BREP_oriented_child_t),  
                (void *)&face_kids[ii]));  
            UF_BREP_INITIALIZE_TOPOLOGY(face[ii], face_kids[ii], 1,  
                UF_BREP_FACE_TOPO);  
            face_kids[ii]->child = loop[ii];  
            face_kids[ii]->orientation = UF_BREP_ORIENTATION_FORWARD;  
            UF_VEC3_unitize(normals[ii], 0, &mag, geom.geom.plane.normal);  
            UF_VEC3_copy(pt_on_planes[ii], geom.geom.plane.pt_on_plane);  
            UF_VEC3_unitize(u0_ref_direc[ii], 0, &mag,  
                geom.geom.plane.u0_ref_direc);  
            UF_CALL(UF_BREP_attach_geometry( face[ii], &geom,  
                UF_BREP_ORIENTATION_FORWARD, &options, &nstates, &states));  
            report_brep_states(nstates, states);  
            UF_CALL(UF_BREP_validate_topology(face[ii], &nstates, &states));  
            report_brep_states(nstates, states);  
            shell_kids[ii].child = face[ii];  
        }  
        UF_CALL(allocate_memory(sizeof(UF_BREP_topology_t),  
            (void *)&solid));  
        UF_CALL(allocate_memory(sizeof(UF_BREP_oriented_child_t),  
            (void *)&solid_kid));  
        UF_BREP_INITIALIZE_TOPOLOGY(solid, solid_kid, 1, UF_BREP_SOLID_TOPO);  
        solid_kid->child = shell;  
        UF_CALL(UF_BREP_validate_topology(solid, &nstates, &states));  
        report_brep_states(nstates, states);  
        if (!UF_CALL(UF_BREP_make_body(solid, &body, &nstates, &states)))  
        {  
            UF_CALL(UF_DISP_add_item_to_display(body));  
            UF_CALL(UF_BREP_release_topology(solid, NULL));  
            UF_CALL(UF_BREP_ask_topology(body, 0, &solid, &nstates, &states));  
            report_brep_states(nstates, states);  
            print_topology_tree(solid, 0);  
            UF_CALL(UF_BREP_release_topology(solid, NULL));  
            UF_CALL(UF_MODL_update());  
        }  
        else  
        {  
            UF_CALL(UF_BREP_release_topology(solid, NULL));  
            report_brep_states(nstates, states);  
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

> 这段代码是一个NX的二次开发示例，主要实现以下功能：
>
> 1. 定义了一些帮助函数，用于错误报告、输出信息等。
> 2. 创建了一个立方体的BREP模型，包括顶点、边、面、壳体和实体。
> 3. 为每个顶点、边、面等分配了几何属性，如点的位置、线的端点、面的平面等。
> 4. 检查了BREP模型的拓扑结构，确保没有错误。
> 5. 将BREP模型转换为NX的Body，并在NX中显示。
> 6. 最后，释放了BREP模型占用的内存。
>
> 整体来说，这段代码演示了如何在NX中通过BREP接口创建一个简单的立方体模型，包括BREP模型的创建、几何属性分配、拓扑检查和模型显示等完整过程。
>
