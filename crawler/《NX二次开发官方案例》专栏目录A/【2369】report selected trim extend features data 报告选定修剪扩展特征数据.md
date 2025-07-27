### 【2369】report selected trim extend features data 报告选定修剪扩展特征数据

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_TRIM_EXTEND_FEATURES_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl_trex.h>  
    #include <uf_sc.h>  
    #include <uf_obj.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %f\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        ECHO(msg);  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        ECHO(msg);  
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
            case UF_circle_type: /* non-std format in the header file 里海译:在头文件中使用非标准格式 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话，请只回答翻译，不要添加无关内容。 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    if (strcmp(define, "gfem_load")) break;  
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
    static void report_smart_wireframe_container(tag_t collector)  
    {  
        int  
            ii,  
            n_rules,  
            n_wireframe_eids,  
            *rule_types;  
        tag_t  
            *wireframe_eids;  
        UF_SC_input_data_t  
            *rules;  
        if (!UF_CALL(UF_MODL_ask_smart_wireframe_container(collector, &n_rules,  
            &rule_types, &rules, &n_wireframe_eids, &wireframe_eids)))  
        {  
            report_sc_input_data(n_rules, rule_types, rules);  
            for (ii = 0; ii < n_rules; ii++)  
                UF_CALL(UF_MODL_free_sc_input_data(rule_types[ii], &(rules[ii])));  
            if (n_rules > 0)  
            {  
                UF_free(rule_types);  
                UF_free(rules);  
            }  
            WRITE_D(n_wireframe_eids);  
            for (ii = 0; ii < n_wireframe_eids; ii++)  
                RT(wireframe_eids[ii]);  
            if (n_wireframe_eids > 0) UF_free(wireframe_eids);  
        }  
    }  
    static void report_trex_feature_data(UF_MODL_TREX_appl_data_t *feature_data)  
    {  
        int  
            ii;  
        WRITE_D(feature_data->num_targets);  
        for (ii = 0; ii < feature_data->num_targets; ii++)  
        {  
            report_smart_wireframe_container(feature_data->targets[ii]->collector);  
            WRITE_F(feature_data->targets[ii]->extend_val);  
            WRITE_F(feature_data->targets[ii]->offset_val);  
            WRITE_D(feature_data->targets[ii]->reversed);  
        }  
        WRITE_D(feature_data->num_tools);  
        for (ii = 0; ii < feature_data->num_tools; ii++)  
        {  
            report_smart_wireframe_container(feature_data->tools[ii]->collector);  
            WRITE_F(feature_data->tools[ii]->extend_val);  
            WRITE_F(feature_data->tools[ii]->offset_val);  
            WRITE_D(feature_data->tools[ii]->reversed);  
        }  
        switch (feature_data->extend_method)  
        {  
            case UF_MODL_TREX_C2:  
                ECHO("feature_data->extend_method = UF_MODL_TREX_C2\n");  
                break;  
            case UF_MODL_TREX_LINEAR:  
                ECHO("feature_data->extend_method = UF_MODL_TREX_LINEAR\n");  
                break;  
            case UF_MODL_TREX_REFLECTED:  
                ECHO("feature_data->extend_method = UF_MODL_TREX_REFLECTED\n");  
                break;  
            case UF_MODL_TREX_NATURAL:  
                ECHO("feature_data->extend_method = UF_MODL_TREX_NATURAL\n");  
                break;  
            default:  
                ECHO("Unknown ");  
                WRITE_D(feature_data->extend_method);  
                break;  
        }  
        switch (feature_data->to_option)  
        {  
            case UF_MODL_TREX_DISTANCE:  
                ECHO("feature_data->to_option = UF_MODL_TREX_DISTANCE\n");  
                break;  
            case UF_MODL_TREX_PERCENT:  
                ECHO("feature_data->to_option = UF_MODL_TREX_PERCENT\n");  
                break;  
            case UF_MODL_TREX_SURFACE:  
                ECHO("feature_data->to_option = UF_MODL_TREX_SURFACE\n");  
                break;  
            default:  
                ECHO("Unknown ");  
                WRITE_D(feature_data->to_option);  
                break;  
        }  
        switch (feature_data->region_option)  
        {  
            case UF_MODL_TREX_KEEP:  
                ECHO("feature_data->region_option = UF_MODL_TREX_KEEP\n");  
                break;  
            case UF_MODL_TREX_REMOVE:  
                ECHO("feature_data->region_option = UF_MODL_TREX_REMOVE\n");  
                break;  
            default:  
                ECHO("Unknown ");  
                WRITE_D(feature_data->region_option);  
                break;  
        }  
        WRITE_D(feature_data->make_corner);  
        WRITE_D(feature_data->keep_boundaries);  
        WRITE_D(feature_data->extract_sheets);  
        WRITE_F(feature_data->dist_tol);  
        WRITE_D(feature_data->algorithm_version);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feature;  
        UF_MODL_TREX_appl_data_t  
            feature_data;  
        while ((feature = select_a_feature("Report trex data")) != NULL_TAG)  
            if (!UF_CALL(UF_MODL_TREX_ask_trim_extend(feature, &feature_data)))  
            {  
                report_trex_feature_data(&feature_data);  
                UF_CALL(UF_MODL_TREX_free_trex_data(&feature_data));  
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

> 这段代码是一个NX Open C++应用程序，主要实现了对NX特征进行修剪和延伸操作的查询和报告。
>
> 主要功能包括：
>
> 1. 选择特征：使用UF_UI_select_feature函数让用户选择一个特征，并返回特征标签。
> 2. 查询特征数据：使用UF_MODL_TREX_ask_trim_extend函数查询特征的数据，包括修剪和延伸的目标和工具等。
> 3. 报告特征数据：使用自定义的report_trex_feature_data函数，详细报告特征数据的各个方面，如目标、工具、延伸方法等。
> 4. 释放特征数据：使用UF_MODL_TREX_free_trex_data函数释放特征数据内存。
> 5. 错误报告：使用自定义的report_error函数来报告函数调用错误。
> 6. 对象类型查询：使用自定义的report_object_type_and_subtype函数来查询和报告特征对象的具体类型。
> 7. 智能线框容器查询：使用自定义的report_smart_wireframe_container函数来查询和报告智能线框容器的规则。
> 8. 主函数：主函数do_it实现了选择特征、查询和报告特征数据的循环。
> 9. NX初始化和终止：ufusr函数实现了NX的初始化和终止。
> 10. 卸载请求：ufusr_ask_unload函数实现了卸载请求的处理。
>
> 总体来说，这段代码实现了对NX特征进行修剪和延伸操作的查询和报告功能，提供了良好的错误处理和内存管理，是一段结构清晰、功能完整的NX二次开发代码。
>
