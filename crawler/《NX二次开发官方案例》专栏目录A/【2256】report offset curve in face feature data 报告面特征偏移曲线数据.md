### 【2256】report offset curve in face feature data 报告面特征偏移曲线数据

#### 代码

```cpp
    /*HEAD REPORT_OFFSET_CURVE_IN_FACE_FEATURE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。因此，UF_print_syslog是V18版本新增的功能，仅回答译文，无需过多解释。 */  
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
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
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
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
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
            case UF_circle_type: /* non-std format in the header file 里海译:非标准格式在头文件中 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话，就回答：如果找不到。 */  
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
    static void report_ocf_data(tag_t ocf_feature)  
    {  
        int  
            ii,  
            jj,  
            n_faces,  
            n_rules,  
            *rule_types;  
        tag_t  
            *faces;  
        UF_SC_input_data_t  
            *rules;  
        UF_CURVE_ocf_data_t  
            *offset_data;  
        if (UF_CALL(UF_CURVE_ask_ocf_data(ocf_feature, &offset_data))) return;  
        WRITE_D(offset_data->num_string_data);  
        for (ii = 0; ii < offset_data->num_string_data; ii++)  
        {  
            WRITE_D(ii);  
            RT(offset_data->string_data[ii].string_tag);  
            report_section_data(offset_data->string_data[ii].string_tag);  
            WRITE_D(offset_data->string_data[ii].offset_direction);  
            WRITE_D(offset_data->string_data[ii].num_offsets);  
            for (jj = 0; jj < offset_data->string_data[ii].num_offsets; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_S(offset_data->string_data[ii].offset_distances[jj].string);  
            }  
        }  
        RT(offset_data->face_data->face_tag);  
        if (!UF_CALL(UF_MODL_ask_smart_face_container(  
            offset_data->face_data->face_tag, &n_rules, &rule_types, &rules,  
            &n_faces, &faces)))  
        {  
            report_sc_input_data(n_rules, rule_types, rules);  
            for (ii = 0; ii < n_rules; ii++)  
                UF_CALL(UF_MODL_free_sc_input_data(rule_types[ii], &rules[ii]));  
            if (n_rules > 0)  
            {  
                UF_free(rule_types);  
                UF_free(rules);  
            }  
            WRITE_D(n_faces);  
            for (ii = 0; ii < n_faces; ii++)  
            {  
                WRITE_D(ii);  
                RT(faces[ii]);  
            }  
            if (n_faces > 0) UF_free(faces);  
        }  
        WRITE("cross_bounday_mode is reversed - See PR 5226467\n");  
        WRITE_D(offset_data->cross_boundary_mode);  
        WRITE("for input to UF_CURVE_create_ocf_feature use ");  
        WRITE_D(1 - offset_data->cross_boundary_mode);  
        WRITE_D(offset_data->offset_method);  
        WRITE_D(offset_data->trim_method);  
        WRITE_D(offset_data->span_method);  
        WRITE_F(offset_data->dist_tol);  
        WRITE_F(offset_data->ang_tol);  
        WRITE_F(offset_data->string_tol);  
        UF_CALL(UF_CURVE_free_ocf_data(&offset_data));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feature;  
        while ((feature = select_a_feature("Select OFFSET_IN_FACE feature"))  
            != NULL_TAG)  
        {  
            report_ocf_data(feature);  
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

> 根据代码内容，这是一个用于报告NX中OFFSET_IN_FACE特征的二次开发代码。
>
> 主要功能包括：
>
> 1. 错误报告：定义了错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 信息输出：定义了多个宏，用于输出不同类型的信息到列表窗口，如整数、字符串、浮点数等。
> 3. 特征选择：定义了函数select_a_feature，用于让用户选择一个OFFSET_IN_FACE特征。
> 4. 类型描述：定义了函数ask_object_type_descriptor，用于获取NX对象类型的描述字符串。
> 5. 类型报告：定义了宏RT，用于报告对象类型和子类型。
> 6. 曲线数据报告：定义了函数report_sc_input_data，用于报告曲线数据。
> 7. 截面数据报告：定义了函数report_section_data，用于报告截面数据。
> 8. 偏移特征报告：定义了函数report_ocf_data，用于报告OFFSET_IN_FACE特征的偏移数据。
> 9. 主要流程：定义了函数do_it，用于循环选择特征并报告数据。
> 10. 初始化和终止：定义了ufusr函数，用于初始化和调用主要流程，以及卸载函数。
>
> 总体来说，这段代码的主要作用是让用户选择OFFSET_IN_FACE特征，并报告该特征的详细信息，包括截面数据、偏移数据等。
>
