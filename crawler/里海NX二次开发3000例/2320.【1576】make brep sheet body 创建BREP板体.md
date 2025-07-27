### 【1576】make brep sheet body 创建BREP板体

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_brep.h>  
    #include <uf_obj.h>  
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
        for (ii = 0; ii < depth; ii++) printf ("  ");  
        switch (a_topology->type)  
        {  
            case UF_BREP_SOLID_TOPO:    printf ("Solid");       break;  
            case UF_BREP_SHEET_TOPO:    printf ("Sheet");       break;  
            case UF_BREP_TRIMSURF_TOPO: printf ("Trimsurf");    break;  
            case UF_BREP_SHELL_TOPO:    printf ("Shell");       break;  
            case UF_BREP_FACE_TOPO:     printf ("Face");        break;  
            case UF_BREP_LOOP_TOPO:     printf ("Loop");        break;  
            case UF_BREP_FIN_TOPO:      printf ("Fin");         break;  
            case UF_BREP_EDGE_TOPO:     printf ("Edge");        break;  
            case UF_BREP_VERTEX_TOPO:   printf ("Vertex");      break;  
            default:                    printf ("Unknown");     break;  
        }  
        UF_BREP_ask_identifier(a_topology, &unique_id);  
        printf(":  UGtag=%d, id=%d\n", a_topology->tag, unique_id);  
        if (depth >= 0)  
        for (ii = 0; ii < a_topology->num_children; ii++)  
            print_topology_tree(a_topology->children[ii].child, depth+1);  
    }  
    static void report_brep_states(int nstates, UF_BREP_state_t *states)  
    {  
        int  
            ii;  
        for (ii = 0; ii < nstates; ii++)  
        {  
            printf("%d.  state_code = %d, flag = %d, value = %f,%f,%f\n", ii+1,  
                states[ii].state_code, states[ii].flag,  
                states[ii].value[0], states[ii].value[1], states[ii].value[2]);  
            switch (states[ii].state_code)  
            {  
                case UF_BREP_STATE_unknown_child:  
                    printf ("\tUF_BREP_STATE_unknown_child\n");  
                    break;  
                case UF_BREP_STATE_duplicate_child:  
                    printf("\tUF_BREP_STATE_duplicate_child\n");  
                    printf("\t\t(Repeated parent-child relationship)\n");  
                    break;  
                case UF_BREP_STATE_wrong_child:  
                    printf("\tUF_BREP_STATE_wrong_child\n");  
                    printf("\t\t(Child topology type not consistent with parent)\n");  
                    break;  
                case UF_BREP_STATE_missing_child:  
                    printf("\tUF_BREP_STATE_missing_child\n");  
                    printf("\t\t(Parent topology has too few children)\n");  
                    break;  
                case UF_BREP_STATE_extra_child:  
                    printf("\tUF_BREP_STATE_extra_child\n");  
                    printf("\t\t(Parent topology has too many children)\n");  
                    break;  
                case UF_BREP_STATE_wrong_parent:  
                    printf("\tUF_BREP_STATE_wrong_parent\n");  
                    printf("\t\t(Vertex has both edge and loop parents)\n");  
                    break;  
                case UF_BREP_STATE_missing_parent:  
                    printf("\tUF_BREP_STATE_missing_parent\n");  
                    printf("\t\t(Child topology has too few parents)\n");  
                    break;  
                case UF_BREP_STATE_extra_parent:  
                    printf("\tUF_BREP_STATE_extra_parent\n");  
                    printf("\t\t(Child topology has too many parents)\n");  
                    break;  
                case UF_BREP_STATE_bad_loop:  
                    printf("\tUF_BREP_STATE_bad_loop\n");  
                    printf("\t\t(Loop edges' start & end vertices\n");  
                    printf("\t\t and sense do not match up)\n");  
                    break;  
                case UF_BREP_STATE_disjoint_shell:  
                    printf("\tUF_BREP_STATE_disjoint_shell\n");  
                    printf("\t\t(Shell's faces not all connected)\n");  
                    break;  
                case UF_BREP_STATE_non_manifold:  
                    printf("\tUF_BREP_STATE_non_manifold\n");  
                    printf("\t\t(Non-manifold vertex)\n");  
                    break;  
                case UF_BREP_STATE_topology_error:  
                    printf("\tUF_BREP_STATE_topology_error\n");  
                    printf("\t\t(Uncategorizable topology error)\n");  
                    break;  
                case UF_BREP_STATE_unknown_body_fault:  
                    printf("\tUF_BREP_STATE_unknown_body_fault\n");  
                    printf("\t\t(Uncategorizable body failure)\n");  
                    break;  
                case UF_BREP_STATE_body_inside_out:  
                    printf("\tUF_BREP_STATE_body_inside_out\n");  
                    break;  
                case UF_BREP_STATE_self_intersecting:  
                    printf("\tUF_BREP_STATE_self_intersecting\n");  
                    break;  
                case UF_BREP_STATE_degenerate_geometry:  
                    printf("\tUF_BREP_STATE_degenerate_geometry\n");  
                    break;  
                case UF_BREP_STATE_invalid_geometry:  
                    printf("\tUF_BREP_STATE_invalid_geometry\n");  
                    break;  
                case UF_BREP_STATE_face_face_inconsistency:  
                    printf("\tUF_BREP_STATE_face_face_inconsistency\n");  
                    break;  
                case UF_BREP_STATE_face_error:  
                    printf("\tUF_BREP_STATE_face_error\n");  
                    printf("\t\t(uncategorizable face problem)\n");  
                    break;  
                case UF_BREP_STATE_vertex_not_on_curve:  
                    printf("\tUF_BREP_STATE_vertex_not_on_curve\n");  
                    printf("\t\t(specify larger tolerance)\n");  
                    break;  
                case UF_BREP_STATE_edge_reversed:  
                    printf("\tUF_BREP_STATE_edge_reversed\n");  
                    printf("\t\t(vertex order appears backward)\n");  
                    break;  
                case UF_BREP_STATE_vertex_not_on_surface:  
                    printf("\tUF_BREP_STATE_vertex_not_on_surface\n");  
                    printf("\t\t(specify larger tolerance)\n");  
                    break;  
                case UF_BREP_STATE_edge_not_on_surface:  
                    printf("\tUF_BREP_STATE_edge_not_on_surface\n");  
                    printf("\t\t(specify larger tolerance)\n");  
                    break;  
                case UF_BREP_STATE_loop_inconsistent:  
                    printf("\tUF_BREP_STATE_loop_inconsistent\n");  
                    printf("\t\t(loop vertices out of order)\n");  
                    break;  
                case UF_BREP_STATE_face_or_edge_non_g1:  
                    printf("\tUF_BREP_STATE_face_or_edge_non_g1\n");  
                    printf("\t\t(not G1 continuous)\n");  
                    break;  
                case UF_BREP_STATE_invalid_entity_ids:  
                    printf("\tUF_BREP_STATE_invalid_entity_ids\n");  
                    break;  
                case UF_BREP_STATE_edge_tol_too_big:  
                    printf("\tUF_BREP_STATE_edge_tol_too_big\n");  
                    printf("\t\t(required tolerance exceeds user request)\n");  
                    break;  
                case UF_BREP_STATE_inconsistent_geom:  
                    printf("\tUF_BREP_STATE_inconsistent_geom\n");  
                    break;  
                case UF_BREP_STATE_invalid_geom:  
                    printf("\tUF_BREP_STATE_invalid_geom\n");  
                    printf("\t\t(geometry does not pass checks)\n");  
                    break;  
                case UF_BREP_STATE_unknown_geom_fault:  
                    printf("\tUF_BREP_STATE_unknown_geom_fault\n");  
                    printf("\t\t(possibly corrupt geometry data)\n");  
                    break;  
                case UF_BREP_STATE_bad_child_count:  
                    printf("\tUF_BREP_STATE_bad_child_count\n");  
                    printf("\t\t(child count illegal for parent topology)\n");  
                    break;  
                case UF_BREP_STATE_null_child_array:  
                    printf("\tUF_BREP_STATE_null_child_array\n");  
                    printf("\t\t(children exist but child array is NULL)\n");  
                    break;  
                case UF_BREP_STATE_null_child_pointer:  
                    printf("\tUF_BREP_STATE_null_child_pointer\n");  
                    printf("\t\t(a child pointer is NULL)\n");  
                    break;  
                case UF_BREP_STATE_multi_vertex_loop:  
                    printf("\tUF_BREP_STATE_multi_vertex_loop\n");  
                    printf("\t\t(loop can have only one vertex e.g. tip of cone)\n");  
                    break;  
                default:  
                    printf("\tUnknown state code\n");  
                    break;  
            }  
        }  
        if (nstates > 0) UF_free(states);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            irc,  
            nstates;  
        tag_t  
            body;  
        double  
            corners[12] = {  0,0,0, 2,0,0, 2,1,0, 0,1,0 };  
        UF_BREP_oriented_child_t  
            *face_kid,  
            *loop_kids;  
        UF_BREP_topology_t  
            *edges[4],  
            *face,  
            *loop,  
            *sheet;  
        UF_BREP_geometry_t  
            geom;  
        UF_BREP_options_t  
            options = { 0, NULL, NULL };  
        UF_BREP_state_t  
            *states;  
        geom.type = UF_BREP_LINE_GEOM;  
        loop_kids = UF_allocate_memory(4 * sizeof(UF_BREP_oriented_child_t), &irc);  
        for (ii = 0; ii < 4; ii++)  
        {  
            edges[ii] = UF_allocate_memory(sizeof(UF_BREP_topology_t), &irc);  
            if (irc) UF_CALL(irc);  
            UF_BREP_INITIALIZE_TOPOLOGY(edges[ii], NULL, 0, UF_BREP_EDGE_TOPO);  
            geom.geom.line.start_point[0] = corners[ii*3];  
            geom.geom.line.start_point[1] = corners[ii*3+1];  
            geom.geom.line.start_point[2] = corners[ii*3+2];  
            if (ii < 3)  
            {  
                geom.geom.line.end_point[0] = corners[ii*3+3];  
                geom.geom.line.end_point[1] = corners[ii*3+4];  
                geom.geom.line.end_point[2] = corners[ii*3+5];  
            }  
            else  
            {  
                geom.geom.line.end_point[0] = corners[0];  
                geom.geom.line.end_point[1] = corners[1];  
                geom.geom.line.end_point[2] = corners[2];  
            }  
            UF_CALL(UF_BREP_attach_geometry( edges[ii], &geom,  
                UF_BREP_ORIENTATION_FORWARD, &options, &nstates, &states));  
            report_brep_states(nstates, states);  
            loop_kids[ii].child = edges[ii];  
            loop_kids[ii].orientation = UF_BREP_ORIENTATION_FORWARD;  
        }  
        loop = UF_allocate_memory(sizeof(UF_BREP_topology_t), &irc);  
        if (irc) UF_CALL(irc);  
        UF_BREP_INITIALIZE_TOPOLOGY(loop, loop_kids, 4, UF_BREP_LOOP_TOPO);  
        face_kid = UF_allocate_memory(sizeof(UF_BREP_oriented_child_t), &irc);  
        face_kid->child = loop;  
        face_kid->orientation = UF_BREP_ORIENTATION_FORWARD;  
        face = UF_allocate_memory(sizeof(UF_BREP_topology_t), &irc);  
        if (irc) UF_CALL(irc);  
        UF_BREP_INITIALIZE_TOPOLOGY(face, face_kid, 1, UF_BREP_FACE_TOPO);  
        geom.type = UF_BREP_PLANE_GEOM;  
        geom.geom.plane.normal[0] = 0.0;  
        geom.geom.plane.normal[1] = 0.0;  
        geom.geom.plane.normal[2] = 1.0;  
        geom.geom.plane.pt_on_plane[0] = 0.0;  
        geom.geom.plane.pt_on_plane[1] = 0.0;  
        geom.geom.plane.pt_on_plane[2] = 0.0;  
        geom.geom.plane.u0_ref_direc[0] = 1.0;  
        geom.geom.plane.u0_ref_direc[1] = 0.0;  
        geom.geom.plane.u0_ref_direc[2] = 0.0;  
        UF_CALL(UF_BREP_attach_geometry( face, &geom,  
            UF_BREP_ORIENTATION_FORWARD, &options, &nstates, &states));  
        report_brep_states(nstates, states);  
        UF_CALL(UF_BREP_validate_topology(face, &nstates, &states));  
        report_brep_states(nstates, states);  
        UF_CALL(UF_BREP_make_body(face, &body, &nstates, &states));  
        report_brep_states(nstates, states);  
        print_topology_tree(face, 0);  
        UF_CALL(UF_BREP_release_topology(face, NULL));  
        UF_CALL(UF_DISP_add_item_to_display(body));  
        UF_CALL(UF_MODL_update());  
        UF_CALL(UF_BREP_ask_topology(body, 0, &sheet, &nstates, &states));  
        report_brep_states(nstates, states);  
        print_topology_tree(sheet, 0);  
        UF_CALL(UF_BREP_release_topology(sheet, NULL));  
    }  
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

