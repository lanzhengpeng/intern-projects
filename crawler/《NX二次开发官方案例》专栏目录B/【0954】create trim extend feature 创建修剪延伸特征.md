### 【0954】create trim extend feature 创建修剪延伸特征

#### 代码

```cpp
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_sc.h>
#include <uf_modl.h>
#include <uf_modl_trex.h>
static void ECHO(char *format, ...)
{
    char msg[UF_UI_MAX_STRING_LEN+1];
    va_list args;
    va_start(args, format);
    vsprintf(msg, format, args);
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
/* qq3123197280 */
static int mask_for_edges(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static int select_edges(char *prompt, tag_t **edges)
{
    int
        cnt,
        ii,
        resp;
    UF_CALL(UF_UI_select_with_class_dialog("Select edges", prompt,
        UF_UI_SEL_SCOPE_WORK_PART, mask_for_edges, NULL, &resp, &cnt, edges));
    for (ii = 0; ii < cnt; ii++)
        UF_CALL(UF_DISP_set_highlight((*edges)[ii], FALSE));
    return cnt;
}
static tag_t create_dumb_edge_wireframe_container(int n_edges, tag_t *edges)
{
    int
        rule_types[1] = { UF_SC_EDGE_DUMB_EDGES };
    tag_t
        collector;
    UF_SC_input_data_t
        rule;
    UF_MODL_init_sc_input_data(UF_SC_EDGE_DUMB_EDGES, &rule);
    rule.edge_dumb_input.num_edges = n_edges;
    rule.edge_dumb_input.edges = edges;
    UF_CALL(UF_MODL_create_smart_wireframe_container(edges[0], 1, rule_types,
        &rule, &collector));
    return collector;
}
#define WRITE_D(X) ECHO("%s = %d\n", #X, X);
#define WRITE_S(X) (write_string_to_listing_window(#X, X))
#define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \
    else ECHO("%s = FALSE\n", #X);
void write_string_to_listing_window(char *title, char *string)
{
    if (string != NULL)
        ECHO("%s = \"%s\"\n", title, string);
    else
        ECHO("%s = NULL\n", title);
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
        case UF_circle_type: /* non-std format in the header file 里海译:非标准格式在头文件中的翻译为：头文件中的非标准格式 */
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
    strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话 */
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
        descriptor[MAX_LINE_SIZE+1];
    if ((object != NULL_TAG) &&
        !UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype)))
    {
        ask_object_type_descriptor(type, subtype, descriptor);
        ECHO("%s = eid %d:  %s (%d/%d)\n", name, object, descriptor,
            type, subtype);
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
            UF_CALL(UF_MODL_free_sc_input_data(rule_types[ii], &rules[ii]));
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
    int
        n;
    double
        tol;
    tag_t
        *edges,
        feature;
    UF_MODL_TREX_appl_data_t
        feature_data;
    UF_MODL_TREX_data_set_t
        target_data_set,
        *target_data_sets[1];
    UF_MODL_ask_distance_tolerance(&tol);
    target_data_sets[0] = &target_data_set;
    if ((n = select_edges("Edge to Move", &edges)) == 0) return;
    UF_CALL(UF_MODL_TREX_init_trex_data(&feature_data));
    UF_CALL(UF_MODL_TREX_init_trex_data_set(&target_data_set));
    feature_data.num_targets = 1;
    feature_data.targets = target_data_sets;
    feature_data.targets[0]->collector =
        create_dumb_edge_wireframe_container(n, edges);
    feature_data.targets[0]->extend_val = 1;
/*  These are the default values set by the "init"s
    feature_data.targets[0]->offset_val = 0;
    feature_data.targets[0]->reversed = 0;
    feature_data.num_tools = 0;
    feature_data.tools = NULL;
    feature_data.extend_method = UF_MODL_TREX_C2;
    feature_data.to_option = UF_MODL_TREX_DISTANCE;
    feature_data.region_option = UF_MODL_TREX_KEEP;
    feature_data.make_corner = 0;
    feature_data.keep_boundaries = 0;
    feature_data.extract_sheets = 0;
    feature_data.dist_tol = tol;
    feature_data.algorithm_version = UF_MODL_TREX_ALGORITHM_CURRENT;
*/
    report_trex_feature_data(&feature_data);
    UF_CALL(UF_MODL_TREX_create_trex_feature(&feature_data, &feature));
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

> 这段代码是NX Open C++的一个示例，主要用于实现以下功能：
>
> 1. 定义了一个ECHO宏，用于将信息输出到日志窗口和系统日志。
> 2. 定义了一个UF_CALL宏，用于执行NX API函数，并在发生错误时输出错误信息。
> 3. 实现了mask_for_edges函数，用于设置选择掩码，以选择边。
> 4. 实现了select_edges函数，用于通过对话框选择边，并返回选中的边的数组。
> 5. 实现了create_dumb_edge_wireframe_container函数，用于创建一个包含输入边的哑边线框容器。
> 6. 定义了WRITE_D、WRITE_S、WRITE_L宏，用于输出不同的信息。
> 7. 实现了write_string_to_listing_window函数，用于将字符串输出到日志窗口。
> 8. 实现了ask_object_type_descriptor函数，用于获取NX对象类型的描述字符串。
> 9. 定义了RT宏，用于输出NX对象类型和子类型。
> 10. 实现了report_sc_input_data函数，用于输出智能线框容器的规则数据。
> 11. 实现了report_smart_wireframe_container函数，用于输出智能线框容器信息。
> 12. 实现了report_trex_feature_data函数，用于输出TREX特征数据。
> 13. 实现了do_it函数，用于创建一个TREX特征，并输出特征数据。
> 14. 实现了ufusr函数，用于初始化NX，执行do_it，然后终止NX。
> 15. 实现了ufusr_ask_unload函数，用于卸载用户库。
>
> 整体来看，这段代码通过NX提供的API实现了选择边、创建TREX特征、输出特征数据等功能，为NX的二次开发提供了实用的示例。
>
