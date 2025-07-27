### 【2306】report sc input data plaintext报告曲面连续性输入数据

#### 代码

```cpp
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

```

#### 代码解析

> 这段代码是用于在NX CAD系统中进行二次开发的一部分，其主要功能是报告形状规则（Shape Constraint）的输入数据。
>
> 代码的主要流程包括：
>
> 1. 输入参数：接收形状规则的数量n_rules和规则类型数组rule_types，以及规则数据结构数组rules。
> 2. 遍历规则：遍历所有规则，根据规则类型，读取并报告相应的规则数据。
> 3. 规则类型处理：针对不同的规则类型，如边的规则、面的规则、曲线的规则、体的规则等，进行不同的处理。
> 4. 规则数据读取：对于每条规则，读取其类型、起始边、结束边、角度公差等参数。
> 5. 递归处理：对于面和体规则，可能包含嵌套的边界面规则，需要递归调用自身函数来处理。
> 6. 数据写入：将规则编号、类型、参数等写入输出。
> 7. 规则类型判断：通过判断规则类型，选择不同的读取和处理方式。
> 8. 规则数据结构：规则数据结构包含规则编号、类型、边、面、角度公差等参数。
> 9. 函数功能：该函数的作用是读取和报告形状规则的数据，以便后续使用这些规则进行几何约束求解。
> 10. NX二次开发：这段代码属于NX的二次开发代码，用于读取和解析形状约束规则，为后续的几何约束求解做准备。
>
