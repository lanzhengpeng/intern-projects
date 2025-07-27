### 【2495】report sweep data 报告扫描数据

#### 代码

```cpp
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_modl.h>
#include <uf_modl_mswp.h>
#include <uf_object_types.h>
#include <uf_obj.h>
#include <uf_part.h>
#include <uf_sc.h>
#include <uf_so.h>
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
static tag_t ask_next_feature(tag_t part, tag_t feat)
{
    while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))
        && (feat != NULL_TAG))
        if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;
    return NULL_TAG;
}
static int allocate_memory(unsigned int nbytes, void **where)
{
    int
        resp;
    *where = UF_allocate_memory(nbytes, &resp);
    return resp;
}
static int make_an_array(uf_list_p_t *object_list, tag_t **objects)
{
    int
        ii,
        n;
    uf_list_p_t
        temp;
    UF_CALL(UF_MODL_ask_list_count(*object_list, &n));
    UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));
    for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)
        (*objects)[ii] = temp->eid;
    UF_CALL(UF_MODL_delete_list(object_list));
    return n;
}
static int ask_all_features(tag_t part, tag_p_t *feats)
{
    tag_t
        feat = NULL_TAG;
    uf_list_p_t
        feat_list;
    UF_CALL(UF_MODL_create_list(&feat_list));
    while ((feat = ask_next_feature(part, feat)) != NULL_TAG)
        UF_CALL(UF_MODL_put_list_item(feat_list, feat));
    return (make_an_array(&feat_list, feats));
}
#define WRITE(X) ECHO("%s", X);
#define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \
    else ECHO("%s = FALSE\n", #X);
#define WRITE_F(X) ECHO("%s = %f\n", #X, X)
#define WRITE_D(X) ECHO("%s = %d\n", #X, X);
#define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);
#define WRITE_S(X) (write_string_to_listing_window(#X, X))
void write_string_to_listing_window(char *title, char *string)
{
    if (string != NULL)
        ECHO("%s = \"%s\"\n", title, string);
    else
        ECHO("%s = NULL\n", title);
}
#define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))
#define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))
#define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))
#define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))
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
        blanks[UF_UI_MAX_STRING_LEN+1];
    memset(blanks, ' ', strlen(name) + 3);
    blanks[strlen(name) + 3] = '\0';
    for (ii = 0; ii < rows; ii++)
    {
        if (cols > 0)
            if (ii == 0) ECHO("%s = ", name);
            else ECHO("%s", blanks);
        for (jj = 0; jj < cols; jj++)
        {
            ECHO("%.3f", array[kk++]);
            if (jj < cols - 1) ECHO("%s", ", ");
        }
        if (cols > 0) ECHO("%s", "\n");
    }
}
#define WRITEnT(n,X) (write_tag_array_to_listing_window(#X, X, 1, n))
static void write_tag_array_to_listing_window(char *name, tag_t *array,
        int rows, int cols)
{
    int
        ii,
        jj,
        kk = 0;
    char
        blanks[UF_UI_MAX_STRING_LEN+1];
    memset(blanks, ' ', strlen(name) + 3);
    blanks[strlen(name) + 3] = '\0';
    for (ii = 0; ii < rows; ii++)
    {
        if (cols > 0)
            if (ii == 0) ECHO("%s = ", name);
            else ECHO("%s", blanks);
        for (jj = 0; jj < cols; jj++)
        {
            ECHO("%d", array[kk++]);
            if (jj < cols - 1)
            {
                ECHO("%s", ", ");
            }
        }
        if (cols > 0) ECHO("%s", "\n");
    }
}
static void report_feature_name_and_type(char *title, tag_t feat)
{
    char
        msg[MAX_LINE_SIZE+1],
        *name,
        *type;
    if (!UF_CALL(UF_MODL_ask_feat_name(feat, &name)) &&
        !UF_CALL(UF_MODL_ask_feat_type(feat, &type)))
    {
        UF_CALL(UF_UI_open_listing_window());
        sprintf(msg, "%s = %d %s (%s)\n", title, feat, name, type);
        UF_CALL(UF_UI_write_listing_window(msg));
        UF_free(name);
        UF_free(type);
    }
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
        ii,
        n_objects,
        n_guides,
        n_profiles,
        n_feats;
    tag_t
        *objects,
        *feats,
        *guides,
        *profiles,
        part = UF_PART_ask_display_part();
    double
        dir[3],
        dir_x[3],
        dir_y[3],
        direction[3],
        location[3],
        pos[3],
        region_point[3];
    char
        *feat_type,
        *limit1,
        *limit2,
        *limits[2],
        *offsets[2],
        *taper_angle;
    UF_MODL_SWEEP_TRIM_object_t
        *trim_ptr;
    UF_MODL_mswp_extrude_t
        extrude;
    n_feats = ask_all_features(part, &feats);
    WRITE_D(n_feats);
    for (ii = 0; ii < n_feats; ii++)
    {
        WRITE_D(ii);
        report_feature_name_and_type("feats[ii]", feats[ii]);
        UF_CALL(UF_MODL_ask_feat_type(feats[ii], &feat_type));
        if (!strcmp(feat_type, "SWP104"))
        {
            WRITE("\nFrom UF_MODL_ask_feat_location:\n");
            if (!UF_CALL(UF_MODL_ask_feat_location(feats[ii], location)))
            {
                WRITE3F(location);
            }
            WRITE("\nFrom UF_MODL_ask_feat_direction:\n");
            if (!UF_CALL(UF_MODL_ask_feat_direction(feats[ii], dir_x, dir_y)))
            {
                WRITE3F(dir_x);
                WRITE3F(dir_y);
            }
        /*  This may choke - see PR 1095526 里海译:翻译：这可能会造成阻塞 - 见 PR 1095526 */
            WRITE("\nFrom UF_MODL_ask_sweep_direction:\n");
            if (!UF_CALL(UF_MODL_ask_sweep_direction(feats[ii], pos, dir)))
            {
                WRITE3F(pos);
                WRITE3F(dir);
            }
            WRITE("\nFrom UF_MODL_ask_sweep_parms:\n");
            if (!UF_CALL(UF_MODL_ask_sweep_parms(feats[ii], 0, &taper_angle,
                    &limit1, &limit2)))
            {
                WRITE_S(taper_angle);
                WRITE_S(limit1);
                WRITE_S(limit2);
                UF_free(taper_angle);
                UF_free(limit1);
                UF_free(limit2);
            }
            WRITE("\nFrom UF_MODL_ask_sweep_curves:\n");
            if (!UF_CALL(UF_MODL_ask_sweep_curves(feats[ii], &n_profiles,
                &profiles, &n_guides, &guides)))
            {
                WRITE_D(n_profiles);
                WRITEnT(n_profiles, profiles);
                WRITE_D(n_guides);
                WRITEnT(n_guides, guides);
                if (n_profiles > 0) UF_free(profiles);
                if (n_guides > 0) UF_free(guides);
            }
            WRITE("\nFrom UF_MODL_ask_extrusion:\n");
            if (!UF_CALL(UF_MODL_ask_extrusion(feats[ii], &n_objects, &objects,
                &trim_ptr, &taper_angle, limits, offsets, region_point,
                &region_specified, &solid_creation, direction)))
            {
                WRITE_D(n_objects);
                WRITEnT(n_objects, objects);
                if (trim_ptr != NULL)
                {
                    WRITEnT(trim_ptr->trim_count, trim_ptr->trim_objects);
                    WRITE_D(trim_ptr->trim_count);
                    if (trim_ptr->trim_count) UF_free(trim_ptr->trim_objects);
                    WRITE_D(trim_ptr->sign);
                    WRITEnT(trim_ptr->num_thru_bodies, trim_ptr->thru_bodies);
                    WRITE_D(trim_ptr->num_thru_bodies);
                    if (trim_ptr->trim_count) UF_free(trim_ptr->trim_objects);
                    UF_free(trim_ptr);
                }
                WRITE_S(taper_angle);
                WRITE_S(limits[0]);
                WRITE_S(limits[1]);
                WRITE_S(offsets[0]);
                WRITE_S(offsets[1]);
                if (region_specified) WRITE3F(region_point);
                WRITE_L(region_specified);
                WRITE_L(solid_creation);
                WRITE3F(direction);
                if (n_objects > 0) UF_free(objects);
                UF_free(taper_angle);
                UF_free(limits[0]);
                UF_free(limits[1]);
                UF_free(offsets[0]);
                UF_free(offsets[1]);
            }
            WRITE("\n");
        }
        if (!strcmp(feat_type, "EXTRUDE"))
        {
            WRITE("\nFrom UF_MODL_mswp_ask_extrude:\n");
            if (!UF_CALL(UF_MODL_mswp_ask_extrude(feats[ii], &extrude)))
            {
                WRITE("extrude.section data:\n");
                report_section_data(extrude.section);
                UF_CALL(UF_SO_ask_direction_of_dirr(extrude.direction,
                    direction));
                WRITE3F(direction);
                WRITE_F(extrude.dist_tol);
                WRITE_F(extrude.planar_tol);
                WRITE_F(extrude.angle_tol);
                WRITE_L(extrude.limits.symmetry);
                WRITE("extrude.limits.start_limit.limit_type = ");
                switch (extrude.limits.start_limit.limit_type)
                {
                    case UF_MODL_MSWP_LIMIT_DISTANCE:
                        WRITE_D(UF_MODL_MSWP_LIMIT_DISTANCE);
                        WRITE_S(extrude.limits.start_limit.limit_data.
                            distance_data.string);
                        break;
                    case UF_MODL_MSWP_LIMIT_UNTIL_NEXT:
                        WRITE_D(UF_MODL_MSWP_LIMIT_UNTIL_NEXT);
                        break;
                    case UF_MODL_MSWP_LIMIT_UNTIL_SELECTED:
                        WRITE_D(UF_MODL_MSWP_LIMIT_UNTIL_SELECTED);
                        RT(extrude.limits.start_limit.limit_data.
                            until_selected_data.selected_object);
                        break;
                    case UF_MODL_MSWP_LIMIT_THRU_ALL:
                        WRITE_D(UF_MODL_MSWP_LIMIT_THRU_ALL);
                        break;
                    default:
                        WRITE("Unknown ");
                        WRITE_D(extrude.limits.start_limit.limit_type);
                        break;
                }
                WRITE_L(extrude.limits.start_limit.surf_extend);
                WRITE("extrude.limits.end_limit.limit_type = ");
                switch (extrude.limits.end_limit.limit_type)
                {
                    case UF_MODL_MSWP_LIMIT_DISTANCE:
                        WRITE_D(UF_MODL_MSWP_LIMIT_DISTANCE);
                        WRITE_S(extrude.limits.end_limit.limit_data.
                            distance_data.string);
                        break;
                    case UF_MODL_MSWP_LIMIT_UNTIL_NEXT:
                        WRITE_D(UF_MODL_MSWP_LIMIT_UNTIL_NEXT);
                        break;
                    case UF_MODL_MSWP_LIMIT_UNTIL_SELECTED:
                        WRITE_D(UF_MODL_MSWP_LIMIT_UNTIL_SELECTED);
                        RT(extrude.limits.end_limit.limit_data.
                            until_selected_data.selected_object);
                        break;
                    case UF_MODL_MSWP_LIMIT_THRU_ALL:
                        WRITE_D(UF_MODL_MSWP_LIMIT_THRU_ALL);
                        break;
                    default:
                        WRITE("Unknown ");
                        WRITE_D(extrude.limits.end_limit.limit_type);
                        break;
                }
                WRITE("extrude.offsets.type = ");
                switch (extrude.offsets.type)
                {
                    case UF_MODL_MSWP_OFFSET_NONE:
                        WRITE_D(UF_MODL_MSWP_OFFSET_NONE);
                        break;
                    case UF_MODL_MSWP_OFFSET_NORMAL:
                        WRITE_D(UF_MODL_MSWP_OFFSET_NORMAL);
                        break;
                    case UF_MODL_MSWP_OFFSET_SYMMETRY:
                        WRITE_D(UF_MODL_MSWP_OFFSET_SYMMETRY);
                        break;
                    case UF_MODL_MSWP_OFFSET_SINGLE:
                        WRITE_D(UF_MODL_MSWP_OFFSET_SINGLE);
                        break;
                    default:
                        WRITE("Unknown ");
                        WRITE_D(extrude.offsets.type);
                        break;
                }
                WRITE_S(extrude.offsets.start_offset.string);
                WRITE_D(extrude.offsets.start_offset.expression_tag);
                WRITE_S(extrude.offsets.end_offset.string);
                WRITE_D(extrude.offsets.end_offset.expression_tag);
                WRITE("extrude.taper.type = ");
                switch (extrude.taper.type)
                {
                    case UF_MODL_MSWP_TAPER_NONE:
                        WRITE_D(UF_MODL_MSWP_TAPER_NONE);
                        break;
                    case UF_MODL_MSWP_TAPER_FROM_EDGE:
                        WRITE_D(UF_MODL_MSWP_TAPER_FROM_EDGE);
                        break;
                    case UF_MODL_MSWP_TAPER_FROM_PROFILE:
                        WRITE_D(UF_MODL_MSWP_TAPER_FROM_PROFILE);
                        break;
                    case UF_MODL_MSWP_TAPER_SYMMETRY:
                        WRITE_D(UF_MODL_MSWP_TAPER_SYMMETRY);
                        break;
                    case UF_MODL_MSWP_TAPER_MATCHED:
                        WRITE_D(UF_MODL_MSWP_TAPER_MATCHED);
                        break;
                    case UF_MODL_MSWP_TAPER_ASYMMETRIC:
                        WRITE_D(UF_MODL_MSWP_TAPER_ASYMMETRIC);
                        break;
                    default:
                        WRITE("Unknown ");
                        WRITE_D(extrude.taper.type);
                        break;
                }
                WRITE_S(extrude.taper.angle.string);
                WRITE_D(extrude.taper.angle.expression_tag);
                WRITE_S(extrude.taper.back_angle.string);
                WRITE_D(extrude.taper.back_angle.expression_tag);
                WRITE("... etc ... \n");
            }
        }
        UF_free(feat_type);
    }
    if (n_feats > 0) UF_free(feats);
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

> 这是段NX的二次开发代码，主要功能是遍历当前激活零件的所有特征，并查询SWEEP和EXTRUDE特征的相关参数。
>
> 主要步骤包括：
>
> 1. 遍历当前激活零件的所有特征，并存储特征数组。
> 2. 遍历特征数组，对每个特征，先获取特征类型。
> 3. 如果特征类型为SWEEP，则查询其位置、方向、轮廓曲线、引导曲线、拉伸参数等信息。
> 4. 如果特征类型为EXTRUDE，则查询其拉伸方向、截面、拉伸参数等信息。
> 5. 通过UF_MODL_ask_sweep_parms等函数查询SWEEP特征的各种参数。
> 6. 通过UF_MODL_mswp_ask_extrude函数查询EXTRUDE特征的各个参数。
> 7. 将查询到的信息输出到日志窗口中。
> 8. 最后释放特征数组。
>
> 该代码主要用于调试和学习SWEEP和EXTRUDE特征的各种参数查询接口。
>
