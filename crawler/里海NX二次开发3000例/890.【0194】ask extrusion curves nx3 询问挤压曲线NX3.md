### 【0194】ask extrusion curves nx3 询问挤压曲线NX3

#### 代码

```cpp
    /*HEAD ASK_EXTRUSION_CURVES_NX3 CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_modl_mswp.h>  
    #include <uf_so.h>  
    #include <uf_disp.h>  
    #include <uf_sc.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中新增的功能。 */  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
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
    static void report_curve_struct_data(UF_CURVE_struct_p_t curve_struct)  
    {  
        int  
            type;  
        double  
            *curve_data;  
        UF_CALL(UF_CURVE_ask_curve_struct_data(curve_struct, &type, &curve_data));  
        switch (type)  
        {  
            case UF_point_type:  
                WRITE("Point:\n ");  
                WRITE(" Absolute coordinates:");  
                WRITE3F(curve_data);  
                break;  
            case UF_line_type:  
                WRITE("Line:\n");  
                WRITE(" Start coordinates:  ");  
                WRITE3F(&curve_data[3]);  
                WRITE(" End coordinates:  ");  
                WRITE3F(&curve_data[6]);  
                break;  
            case UF_circle_type:  
                WRITE("Arc:\n");  
                WRITE(" Start angle (radians):  ");  
                WRITE_F(curve_data[3]);  
                WRITE(" End angle (radians):  ");  
                WRITE_F(curve_data[4]);  
                WRITE(" Center coordinates:  ");  
                WRITE3F(&curve_data[6]);  
                WRITE(" X axis of construction plane:  ");  
                WRITE3F(&curve_data[9]);  
                WRITE(" Y axis of construction plane:  ");  
                WRITE3F(&curve_data[12]);  
                break;  
            case UF_conic_type:  
                WRITE("Conic:\n");  
                WRITE(" Center coordinates:  ");  
                WRITE3F(&curve_data[8]);  
                WRITE(" axis1:  ");  
                WRITE3F(&curve_data[11]);  
                WRITE(" axis2:  ");  
                WRITE3F(&curve_data[14]);  
                break;  
            case UF_spline_type:  
                WRITE("Spline:\n");  
                WRITE(" Number of poles:  ");  
                WRITE_F(curve_data[3]);  
                WRITE(" Order:  ");  
                WRITE_F(curve_data[4]);  
                WRITE(" Knots:  ");  
                WRITEnF((int)(curve_data[3]+curve_data[4]),&curve_data[5]);  
                WRITE(" Poles:  ");  
                WRITEnF4((&curve_data[5])+(int)(curve_data[3]+curve_data[4]),  
                    curve_data[3]);  
                break;  
            default:  
                WRITE("Unknown curve type ");  
                WRITE_D(type);  
                break;  
        }  
        UF_free(curve_data);  
    }  
    static void report_curve_data(tag_t curve)  
    {  
        UF_CURVE_struct_p_t  
            curve_struct;  
        if (UF_CALL(UF_CURVE_ask_curve_struct(curve, &curve_struct))) return;  
        report_curve_struct_data(curve_struct);  
        UF_CALL(UF_CURVE_free_curve_struct(curve_struct));  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_FNT(X) report_feature_name_and_type(#X, X)  
    static void report_feature_name_and_type(char *which, tag_t feature)  
    {  
        char  
            *name,  
            *type,  
            msg[MAX_LINE_LENGTH+1];  
        if (!UF_CALL(UF_MODL_ask_feat_name(feature, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feature, &type)))  
        {  
            sprintf(msg, "%s - tag_t %d - %s (%s)\n", which, feature, name, type);  
            WRITE(msg);  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void ask_object_type_descriptor(int type, int subtype, char *descriptor)  
    {  
        int  
            ii,  
            a_type;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            *ptr,  
            uoth_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        switch (type)  
        {  
            case UF_circle_type: /* non-std format in the header file 译:non-std format in the header file 的翻译为：头文件中的非标准格式 */  
                strcpy(descriptor, "arc");  
                return;  
            case UF_OBJ_EXPR_TYPE:  
                strcpy(descriptor, "expression");  
                return;  
            case UF_OBJ_PART_TYPE:  
                strcpy(descriptor, "part");  
                return;  
            default:  
                break;  
        }  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf_object_types.h", uoth_file));  
        if ((in_file = fopen(uoth_file, "r")) == NULL)  
        {  
            strcpy(descriptor, "(uf_object_types.h not found)");  
            return;  
        }  
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:如果找不到，就回答“找不到”。 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    break;  
                }  
            }  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if (ptr = strstr(define, "_type")) break;  
                if (a_type == subtype)  
                {  
                    if (ptr = strstr(define, "_subtype")) *ptr = '\0';  
                    if (strstr(define, descriptor) == define)  
                        strcpy(descriptor, define);  
                    else  
                    {  
                        strcat(descriptor, "/");  
                        strcat(descriptor, define);  
                    }  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
        while (ptr = strchr(descriptor, '_')) *ptr = ' ';  
    }  
    #define RT(X) report_object_type_and_subtype(#X, X)  
    static void report_object_type_and_subtype(char *name, tag_t object)  
    {  
        int  
            subtype,  
            type;  
        char  
            descriptor[MAX_LINE_SIZE+1],  
            msg[UF_UI_MAX_STRING_LEN];  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            ask_object_type_descriptor(type, subtype, descriptor);  
            sprintf(msg, "%s = eid %d:  %s (%d/%d)\n", name, object, descriptor,  
                type, subtype);  
            UF_CALL(UF_UI_open_listing_window());  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    static void report_sc_input_data(int n_rules, int *rule_types,  
        UF_SC_input_data_t *rules)  
    {  
        int  
            ii, jj;  
        WRITE_D(n_rules);  
        for (ii = 0; ii < n_rules; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(rule_types[ii]);  
            switch(rule_types[ii])  
            {  
                case UF_SC_EDGE_DUMB_EDGES:  
                    WRITE_D(UF_SC_EDGE_DUMB_EDGES);  
                    WRITE_D(rules[ii].edge_dumb_input.num_edges);  
                    for (jj = 0; jj < rules[ii].edge_dumb_input.num_edges; jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].edge_dumb_input.edges[jj]);  
                    }  
                    break;  
                case UF_SC_EDGE_CHAIN_EDGES:  
                    WRITE_D(UF_SC_EDGE_CHAIN_EDGES);  
                    RT(rules[ii].edge_chain_input.start_edge);  
                    RT(rules[ii].edge_chain_input.end_edge);  
                    WRITE_L(rules[ii].edge_chain_input.from_start);  
                    break;  
                case UF_SC_EDGE_TANGENT_EDGES:  
                    WRITE_D(UF_SC_EDGE_TANGENT_EDGES);  
                    RT(rules[ii].edge_tangent_input.start_edge);  
                    RT(rules[ii].edge_tangent_input.end_edge);  
                    WRITE_L(rules[ii].edge_tangent_input.from_start);  
                    WRITE_F(rules[ii].edge_tangent_input.angle_tol);  
                    WRITE_L(rules[ii].edge_tangent_input.same_convexity);  
                    break;  
                case UF_SC_EDGE_FACE_EDGES:  
                    WRITE_D(UF_SC_EDGE_FACE_EDGES);  
                    WRITE_D(rules[ii].edge_face_input.num_faces);  
                    for (jj = 0; jj < rules[ii].edge_face_input.num_faces; jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].edge_face_input.faces[jj]);  
                    }  
                    break;  
                case UF_SC_EDGE_BODY_EDGES:  
                    WRITE_D(UF_SC_EDGE_BODY_EDGES);  
                    RT(rules[ii].edge_body_input.body);  
                    break;  
                case UF_SC_EDGE_FEATURE_EDGES:  
                    WRITE_D(UF_SC_EDGE_FEATURE_EDGES);  
                    WRITE_D(rules[ii].edge_feature_input.num_features);  
                    for (jj = 0; jj < rules[ii].edge_feature_input.num_features;  
                        jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].edge_feature_input.features[jj]);  
                    }  
                    break;  
                case UF_SC_EDGE_SHBNDARY_EDGES:  
                    WRITE_D(UF_SC_EDGE_SHBNDARY_EDGES);  
                    RT(rules[ii].edge_shbndary_input.sheet);  
                    break;  
                case UF_SC_EDGE_BOUNDARY_EDGES:  
                    WRITE_D(UF_SC_EDGE_BOUNDARY_EDGES);  
                    WRITE_D(rules[ii].edge_boundary_input.num_faces_n_features);  
                    for (jj = 0;  
                        jj < rules[ii].edge_boundary_input.num_faces_n_features;  
                        jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].edge_boundary_input.faces_n_features[jj]);  
                    }  
                    break;  
                case UF_SC_EDGE_VERTEX_EDGES:  
                    WRITE_D(UF_SC_EDGE_VERTEX_EDGES);  
                    RT(rules[ii].edge_vertex_input.start_edge);  
                    WRITE_L(rules[ii].edge_vertex_input.from_start);  
                    break;  
                case UF_SC_EDGE_VERTEX_TANGENT_EDGES:  
                    WRITE_D(UF_SC_EDGE_VERTEX_TANGENT_EDGES);  
                    RT(rules[ii].edge_vertex_tangent_input.start_edge);  
                    WRITE_L(rules[ii].edge_vertex_tangent_input.from_start);  
                    WRITE_F(rules[ii].edge_vertex_tangent_input.angle_tol);  
                    WRITE_L(rules[ii].edge_vertex_tangent_input.same_convexity);  
                    break;  
                case UF_SC_EDGE_INTERSECT_EDGES:  
                    WRITE_D(UF_SC_EDGE_INTERSECT_EDGES);  
                    WRITE_D(rules[ii].edge_intersect_input.  
                        num_faces_n_features1);  
                    for (jj = 0;  
                        jj < rules[ii].edge_intersect_input.  
                        num_faces_n_features1; jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].edge_intersect_input.faces_n_features1[jj]);  
                    }  
                    WRITE_D(rules[ii].edge_intersect_input.  
                        num_faces_n_features2);  
                    for (jj = 0;  
                        jj < rules[ii].edge_intersect_input.  
                            num_faces_n_features2; jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].edge_intersect_input.faces_n_features2[jj]);  
                    }  
                    break;  
                case UF_SC_EDGE_MSEEDTAN_EDGES:  
                    WRITE_D(UF_SC_EDGE_MSEEDTAN_EDGES);  
                    WRITE_D(rules[ii].edge_mseedtan_input.num_seed_edges);  
                    for (jj = 0; jj < rules[ii].edge_mseedtan_input.num_seed_edges;  
                        jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].edge_mseedtan_input.seed_edges[jj]);  
                    }  
                    WRITE_F(rules[ii].edge_mseedtan_input.angle_tol);  
                    WRITE_L(rules[ii].edge_mseedtan_input.same_convexity);  
                    break;  
                case UF_SC_FACE_DUMB_FACES:  
                    WRITE_D(UF_SC_FACE_DUMB_FACES);  
                    WRITE_D(rules[ii].face_dumb_input.num_faces);  
                    for (jj = 0; jj < rules[ii].face_dumb_input.num_faces; jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].face_dumb_input.faces[jj]);  
                    }  
                    break;  
                case UF_SC_FACE_REGION_FACES:  
                    WRITE_D(UF_SC_FACE_REGION_FACES);  
                    RT(rules[ii].face_region_input.seed_face);  
                    WRITE_D(rules[ii].face_region_input.num_bndary_faces);  
                    for (jj = 0;  
                        jj < rules[ii].face_region_input.num_bndary_faces;  
                        jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].face_region_input.bndary_faces[jj]);  
                    }  
                    report_sc_input_data(  
                        rules[ii].face_region_input.n_bndary_face_data,  
                        rules[ii].face_region_input.bndary_face_types,  
                        rules[ii].face_region_input.bndary_face_data);  
                    break;  
                case UF_SC_FACE_TANGENT_FACES:  
                    WRITE_D(UF_SC_FACE_TANGENT_FACES);  
                    RT(rules[ii].face_tangent_input.seed_face);  
                    WRITE_D(rules[ii].face_tangent_input.num_bndary_faces);  
                    for (jj = 0;  
                        jj < rules[ii].face_tangent_input.num_bndary_faces;  
                        jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].face_tangent_input.bndary_faces[jj]);  
                    }  
                    WRITE_F(rules[ii].face_tangent_input.angle_tol);  
                    report_sc_input_data(  
                        rules[ii].face_tangent_input.n_bndary_face_data,  
                        rules[ii].face_tangent_input.bndary_face_types,  
                        rules[ii].face_tangent_input.bndary_face_data);  
                    break;  
                case UF_SC_FACE_FEATURE_FACES:  
                    WRITE_D(UF_SC_FACE_FEATURE_FACES);  
                    WRITE_D(rules[ii].face_feature_input.num_features);  
                    for (jj = 0; jj < rules[ii].face_feature_input.num_features;  
                        jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].face_feature_input.features[jj]);  
                    }  
                    break;  
                case UF_SC_FACE_BODY_FACES:  
                    WRITE_D(UF_SC_FACE_BODY_FACES);  
                    RT(rules[ii].face_body_input.body);  
                    break;  
                case UF_SC_FACE_ADJACENT_FACES:  
                    WRITE_D(UF_SC_FACE_ADJACENT_FACES);  
                    RT(rules[ii].face_adjacent_input.seed);  
                    break;  
                case UF_SC_CURVE_DUMB_CURVES:  
                    WRITE_D(UF_SC_CURVE_DUMB_CURVES);  
                    WRITE_D(rules[ii].curve_dumb_input.num_curves);  
                    for (jj = 0; jj < rules[ii].curve_dumb_input.num_curves;  
                        jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].curve_dumb_input.curves[jj]);  
                    }  
                    break;  
                case UF_SC_CURVE_FEATURE_CURVES:  
                    WRITE_D(UF_SC_CURVE_FEATURE_CURVES);  
                    WRITE_D(rules[ii].curve_feature_input.num_features);  
                    for (jj = 0; jj < rules[ii].curve_feature_input.num_features;  
                        jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].curve_feature_input.features[jj]);  
                    }  
                    break;  
                case UF_SC_CURVE_FEAT_CHAIN_CURVES:  
                    WRITE_D(UF_SC_CURVE_FEAT_CHAIN_CURVES);  
                    WRITE_D(rules[ii].curve_feat_chain_input.num_features);  
                    for (jj = 0;  
                        jj < rules[ii].curve_feat_chain_input.num_features;  
                        jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].curve_feat_chain_input.features[jj]);  
                    }  
                    RT(rules[ii].curve_feat_chain_input.seed_curve);  
                    RT(rules[ii].curve_feat_chain_input.end_curve);  
                    WRITE_L(rules[ii].curve_feat_chain_input.from_seed_start);  
                    WRITE_F(rules[ii].curve_feat_chain_input.gap_tol);  
                    break;  
                case UF_SC_CURVE_FEAT_TANGENT_CURVES:  
                    WRITE_D(UF_SC_CURVE_FEAT_TANGENT_CURVES);  
                    WRITE_D(rules[ii].curve_feat_tangent_input.num_features);  
                    for (jj = 0;  
                        jj < rules[ii].curve_feat_tangent_input.num_features;  
                        jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].curve_feat_tangent_input.features[jj]);  
                    }  
                    RT(rules[ii].curve_feat_tangent_input.seed_curve);  
                    RT(rules[ii].curve_feat_tangent_input.end_curve);  
                    WRITE_F(rules[ii].curve_feat_tangent_input.gap_angle_tols[0]);  
                    WRITE_F(rules[ii].curve_feat_tangent_input.gap_angle_tols[1]);  
                    break;  
                case UF_SC_BODY_DUMB_BODIES:  
                    WRITE_D(UF_SC_BODY_DUMB_BODIES);  
                    WRITE_D(rules[ii].body_dumb_input.num_bodies);  
                    for (jj = 0;  
                        jj < rules[ii].body_dumb_input.num_bodies;  
                        jj++)  
                    {  
                        WRITE_D(jj);  
                        RT(rules[ii].body_dumb_input.bodies[jj]);  
                    }  
                    break;  
                default:  
                    break;  
            }  
        }  
    }  
    static void report_section_data(tag_t section)  
    {  
        int  
            ii,  
            jj,  
            n_rules;  
        tag_t  
            starting_object;  
        double  
            direction[3],  
            starting_point[3];  
        UF_SC_section_data_t  
            *sec_rules;  
        if (!UF_CALL(UF_MODL_ask_section(section, &n_rules, &sec_rules,  
            &starting_object, starting_point, direction)))  
        {  
            for (ii = 0; ii < n_rules; ii++)  
            {  
                WRITE_D(ii);  
                report_sc_input_data(sec_rules[ii].n_rules,  
                    sec_rules[ii].rule_types, sec_rules[ii].rules);  
                WRITE_F(sec_rules[ii].chaining_tolerance);  
                WRITE_F(sec_rules[ii].distance_tolerance);  
                RT(sec_rules[ii].start_connector);  
                RT(sec_rules[ii].end_connector);  
                WRITE3F(sec_rules[ii].help_point);  
                WRITE3F(sec_rules[ii].intersection_point_1);  
                WRITE3F(sec_rules[ii].intersection_point_2);  
                WRITE_D(sec_rules[ii].n_output_objects);  
                for (jj = 0; jj < sec_rules[ii].n_output_objects; jj++)  
                {  
                    RT(sec_rules[ii].output_objects[jj].output_object);  
                    RT(sec_rules[ii].output_objects[jj].start_connected_object);  
                    WRITE3F(sec_rules[ii].output_objects[jj].start_point);  
                    RT(sec_rules[ii].output_objects[jj].end_connected_object);  
                    WRITE3F(sec_rules[ii].output_objects[jj].end_point);  
                }  
                UF_CALL(UF_MODL_free_section_data(&sec_rules[ii]));  
            }  
            if (n_rules > 0) UF_free(sec_rules);  
            RT(starting_object);  
            WRITE3F(starting_point);  
            WRITE3F(direction);  
            display_conehead(starting_point, direction);  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            region_specified,  
            solid_creation;  
        int  
            count,  
            ii,  
            *indices,  
            jj,  
            n_curves;  
        tag_t  
            body = NULL_TAG,  
            *curves,  
            feat,  
            part = UF_PART_ask_display_part();  
        double  
            direction[3],  
            region_point[3];  
        uf_list_p_t  
            list;  
        char  
            *limits[2],  
            *offsets[2],  
            *feat_type,  
            *taper_angle;  
        UF_MODL_SWEEP_TRIM_object_p_t  
            trim_ptr;  
        UF_MODL_mswp_extrude_t  
            extrude;  
        UF_CURVE_struct_p_t  
            *curve_structs;  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            UF_CALL( UF_MODL_ask_body_feats( body, &list));  
            UF_CALL( UF_MODL_ask_list_count( list, &count));  
            for (ii = 0; ii < count; ii++)  
            {  
                UF_CALL( UF_MODL_ask_list_item( list, ii, &feat));  
                WRITE_FNT(feat);  
                UF_CALL( UF_MODL_ask_feat_type( feat, &feat_type));  
                if (!strcmp( feat_type, "SWP104") &&  
                    !UF_CALL( UF_MODL_ask_extrusion( feat, &n_curves, &curves,  
                        &trim_ptr, &taper_angle, limits, offsets, region_point,  
                        &region_specified, &solid_creation, direction)))  
                {  
                    for (jj = 0; jj < n_curves; jj++)  
                        report_curve_data(curves[jj]);  
                    UF_free(curves);  
                    if (trim_ptr) UF_free(trim_ptr);  
                    UF_free(taper_angle);  
                    for (jj = 0; jj < 2; jj++) UF_free(limits[jj]);  
                    for (jj = 0; jj < 2; jj++) UF_free(offsets[jj]);  
                }  
                if (!strcmp( feat_type, "EXTRUDE") &&  
                    !UF_CALL( UF_MODL_mswp_ask_extrude( feat, &extrude)))  
                {  
                    report_section_data(extrude.section);  
                    UF_CALL(UF_MODL_ask_output_curves_from_section(  
                        extrude.section, &n_curves, &indices, &curve_structs));  
                    for (jj = 0; jj < n_curves; jj++)  
                    {  
                        WRITE_D(jj);  
                        WRITE_D(indices[jj]);  
                        report_curve_struct_data(curve_structs[jj]);  
                    }  
                    if (n_curves > 0)  
                    {  
                        UF_free(indices);  
                        UF_free(curve_structs);  
                    }  
                }  
                UF_free( feat_type);  
            }  
            UF_CALL(UF_MODL_delete_list(&list));  
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

```

