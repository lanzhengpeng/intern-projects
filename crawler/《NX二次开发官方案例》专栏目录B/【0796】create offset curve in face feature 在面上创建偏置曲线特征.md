### 【0796】create offset curve in face feature 在面上创建偏置曲线特征

#### 代码

```cpp
/*HEAD CREATE_OFFSET_CURVE_IN_FACE_FEATURE CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_object_types.h>
#include <uf_modl.h>
#include <uf_curve.h>
#include <uf_sc.h>
#include <uf_disp.h>
#include <uf_obj.h>
#include <uf_undo.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本中新增的函数。 */
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
/* 里海 */
static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask[5] = { { UF_line_type, 0, 0 },
                    { UF_circle_type, 0, 0 },
                    { UF_conic_type, 0, 0 },
                    { UF_spline_type, 0, 0 },
                    { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
extern tag_t select_a_curve_or_edge(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog("Select a curve or edge", prompt,
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,
        &resp, &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, 0));
        return object;
    }
    else return NULL_TAG;
}
/* 里海 */
static int mask_for_faces(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static tag_t select_a_face(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, FALSE));
        return object;
    }
    else return NULL_TAG;
}
static logical ask_yes_or_no(char *title, char *message)
{
    int
        resp;
    UF_UI_message_buttons_t
        buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };
    UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,
        TRUE, &buttons, &resp));
    return (2 - resp);
}
static logical prompt_for_a_number(char *prompt, char *item, double *number)
{
    int
        irc,
        resp;
    char
        menu[1][16];
    double
        da[1];
    strncpy(&menu[0][0], item, 16);
    menu[0][15] = '\0';
    da[0] = *number;
    resp = uc1609(prompt, menu, 1, da, &irc);
    if (resp == 3 || resp == 4)
    {
        *number = da[0];
        return TRUE;
    }
    else return FALSE;
}
static void do_it(void)
{
    int
        resp,
        rule_types[1],
        subtype,
        type;
    tag_t
        curve,
        face,
        feat;
    double
        dist = 1.0,
        offset_direction[3],
        offset_point[3];
    UF_SC_section_data_t
        sec_rule;
    UF_SC_input_data_t
        rule;
    UF_CURVE_ocf_data_t
        *offset_data;
    UF_UNDO_mark_id_t
        mark;
    while (((curve = select_a_curve_or_edge("Offset in Face")) != NULL_TAG) &&
        ((face = select_a_face("Offset in Face")) != NULL_TAG) &&
        prompt_for_a_number("Offset in Face", "Distance", &dist))
    {
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, "Offset Curve in Face",
            &mark));
        offset_data = UF_allocate_memory(sizeof(UF_CURVE_ocf_data_t), &resp);
        UF_CALL(UF_CURVE_init_ocf_data(offset_data));
        offset_data->string_data =
            UF_allocate_memory(sizeof(UF_CURVE_ocf_string_data_t), &resp);
        offset_data->num_string_data = 1;
        UF_MODL_initialize_section_data(&sec_rule);
        sec_rule.n_rules = 1;
        sec_rule.rule_types = UF_allocate_memory(sizeof(int), &resp);
        sec_rule.rules = UF_allocate_memory(sizeof(UF_SC_input_data_t), &resp);
        UF_CALL(UF_OBJ_ask_type_and_subtype(curve, &type, &subtype));
        if (type == UF_solid_type)
        {
            UF_MODL_init_sc_input_data(UF_SC_EDGE_DUMB_EDGES,
                &sec_rule.rules[0]);
            sec_rule.rule_types[0] = UF_SC_EDGE_DUMB_EDGES;
            sec_rule.rules[0].edge_dumb_input.num_edges = 1;
            sec_rule.rules[0].edge_dumb_input.edges =
                UF_allocate_memory(sizeof(tag_t), &resp);
            sec_rule.rules[0].edge_dumb_input.edges[0] = curve;
        }
        else
        {
            UF_MODL_init_sc_input_data(UF_SC_CURVE_DUMB_CURVES,
                &sec_rule.rules[0]);
            sec_rule.rule_types[0] = UF_SC_CURVE_DUMB_CURVES;
            sec_rule.rules[0].curve_dumb_input.num_curves = 1;
            sec_rule.rules[0].curve_dumb_input.curves =
                UF_allocate_memory(sizeof(tag_t), &resp);
            sec_rule.rules[0].curve_dumb_input.curves[0] = curve;
        }
        UF_CALL(UF_MODL_create_section(curve, 1, &sec_rule, NULL_TAG, NULL,
            NULL, FALSE, &offset_data->string_data->string_tag));
        UF_CALL(UF_MODL_free_section_data(&sec_rule));
        offset_data->string_data->offset_direction = 1;
        offset_data->string_data->num_offsets = 1;
        offset_data->string_data->offset_distances =
            UF_allocate_memory(sizeof(UF_CURVE_ocf_values_t), &resp);
        sprintf(offset_data->string_data->offset_distances[0].string,
            "%f", dist);
        offset_data->face_data =
            UF_allocate_memory(sizeof(UF_CURVE_ocf_face_data_t), &resp);
        UF_MODL_init_sc_input_data(UF_SC_FACE_DUMB_FACES, &rule);
        rule_types[0] = UF_SC_FACE_DUMB_FACES;
        rule.face_dumb_input.num_faces = 1;
        rule.face_dumb_input.faces = UF_allocate_memory(sizeof(tag_t), &resp);
        rule.face_dumb_input.faces[0] = face;
        UF_CALL(UF_MODL_create_smart_face_container(face, 1, rule_types, &rule,
            &offset_data->face_data->face_tag));
        UF_CALL(UF_MODL_free_sc_input_data(UF_SC_FACE_DUMB_FACES, &rule));
        UF_CALL(UF_CURVE_ocf_offset_pt_direction(
            offset_data->string_data->string_tag,
            offset_data->face_data->face_tag, offset_point, offset_direction));
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, offset_point,
            offset_direction, 0);
        if (ask_yes_or_no("Offset curve in face", "Flip direction?"))
            offset_data->string_data->offset_direction = -1;
        offset_data->cross_boundary_mode = UF_CURVE_OCF_CROSS_BOUNDARIES_NONE;
        offset_data->offset_method = UF_CURVE_OCF_CHORDAL;
        offset_data->trim_method = UF_CURVE_OCF_TANGENT;
        offset_data->span_method = UF_CURVE_OCF_SPAN_NONE;
        offset_data->dist_tol = 0.000100;
        offset_data->ang_tol = 0.500000;
        offset_data->string_tol = 0.000100;
        UF_CALL(UF_CURVE_create_ocf_feature(offset_data, &feat));
        UF_CALL(UF_CURVE_free_ocf_data(&offset_data));
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

> 这是段NX二次开发代码，主要功能是在NX中创建曲线或边的偏移特征。以下是代码的主要功能和实现：
>
> 1. 功能介绍：该代码允许用户选择一个曲线或边以及一个面，然后指定偏移距离，在面上创建曲线或边的偏移特征。
> 2. 选择曲线或边：通过select_a_curve_or_edge函数，用户可以从NX中任意选择一个曲线或边。
> 3. 选择面：通过select_a_face函数，用户可以从NX中任意选择一个面。
> 4. 输入偏移距离：通过prompt_for_a_number函数提示用户输入偏移距离。
> 5. 创建偏移特征：在用户选择曲线/边、面并输入偏移距离后，调用NX API创建偏移特征。
> 6. 可视化偏移方向：在创建偏移特征前，会显示偏移方向以供用户确认。
> 7. 确认偏移方向：如果用户选择翻转偏移方向，代码会相应调整。
> 8. 偏移参数设置：设置了偏移的参数，如跨边界模式、偏移方法、修剪方法等。
> 9. 调用API创建偏移特征：通过调用NX的偏移曲线API，根据输入参数在面上创建曲线/边的偏移特征。
> 10. 资源释放：在创建完偏移特征后，释放相关资源。
> 11. 错误处理：通过错误处理函数report_error记录错误。
> 12. 二次开发接口：通过UF函数接口实现NX的二次开发。
>
> 总的来说，这段代码实现了在NX中创建曲线/边的偏移特征的功能，使用户可以便捷地通过选择和输入参数来创建所需的偏移特征。
>
