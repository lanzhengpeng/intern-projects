### 【2133】report flattened brep topology tree geometry for all visible faces 报告所有可见面的扁平BREP拓扑树几何

#### 代码

```cpp
    /*HEAD REPORT_FLATTENED_BREP_TOPOLOGY_TREE_GEOMETRY_FOR_ALL_VISIBLE_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_brep.h>  
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
UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t ask_next_visible_face(tag_t face)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_VIEW_cycle_objects(NULL_TAG, UF_VIEW_VISIBLE_OBJECTS,  
                &face)) && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_face_subtype))  
                return face;  
        }  
        return NULL_TAG;  
    }  
    #define WRITE(X)      (UF_UI_write_listing_window(X))  
    #define WRITE_D(X)    (write_integer_to_listing_window(#X, X))  
    #define WRITEnD(X,n)  (write_integer_array_to_listing_window(#X, X, 1, n))  
    #define WRITE_F(X)    (write_double_to_listing_window(#X, X))  
    #define WRITE3F(X)    (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITEnF(X,n)  (write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    #define WRITE16F(X)   (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITE_S(X)    (write_string_to_listing_window(#X, X))  
    #define WRITE_L(X)    (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void write_integer_array_to_listing_window(char *name, int *array,  
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
                sprintf(msg, "%d", array[kk++]);  
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
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
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
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void report_brep_geometry(UF_BREP_geometry_t *geometry)  
    {  
        int  
            ii;  
        UF_UI_open_listing_window();  
        WRITE("geometry->type = ");  
        switch (geometry->type)  
        {  
            case UF_BREP_POINT_GEOM:  
                WRITE_D(UF_BREP_POINT_GEOM);  
                WRITE3F(geometry->geom.point);  
                break;  
            case UF_BREP_LINE_GEOM:  
                WRITE_D(UF_BREP_LINE_GEOM);  
                WRITE3F(geometry->geom.line.start_point);  
                WRITE3F(geometry->geom.line.end_point);  
                break;  
            case UF_BREP_SPLINE_GEOM:  
                WRITE_D(UF_BREP_SPLINE_GEOM);  
                WRITE_D(geometry->geom.spline.num_poles);  
                WRITE_D(geometry->geom.spline.order);  
                WRITE_L(geometry->geom.spline.is_rational);  
                WRITEnF(geometry->geom.spline.knots,  
                    geometry->geom.spline.num_poles + geometry->geom.spline.order);  
                WRITEnF4((double *)geometry->geom.spline.poles,  
                    geometry->geom.spline.num_poles);  
                WRITE_F(geometry->geom.spline.start_param);  
                WRITE_F(geometry->geom.spline.end_param);  
                break;  
            case UF_BREP_ARC_GEOM:  
                WRITE_D(UF_BREP_ARC_GEOM);  
                WRITE_D(geometry->geom.arc.matrix_tag);  
                WRITE_F(geometry->geom.arc.start_angle * RADEG);  
                WRITE_F(geometry->geom.arc.end_angle * RADEG);  
                WRITE3F(geometry->geom.arc.arc_center);  
                WRITE_F(geometry->geom.arc.radius);  
                break;  
            case UF_BREP_ELLIPSE_GEOM:  
            case UF_BREP_HYPERBOLA_GEOM:  
            case UF_BREP_PARABOLA_GEOM:  
                WRITE_D(UF_BREP_ELLIPSE_GEOM);  
                WRITE_D(geometry->geom.conic.matrix_tag);  
                WRITE_D(geometry->geom.conic.conic_type);  
                WRITE_F(geometry->geom.conic.rotation_angle);  
                WRITE_F(geometry->geom.conic.start_param);  
                WRITE_F(geometry->geom.conic.end_param);  
                WRITE3F(geometry->geom.conic.center);  
                WRITE_F(geometry->geom.conic.k1);  
                WRITE_F(geometry->geom.conic.k2);  
                break;  
            case UF_BREP_INTERSECTION_GEOM:  
                WRITE_D(UF_BREP_INTERSECTION_GEOM);  
                break;  
            case UF_BREP_COMPOSITE_GEOM:  
                WRITE_D(UF_BREP_COMPOSITE_GEOM);  
                WRITE_D(geometry->geom.composite.num_curves);  
                for (ii = 0; ii < geometry->geom.composite.num_curves; ii++)  
                {  
                    WRITE_D(ii);  
                    report_brep_geometry(&(geometry->geom.composite.curves[ii]));  
                }  
                break;  
            case UF_BREP_CYLINDER_GEOM:  
                WRITE_D(UF_BREP_CYLINDER_GEOM);  
                WRITE3F(geometry->geom.cylinder.pt_on_axis);  
                WRITE3F(geometry->geom.cylinder.axis);  
                WRITE3F(geometry->geom.cylinder.u0_ref_direc);  
                WRITE_F(geometry->geom.cylinder.radius);  
                break;  
            case UF_BREP_CONE_GEOM:  
                WRITE_D(UF_BREP_CONE_GEOM);  
                WRITE3F(geometry->geom.cone.pt_at_apex);  
                WRITE3F(geometry->geom.cone.axis);  
                WRITE3F(geometry->geom.cone.u0_ref_direc);  
                WRITE_F(geometry->geom.cone.half_apex_angle);  
                break;  
            case UF_BREP_SPHERE_GEOM:  
                WRITE_D(UF_BREP_SPHERE_GEOM);  
                WRITE3F(geometry->geom.sphere.center);  
                WRITE3F(geometry->geom.sphere.axis);  
                WRITE3F(geometry->geom.sphere.u0_ref_direc);  
                WRITE_F(geometry->geom.sphere.radius);  
                break;  
            case UF_BREP_TORUS_GEOM:  
                WRITE_D(UF_BREP_TORUS_GEOM);  
                WRITE3F(geometry->geom.torus.center);  
                WRITE3F(geometry->geom.torus.axis);  
                WRITE3F(geometry->geom.torus.u0_ref_direc);  
                WRITE_F(geometry->geom.torus.major_radius);  
                WRITE_F(geometry->geom.torus.minor_radius);  
                break;  
            case UF_BREP_REVOLVE_GEOM:  
                WRITE_D(UF_BREP_REVOLVE_GEOM);  
                WRITE3F(geometry->geom.revolve.pt_on_axis);  
                WRITE3F(geometry->geom.revolve.rot_axis);  
                report_brep_geometry(geometry->geom.revolve.generator);  
                break;  
            case UF_BREP_EXTRUDE_GEOM:  
                WRITE_D(UF_BREP_EXTRUDE_GEOM);  
                WRITE3F(geometry->geom.extrude.direction);  
                report_brep_geometry(geometry->geom.extrude.generator);  
                break;  
            case UF_BREP_BSURFACE_GEOM:  
                WRITE_D(UF_BREP_BSURFACE_GEOM);  
                WRITE_D(geometry->geom.bsurface.num_poles_u);  
                WRITE_D(geometry->geom.bsurface.num_poles_v);  
                WRITE_D(geometry->geom.bsurface.order_u);  
                WRITE_D(geometry->geom.bsurface.order_v);  
                WRITE_L(geometry->geom.bsurface.is_rational);  
                WRITEnF(geometry->geom.bsurface.knots_u,  
                    geometry->geom.bsurface.num_poles_u +  
                    geometry->geom.bsurface.order_u);  
                WRITEnF(geometry->geom.bsurface.knots_v,  
                    geometry->geom.bsurface.num_poles_v +  
                    geometry->geom.bsurface.order_v);  
                WRITEnF4((double *)geometry->geom.bsurface.poles,  
                    geometry->geom.bsurface.num_poles_u *  
                    geometry->geom.bsurface.num_poles_v );  
                break;  
            case UF_BREP_OFFSET_GEOM:  
                WRITE_D(UF_BREP_OFFSET_GEOM);  
                report_brep_geometry(geometry->geom.offset.base);  
                WRITE_F(geometry->geom.offset.distance);  
                break;  
            case UF_BREP_PLANE_GEOM:  
                WRITE_D(UF_BREP_PLANE_GEOM);  
                WRITE3F(geometry->geom.plane.pt_on_plane);  
                WRITE3F(geometry->geom.plane.normal);  
                WRITE3F(geometry->geom.plane.u0_ref_direc);  
                break;  
            case UF_BREP_BLEND_GEOM:  
                WRITE_D(UF_BREP_BLEND_GEOM);  
                break;  
            case UF_BREP_FORSURF_GEOM:  
                WRITE_D(UF_BREP_FORSURF_GEOM);  
                WRITE_S(geometry->geom.foreign.char_data);  
                WRITE_D(geometry->geom.foreign.nspace);  
                WRITE_D(geometry->geom.foreign.num_ints);  
                WRITEnD(geometry->geom.foreign.int_array,  
                    geometry->geom.foreign.num_ints);  
                WRITE_D(geometry->geom.foreign.num_doubles);  
                WRITEnF(geometry->geom.foreign.double_array,  
                    geometry->geom.foreign.num_doubles);  
                WRITE16F((double *)geometry->geom.foreign.transform);  
                break;  
            case UF_BREP_UVBOX_GEOM:  
                WRITE_D(UF_BREP_UVBOX_GEOM);  
                WRITE_F(geometry->geom.uvbox.umin);  
                WRITE_F(geometry->geom.uvbox.umax);  
                WRITE_F(geometry->geom.uvbox.vmin);  
                WRITE_F(geometry->geom.uvbox.vmax);  
                break;  
            case UF_BREP_TAG_GEOM:  
                WRITE_D(UF_BREP_TAG_GEOM);  
                WRITE_D(geometry->geom.tagged.object);  
                break;  
            default:  
                WRITE("Unknown geometry type - ");  
                WRITE_D(geometry->type);  
                break;  
        }  
    }  
    static void report_brep_topology_tree_geometry(UF_BREP_topology_t *topology)  
    {  
        int  
            ii,  
            nstates;  
        UF_BREP_geometry_t  
            geometry;  
        UF_BREP_orientation_t  
            geom_sense;  
        UF_BREP_state_t  
            *states;  
        UF_UI_open_listing_window();  
        report_brep_topology(topology);  
        if (topology->type > UF_BREP_SHELL_TOPO)  
        {  
            if (!UF_CALL(UF_BREP_ask_geometry(topology, NULL, NULL, &geometry,  
                &geom_sense, &nstates, &states)))  
            {  
                report_brep_states(nstates, states);  
                report_brep_geometry(&geometry);  
                UF_CALL(UF_BREP_free_geometry_data(&geometry, NULL));  
            }  
        }  
        UF_UI_write_listing_window("\n");  
        for (ii = 0; ii < topology->num_children; ii++)  
            report_brep_topology_tree_geometry(topology->children[ii].child);  
    }  
    static void do_it(void)  
    {  
        int  
            flatten[1] = { UF_BREP_FLATTEN_OPTION },  
            nstates;  
        tag_t  
            object = NULL_TAG;  
        UF_BREP_topology_p_t  
            topo;  
        UF_BREP_state_p_t  
            states;  
        UF_BREP_options_t  
            options = { 1, NULL, NULL };  
        options.options = flatten;  
        while ((object = ask_next_visible_face(object)) != NULL_TAG)  
        {  
            UF_CALL(UF_BREP_ask_topology(object,&options,&topo,&nstates,&states));  
            report_brep_states(nstates, states);  
            report_brep_topology_tree_geometry(topo);  
            UF_CALL(UF_BREP_release_topology(topo, NULL));  
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

> 这是NX的二次开发代码，主要功能是遍历所有可见的面，获取每个面的拓扑和几何信息，并将其打印到日志窗口。
>
> 代码的主要逻辑包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个函数ask_next_visible_face，用于遍历所有可见的面。
> 3. 定义了一系列write_xxx_to_listing_window函数，用于将各种类型的数据格式化输出到日志窗口。
> 4. 定义了report_brep_topology函数，用于输出面的拓扑信息。
> 5. 定义了report_brep_states函数，用于输出面的状态信息。
> 6. 定义了report_brep_geometry函数，用于输出面的几何信息。
> 7. 定义了report_brep_topology_tree_geometry函数，递归调用上述函数，遍历输出面的所有拓扑和几何信息。
> 8. 主函数do_it中，遍历所有可见面，获取每个面的拓扑树，并调用report_brep_topology_tree_geometry函数输出信息。
> 9. ufusr是NX二次开发的入口函数，调用do_it函数执行上述操作。
> 10. ufusr_ask_unload用于卸载用户函数。
>
> 总的来说，这段代码实现了NX中面的拓扑和几何信息的遍历和输出功能，通过递归遍历面的拓扑树，并调用不同的函数输出面的拓扑、状态、几何信息到日志窗口。
>