#### 代码解析

> 这段NX二次开发代码主要用于查询和分析NX模型中的挤出特征。以下是代码的主要功能和流程：
>
> 1. 初始化：使用UF_initialize()进行初始化。
> 2. 遍历模型中的所有零件：通过循环调用ask_next_body()函数，遍历每个零件。
> 3. 查询每个零件中的特征：使用UF_MODL_ask_body_feats()函数查询每个零件中的特征列表，并获取特征数量。
> 4. 遍历每个特征：对于每个特征，使用UF_MODL_ask_list_item()函数获取特征信息，并使用UF_MODL_ask_feat_type()函数获取特征类型。
> 5. 查询挤出特征：如果特征类型为"SWP104"，则使用UF_MODL_ask_extrusion()函数查询挤出特征信息，包括轮廓曲线、修剪对象等，然后调用report_curve_data()函数报告曲线信息。
> 6. 查询MSWP挤出特征：如果特征类型为"EXTRUDE"，则使用UF_MODL_mswp_ask_extrude()函数查询MSWP挤出特征信息，然后调用report_section_data()函数报告截面信息。
> 7. 查询输出曲线：使用UF_MODL_ask_output_curves_from_section()函数查询挤出特征输出的曲线，然后调用report_curve_struct_data()函数报告曲线结构信息。
> 8. 清理内存：对于使用完的内存，调用相应的free函数进行清理。
> 9. 终止：使用UF_terminate()终止。
> 10. 卸载：提供ufusr_ask_unload()函数，用于立即卸载用户函数。
>
> 总体来说，这段代码实现了查询和分析NX模型中挤出特征的功能，包括轮廓曲线、修剪对象、截面信息等。
>
