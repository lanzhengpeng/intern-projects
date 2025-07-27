### 【0534】copy brep body 复制BREP体

#### 代码

```cpp
    /*HEAD COPY_BREP_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_brep.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，V18新增了UF_print_syslog函数。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
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
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
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
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(X,n)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
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
    static void build_copy_partname(tag_t part, char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, "-new");  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
    }  
    static void open_copy_part(tag_t part, tag_t *copy_part)  
    {  
        int  
            units;  
        char  
            copy_fspec[MAX_FSPEC_SIZE+1];  
        build_copy_partname(part, copy_fspec);  
        UF_CALL(UF_PART_ask_units(part, &units));  
        UF_CALL(UF_PART_new(copy_fspec, units, copy_part));  
    }  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
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
    static void copy_topology_tree(  
        UF_BREP_topology_t  *OldParentTopology,  
        UF_BREP_topology_t  *OldChildTopology,  
        int                  ChildNdx,  
        UF_BREP_topology_t  *NewParentTopology,  
        UF_BREP_topology_t **NewChildTopology  
    )  
    {  
        int  
            err,  
            ii,  
            StateCount;  
        UF_BREP_geometry_t  
            Geometry;  
        UF_BREP_mapping_t  
            *Mapping = NULL;  
        UF_BREP_options_t  
            *AttachOptions = NULL;  
        UF_BREP_orientation_t  
            GeomSense;  
        UF_BREP_state_t  
            *States;  
        UF_BREP_topology_t  
            *CreatedChildTopology;  
        if (OldChildTopology->u.user_ptr != NULL)  
        {  
            NewParentTopology->children[ChildNdx].child =  
                OldChildTopology->u.user_ptr;  
            NewParentTopology->children[ChildNdx].orientation =  
                OldParentTopology->children[ChildNdx].orientation;  
            return;  
        }  
        *NewChildTopology = (UF_BREP_topology_t *)  
            UF_allocate_memory(sizeof(UF_BREP_topology_t), &err);  
        (*NewChildTopology)->type = OldChildTopology->type;  
        (*NewChildTopology)->tag = 0;  
        (*NewChildTopology)->u = OldChildTopology->u;  
        (*NewChildTopology)->extension = NULL;  
        (*NewChildTopology)->num_children = OldChildTopology->num_children;  
        (*NewChildTopology)->children = NULL;  
        if (OldChildTopology->num_children > 0)  
            (*NewChildTopology)->children = (UF_BREP_oriented_child_t *)  
                UF_allocate_memory ((*NewChildTopology)->num_children *  
                sizeof (UF_BREP_oriented_child_t), &err);  
        if (OldChildTopology->type == UF_BREP_FACE_TOPO ||  
            OldChildTopology->type == UF_BREP_EDGE_TOPO ||  
            OldChildTopology->type == UF_BREP_VERTEX_TOPO)  
        {  
            UF_CALL(UF_BREP_ask_geometry (OldChildTopology, NULL, Mapping,  
                &Geometry, &GeomSense, &StateCount, &States));  
            report_brep_states(StateCount, States);  
            UF_CALL(UF_BREP_attach_geometry (*NewChildTopology, &Geometry,  
                GeomSense, AttachOptions, &StateCount, &States));  
            report_brep_states(StateCount, States);  
            UF_CALL(UF_BREP_free_geometry_data (&Geometry, (void *) NULL));  
        }  
        OldChildTopology->u.user_ptr = (*NewChildTopology);  
        if (OldParentTopology != NULL)  
        {  
            NewParentTopology->children[ChildNdx].child = (*NewChildTopology);  
            NewParentTopology->children[ChildNdx].orientation =  
                OldParentTopology->children[ChildNdx].orientation;  
        }  
        for (ii=0;  ii < OldChildTopology->num_children;  ii++)  
            copy_topology_tree (OldChildTopology,  
                OldChildTopology->children[ii].child, ii, *NewChildTopology,  
                &CreatedChildTopology);  
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
                WRITEnD( geometry->geom.foreign.num_ints,  
                    geometry->geom.foreign.int_array);  
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
            count,  
            body_type,  
            err,  
            ii,  
            nstates;  
        tag_t  
            body = NULL_TAG,  
            body_copy,  
            cp_part,  
            part = UF_PART_ask_display_part(),  
            sew,  
            *tools;  
        double  
            toler;  
        uf_list_p_t  
            disjoint_list;  
        UF_BREP_topology_p_t  
            make_topo,  
            new_topo,  
            topo;  
        UF_BREP_state_p_t  
            states;  
        open_copy_part(part, &cp_part);  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            WRITE("Original body topology tree:\n");  
            UF_CALL(UF_BREP_ask_topology(body, 0, &topo, &nstates, &states));  
            report_brep_states(nstates, states);  
            report_brep_topology_tree_geometry(topo);  
            WRITE("\nCopying topology tree...\n");  
            copy_topology_tree(NULL, topo, 0, NULL, &new_topo);  
            UF_CALL(UF_BREP_validate_topology(new_topo, &nstates, &states));  
            report_brep_states(nstates, states);  
            if (!UF_CALL(UF_BREP_make_body(new_topo, &body_copy,&nstates,&states)))  
                UF_CALL(UF_DISP_add_item_to_display(body_copy));  
            report_brep_states(nstates, states);  
        /*  The following is a work around for PR 1063454 译:The following is a work around for PR 1063454. */  
            UF_CALL(UF_MODL_ask_body_type(body, &body_type));  
            if (body_type == UF_MODL_SHEET_BODY)  
            {  
                make_topo = new_topo;  
                while (make_topo->children[0].child->type != UF_BREP_FACE_TOPO)  
                    make_topo = make_topo->children[0].child;  
                if (make_topo->num_children > 1)  
                {  
                    tools = (tag_t *)UF_allocate_memory(  
                        (make_topo->num_children - 1) * sizeof(tag_t), &err);  
                    for (ii = 1; ii < make_topo->num_children; ii++)  
                    {  
                        if (!UF_CALL(UF_BREP_make_body(  
                            make_topo->children[ii].child, &tools[ii-1],  
                            &nstates, &states)))  
                            UF_CALL(UF_DISP_add_item_to_display(tools[ii-1]));  
                        report_brep_states(nstates, states);  
                    }  
                    UF_MODL_ask_distance_tolerance(&toler);  
                    UF_CALL(UF_MODL_create_sew(0, 1, &body_copy,  
                        make_topo->num_children - 1, tools, toler, 0,  
                        &disjoint_list, &sew));  
                    UF_CALL(UF_MODL_ask_list_count( disjoint_list, &count));  
                    if (count > 0) UF_MODL_delete_list(&disjoint_list);  
                    UF_free(tools);  
                }  
            }  
        /*  End work around for PR 1063454 译:翻译结果如下：

绕过 PR 1063454 的临时解决办法 */  
            UF_CALL(UF_BREP_release_topology(topo, NULL));  
            UF_CALL(UF_BREP_release_topology(new_topo, NULL));  
            WRITE("\nCopied body topology tree:\n");  
            UF_CALL(UF_BREP_ask_topology(body_copy, 0, &topo, &nstates, &states));  
            report_brep_states(nstates, states);  
            report_brep_topology_tree_geometry(topo);  
            UF_CALL(UF_BREP_release_topology(topo, NULL));  
        }  
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
        return (UF_UNLOAD_IMMEDIATELY);  
    }  
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                if (UF_CALL(UF_PART_open(part_name, &part, &status))  
                    || status.n_parts > 0) check_load_status(&status);  
                else  
                {  
                    do_it();  
                    UF_CALL(UF_PART_save());  
                    UF_PART_close_all();  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，其主要功能是复制一个实体模型的BREP拓扑结构树，并检查复制后拓扑结构的几何状态。
>
> 主要步骤包括：
>
> 1. 定义一些辅助函数，用于报告错误、写入日志等。
> 2. 定义一个递归函数copy_topology_tree，用于复制BREP拓扑结构树。它递归地复制每个拓扑节点，并为其分配新的几何数据。
> 3. 定义一个函数report_brep_topology_tree_geometry，用于遍历BREP拓扑树并报告每个节点的几何信息。
> 4. 在do_it函数中，首先打开原始模型，然后遍历每个实体，调用UF_BREP_ask_topology查询其BREP拓扑结构树。
> 5. 调用copy_topology_tree复制拓扑结构树，然后调用UF_BREP_make_body创建新的实体。
> 6. 如果是薄片体，还需要处理薄片体的面片，避免出现错误。
> 7. 最后，查询复制后实体的BREP拓扑结构树，确认复制成功。
> 8. 在main函数中，处理命令行传入的部件文件路径，打开每个部件，执行do_it函数，然后保存并关闭部件。
>
> 通过这个示例，我们可以学习到如何使用NX的BREP API来操作实体模型的几何数据，以及如何处理一些特殊情况，如薄片体。
>