> 这是段NX二次开发的示例代码，主要用于创建一个简单平面四边形模型，并显示在NX界面上。
>
> 代码的主要功能包括：
>
> 1. 定义了一个名为ECHO的函数，用于打印错误信息。
> 2. 定义了一个名为report_error的函数，用于报告函数调用失败时的错误码和错误消息。
> 3. 定义了一个名为print_topology_tree的函数，用于递归打印BREP拓扑树的类型和标识信息。
> 4. 定义了一个名为report_brep_states的函数，用于打印BREP验证状态码及其含义。
> 5. 定义了一个名为do_it的函数，用于执行创建模型的流程。
> 6. 在do_it函数中，首先定义了四条边的几何信息，然后创建了一个循环，将这四条边添加到模型中。
> 7. 接着创建了一个面，将四条边添加到面中，并定义了面的几何信息。
> 8. 调用UF_BREP_validate_topology验证面的拓扑结构，打印验证结果。
> 9. 调用UF_BREP_make_body生成实体，并打印验证结果。
> 10. 打印创建的面拓扑树，显示在NX界面上。
> 11. 释放面拓扑树。
> 12. 获取生成的实体拓扑树，打印并释放。
> 13. 最后，在ufusr函数中初始化NX，调用do_it函数，并释放NX。
> 14. 提供了ufusr_ask_unload函数，用于立即卸载二次开发代码。
>
> 总体来说，这段代码通过BREP API展示了如何创建一个简单的NX模型，并提供了打印验证结果和拓扑结构的功能，对学习NX二次开发具有一定的参考价值。
>
