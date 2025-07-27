### 【1613】make trimmed brep body 创建修剪后的BREP实体

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_brep.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN + 1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char *file, int line, char *call, int irc)  
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
    static void report_brep_states(int nstates, UF_BREP_state_t *states)  
    {  
        int  
            ii;  
        for (ii = 0; ii < nstates; ii++)  
        {  
            ECHO("%d.  state_code = %d, flag = %d, value = %f,%f,%f\n", ii + 1,  
                states[ii].state_code, states[ii].flag,  
                states[ii].value[0], states[ii].value[1], states[ii].value[2]);  
            switch (states[ii].state_code)  
            {  
            case UF_BREP_STATE_unknown_child:  
                ECHO("\tUF_BREP_STATE_unknown_child\n");  
                break;  
            case UF_BREP_STATE_duplicate_child:  
                ECHO("\tUF_BREP_STATE_duplicate_child\n");  
                ECHO("\t\t(Repeated parent-child relationship)\n");  
                break;  
            case UF_BREP_STATE_wrong_child:  
                ECHO("\tUF_BREP_STATE_wrong_child\n");  
                ECHO("\t\t(Child topology type not consistent with parent)\n");  
                break;  
            case UF_BREP_STATE_missing_child:  
                ECHO("\tUF_BREP_STATE_missing_child\n");  
                ECHO("\t\t(Parent topology has too few children)\n");  
                break;  
            case UF_BREP_STATE_extra_child:  
                ECHO("\tUF_BREP_STATE_extra_child\n");  
                ECHO("\t\t(Parent topology has too many children)\n");  
                break;  
            case UF_BREP_STATE_wrong_parent:  
                ECHO("\tUF_BREP_STATE_wrong_parent\n");  
                ECHO("\t\t(Vertex has both edge and loop parents)\n");  
                break;  
            case UF_BREP_STATE_missing_parent:  
                ECHO("\tUF_BREP_STATE_missing_parent\n");  
                ECHO("\t\t(Child topology has too few parents)\n");  
                break;  
            case UF_BREP_STATE_extra_parent:  
                ECHO("\tUF_BREP_STATE_extra_parent\n");  
                ECHO("\t\t(Child topology has too many parents)\n");  
                break;  
            case UF_BREP_STATE_bad_loop:  
                ECHO("\tUF_BREP_STATE_bad_loop\n");  
                ECHO("\t\t(Loop edges' start & end vertices\n");  
                ECHO("\t\t and sense do not match up)\n");  
                break;  
            case UF_BREP_STATE_disjoint_shell:  
                ECHO("\tUF_BREP_STATE_disjoint_shell\n");  
                ECHO("\t\t(Shell's faces not all connected)\n");  
                break;  
            case UF_BREP_STATE_non_manifold:  
                ECHO("\tUF_BREP_STATE_non_manifold\n");  
                ECHO("\t\t(Non-manifold vertex)\n");  
                break;  
            case UF_BREP_STATE_topology_error:  
                ECHO("\tUF_BREP_STATE_topology_error\n");  
                ECHO("\t\t(Uncategorizable topology error)\n");  
                break;  
            case UF_BREP_STATE_unknown_body_fault:  
                ECHO("\tUF_BREP_STATE_unknown_body_fault\n");  
                ECHO("\t\t(Uncategorizable body failure)\n");  
                break;  
            case UF_BREP_STATE_body_inside_out:  
                ECHO("\tUF_BREP_STATE_body_inside_out\n");  
                break;  
            case UF_BREP_STATE_self_intersecting:  
                ECHO("\tUF_BREP_STATE_self_intersecting\n");  
                break;  
            case UF_BREP_STATE_degenerate_geometry:  
                ECHO("\tUF_BREP_STATE_degenerate_geometry\n");  
                break;  
            case UF_BREP_STATE_invalid_geometry:  
                ECHO("\tUF_BREP_STATE_invalid_geometry\n");  
                break;  
            case UF_BREP_STATE_face_face_inconsistency:  
                ECHO("\tUF_BREP_STATE_face_face_inconsistency\n");  
                break;  
            case UF_BREP_STATE_face_error:  
                ECHO("\tUF_BREP_STATE_face_error\n");  
                ECHO("\t\t(uncategorizable face problem)\n");  
                break;  
            case UF_BREP_STATE_vertex_not_on_curve:  
                ECHO("\tUF_BREP_STATE_vertex_not_on_curve\n");  
                ECHO("\t\t(specify larger tolerance)\n");  
                break;  
            case UF_BREP_STATE_edge_reversed:  
                ECHO("\tUF_BREP_STATE_edge_reversed\n");  
                ECHO("\t\t(vertex order appears backward)\n");  
                break;  
            case UF_BREP_STATE_vertex_not_on_surface:  
                ECHO("\tUF_BREP_STATE_vertex_not_on_surface\n");  
                ECHO("\t\t(specify larger tolerance)\n");  
                break;  
            case UF_BREP_STATE_edge_not_on_surface:  
                ECHO("\tUF_BREP_STATE_edge_not_on_surface\n");  
                ECHO("\t\t(specify larger tolerance)\n");  
                break;  
            case UF_BREP_STATE_loop_inconsistent:  
                ECHO("\tUF_BREP_STATE_loop_inconsistent\n");  
                ECHO("\t\t(loop vertices out of order)\n");  
                break;  
            case UF_BREP_STATE_face_or_edge_non_g1:  
                ECHO("\tUF_BREP_STATE_face_or_edge_non_g1\n");  
                ECHO("\t\t(not G1 continuous)\n");  
                break;  
            case UF_BREP_STATE_invalid_entity_ids:  
                ECHO("\tUF_BREP_STATE_invalid_entity_ids\n");  
                break;  
            case UF_BREP_STATE_edge_tol_too_big:  
                ECHO("\tUF_BREP_STATE_edge_tol_too_big\n");  
                ECHO("\t\t(required tolerance exceeds user request)\n");  
                break;  
            case UF_BREP_STATE_inconsistent_geom:  
                ECHO("\tUF_BREP_STATE_inconsistent_geom\n");  
                break;  
            case UF_BREP_STATE_invalid_geom:  
                ECHO("\tUF_BREP_STATE_invalid_geom\n");  
                ECHO("\t\t(geometry does not pass checks)\n");  
                break;  
            case UF_BREP_STATE_unknown_geom_fault:  
                ECHO("\tUF_BREP_STATE_unknown_geom_fault\n");  
                ECHO("\t\t(possibly corrupt geometry data)\n");  
                break;  
            case UF_BREP_STATE_bad_child_count:  
                ECHO("\tUF_BREP_STATE_bad_child_count\n");  
                ECHO("\t\t(child count illegal for parent topology)\n");  
                break;  
            case UF_BREP_STATE_null_child_array:  
                ECHO("\tUF_BREP_STATE_null_child_array\n");  
                ECHO("\t\t(children exist but child array is NULL)\n");  
                break;  
            case UF_BREP_STATE_null_child_pointer:  
                ECHO("\tUF_BREP_STATE_null_child_pointer\n");  
                ECHO("\t\t(a child pointer is NULL)\n");  
                break;  
            case UF_BREP_STATE_multi_vertex_loop:  
                ECHO("\tUF_BREP_STATE_multi_vertex_loop\n");  
                ECHO("\t\t(loop can have only one vertex e.g. tip of cone)\n");  
                break;  
            default:  
                ECHO("\tUnknown state code\n");  
                break;  
            }  
        }  
        if (nstates > 0) UF_free(states);  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        if (resp == 1) return TRUE;  
        else return FALSE;  
    }  
    static void display_temporary_line(double start[3], double end[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
            UF_OBJ_FONT_SOLID, FALSE };  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
    }  
    static tag_t do_it(void)  
    {  
        int  
            ii,  
            irc,  
            nstates;  
        tag_t  
            body;  
        UF_BREP_oriented_child_t  
            *face_kid,  
            *loop_kids;  
        UF_BREP_topology_t  
            *fins[4],  
            *face,  
            *loop;  
        UF_BREP_geometry_t  
            geom;  
        UF_BREP_options_t  
            options = { 0, NULL, NULL };  
        UF_BREP_state_t  
            *states;  
        // create 4 fins  
        double knots_fin[4] = { 0.0, 0.0, 1.0, 1.0 };  /* Knot vector 里海译:结向量 */  
        double poles_fin[][4] = {  
            { 0.25, 0.9, 0.0, 1.0 },  
            { 0.25, 0.75, 0.0, 1.0 },  
            { 0.5, 0.75, 0.0, 1.0 },  
            { 0.5, 0.9, 0.0, 1.0 },  
            { 0.25, 0.9, 0.0, 1.0 }    };  // Counterclock and closed loop  
        geom.type = UF_BREP_SPLINE_GEOM;  
        loop_kids = (UF_BREP_oriented_child_t*)UF_allocate_memory(4 * sizeof(UF_BREP_oriented_child_t), &irc);  
        for (ii = 0; ii < 4; ii++)  
        {  
            fins[ii] = (UF_BREP_topology_t*)UF_allocate_memory(sizeof(UF_BREP_topology_t), &irc);  
            if (irc) UF_CALL(irc);  
            UF_BREP_INITIALIZE_TOPOLOGY(fins[ii], NULL, 0, UF_BREP_FIN_TOPO);  
            geom.geom.spline.is_rational = false;  
            geom.geom.spline.start_param = 0.0;  
            geom.geom.spline.end_param = 1.0;  
            geom.geom.spline.order = 2;  
            geom.geom.spline.knots = knots_fin;  
            geom.geom.spline.num_poles = 2;  
            geom.geom.spline.poles = &poles_fin[ii];  
            int fin_option_token[1] = { UF_BREP_FINSP_OPTION };  
            double fin_option_data = 0.01;  
            UF_BREP_options_t fin_options = { 1, fin_option_token, &fin_option_data };  
            UF_CALL(UF_BREP_attach_geometry(fins[ii], &geom,  
                UF_BREP_ORIENTATION_FORWARD, &options, &nstates, &states));  
            report_brep_states(nstates, states);  
            // add fins to loop  
            loop_kids[ii].child = fins[ii];  
            loop_kids[ii].orientation = UF_BREP_ORIENTATION_FORWARD;  
        }  
        // create the loop   
        loop = (UF_BREP_topology_t*)UF_allocate_memory(sizeof(UF_BREP_topology_t), &irc);  
        if (irc) UF_CALL(irc);  
        UF_BREP_INITIALIZE_TOPOLOGY(loop, loop_kids, 4, UF_BREP_LOOP_TOPO);  
        face_kid = (UF_BREP_oriented_child_t*)UF_allocate_memory(sizeof(UF_BREP_oriented_child_t), &irc);  
        face_kid->child = loop;  
        face_kid->orientation = UF_BREP_ORIENTATION_FORWARD;  
        // create the face based on bsurface geometry  
        face = (UF_BREP_topology_t*)UF_allocate_memory(sizeof(UF_BREP_topology_t), &irc);  
        if (irc) UF_CALL(irc);  
        if (ask_yes_or_no("UF_BREP_INITIALIZE_TOPOLOGY", "With Trim Topology?"))  
        {  
            UF_BREP_INITIALIZE_TOPOLOGY(face, face_kid, 1, UF_BREP_FACE_TOPO);  
        }  
        else  
        {  
            UF_BREP_INITIALIZE_TOPOLOGY(face, NULL, 0, UF_BREP_FACE_TOPO);  
        }  
        double knots_u[4] = { 0.0, 0.0, 1.0, 1.0 }; /* Knot vector for the u direction. 里海译:u方向的结向量 */  
        double knots_v[4] = { 0.0, 0.0, 1.0, 1.0 }; /* Knot vector for the v direction. 里海译:向量Knot vector for the v direction的翻译是：v方向的节点向量。 */  
        double poles[4][4] = {  
            { -100, 0, 25, 1 },  
            { -100, 40, 25, 1 },  
            { 0, 0, 55, 1 },  
            { 0, 40, 55, 1 } };  
        // display original untrimmed boundary  
        display_temporary_line(poles[0], poles[1]);  
        display_temporary_line(poles[1], poles[3]);  
        display_temporary_line(poles[3], poles[2]);  
        display_temporary_line(poles[2], poles[0]);  
        UF_BREP_geometry_t nurbs;  
        nurbs.type = UF_BREP_BSURFACE_GEOM;  
        nurbs.geom.bsurface.is_rational = false;  
        nurbs.geom.bsurface.num_poles_u = 2;  
        nurbs.geom.bsurface.num_poles_v = 2;  
        nurbs.geom.bsurface.order_u = 2;  
        nurbs.geom.bsurface.order_v = 2;  
        nurbs.geom.bsurface.knots_u = knots_u;  
        nurbs.geom.bsurface.knots_v = knots_v;  
        nurbs.geom.bsurface.poles = poles;  
        UF_CALL(UF_BREP_attach_geometry(face, &nurbs,  
            UF_BREP_ORIENTATION_FORWARD, &options, &nstates, &states));  
        report_brep_states(nstates, states);  
        UF_CALL(UF_BREP_validate_topology(face, &nstates, &states));  
        report_brep_states(nstates, states);  
        // create the body  
        UF_CALL(UF_BREP_make_body(face, &body, &nstates, &states));  
        report_brep_states(nstates, states);  
        UF_CALL(UF_BREP_release_topology(face, NULL));  
        UF_CALL(UF_DISP_add_item_to_display(body));  
        // Update part navigator  
        UF_CALL( UF_MODL_update());  
        return body;  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            tag_t brep_body = do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 创建了4个曲面细分成4个loop，每个loop由一条NURBS曲线组成。
> 2. 将4个loop组合成一个环面，构成一个面。
> 3. 为面添加NURBS曲面几何，创建一个环面。
> 4. 验证面的拓扑结构，确保无错误。
> 5. 将面转换成一个体，并显示在NX中。
> 6. 释放面相关的拓扑结构。
> 7. 更新NX的部件导航器。
> 8. 提供了错误报告和提示功能。
> 9. 包含了用户交互，如询问是否添加修剪边界。
> 10. 使用UF_BREP相关的API完成BREP相关操作。
>
> 总体而言，这段代码通过BREP相关的API实现了一个简单的NX二次开发示例，展示了如何创建面和体，以及进行错误检查等操作。
>
