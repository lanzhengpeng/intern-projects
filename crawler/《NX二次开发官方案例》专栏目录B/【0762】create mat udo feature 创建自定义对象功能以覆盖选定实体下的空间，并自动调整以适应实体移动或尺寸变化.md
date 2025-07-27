### 【0762】create mat udo feature 创建自定义对象功能以覆盖选定实体下的空间，并自动调整以适应实体移动或尺寸变化

#### 代码

```cpp
/*HEAD CREATE_MAT_UDO_FEATURE CCC UFUN */
/*
    This example program demonstrates creating a UDO feature.  The
    feature is a "mat" which covers the space under selected solids
    even when they are moved or change size.  The user specified
    feature parameters (Hover gap, Border size, & Thickness) are
    automatically presented when using
    Edit->Feature->Parameters...->Feature Dialog
    To add an option Insert->Form Feature->Mat to the modeling menu put
    the resulting shared library in a startup directory and create the
    following two menufiles:
    startup/mat_udo_startup.men:
        VERSION 120
        EDIT UG_GATEWAY_MAIN_MENUBAR
        MODIFY
            APPLICATION_BUTTON UG_APP_MODELING
            MENU_FILES mat_udo_application.men
        END_OF_MODIFY
    application/mat_udo_application.men:
        VERSION 120
        EDIT UG_GATEWAY_MAIN_MENUBAR
        AFTER UG_MODELING_GROOVE_FEATURE
            BUTTON mat_udo_feature
            LABEL  Mat...
            ACTIONS mat_udo_feature
        END_OF_AFTER
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_udobj.h>
#include <uf_object_types.h>
#include <uf_obj.h>
#include <uf_exit.h>
#include <uf_disp.h>
#include <uf_part.h>
#include <uf_modl.h>
#include <uf_so.h>
#include <uf_mb.h>
#include <stdarg.h>
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
static UF_UDOBJ_class_t MAT_class_id = 0;
DllExport extern UF_UDOBJ_class_t get_MAT_class_id(void)
{
    return MAT_class_id;
}
static void calc_mat_corners(tag_t *solids, int n_solids, double up,
    double around, double *corners)
{
    int
        ii, jj;
    double
        this_box[6],
        m_box[6];
    for (ii = 0; ii < n_solids; ii++)
    {
        UF_CALL(UF_MODL_ask_bounding_box(solids[ii], this_box));
        for (jj = 0; jj < 3; jj++)
            if ((ii == 0) || (this_box[jj] < m_box[jj]))
                m_box[jj] = this_box[jj];
        for (jj = 3; jj < 6; jj++)
            if ((ii == 0) || (this_box[jj] > m_box[jj]))
                m_box[jj] = this_box[jj];
    }
    corners[0] = m_box[0] - around;
    corners[1] = m_box[1] - around;
    corners[2] = m_box[2] - up;
    corners[3] = m_box[3] + around;
    corners[4] = m_box[4] + around;
    corners[5] = m_box[2] - up;
}
static void create_or_edit_curve_rectangle(double *corners, tag_t *lines)
{
    UF_CURVE_line_t
        line_data;
    line_data.start_point[0] = corners[0];
    line_data.start_point[1] = corners[1];
    line_data.start_point[2] = corners[2];
    line_data.end_point[0] = corners[3];
    line_data.end_point[1] = corners[1];
    line_data.end_point[2] = corners[2];
    if (lines[0] == NULL_TAG)
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[0]));
    else
        UF_CALL(UF_CURVE_edit_line_data(lines[0], &line_data));
    line_data.start_point[0] = corners[3];
    line_data.start_point[1] = corners[1];
    line_data.start_point[2] = corners[2];
    line_data.end_point[0] = corners[3];
    line_data.end_point[1] = corners[4];
    line_data.end_point[2] = corners[2];
    if (lines[1] == NULL_TAG)
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[1]));
    else
        UF_CALL(UF_CURVE_edit_line_data(lines[1], &line_data));
    line_data.start_point[0] = corners[3];
    line_data.start_point[1] = corners[4];
    line_data.start_point[2] = corners[2];
    line_data.end_point[0] = corners[0];
    line_data.end_point[1] = corners[4];
    line_data.end_point[2] = corners[2];
    if (lines[2] == NULL_TAG)
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[2]));
    else
        UF_CALL(UF_CURVE_edit_line_data(lines[2], &line_data));
    line_data.start_point[0] = corners[0];
    line_data.start_point[1] = corners[4];
    line_data.start_point[2] = corners[2];
    line_data.end_point[0] = corners[0];
    line_data.end_point[1] = corners[1];
    line_data.end_point[2] = corners[2];
    if (lines[3] == NULL_TAG)
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[3]));
    else
        UF_CALL(UF_CURVE_edit_line_data(lines[3], &line_data));
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
static int ask_udo_linked_solids(tag_t udo, tag_t **solids)
{
    unsigned int
        ii;
    int
        subtype,
        type;
    UF_UDOBJ_all_data_t
        data;
    uf_list_p_t
        solid_list;
    UF_CALL(UF_MODL_create_list(&solid_list));
    UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));
    for (ii = 0; ii < data.num_links; ii++)
    {
        UF_CALL(UF_OBJ_ask_type_and_subtype(data.link_defs[ii].assoc_ug_tag,
            &type, &subtype));
        if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype))
            UF_CALL(UF_MODL_put_list_item(solid_list,
                data.link_defs[ii].assoc_ug_tag));
    }
    UF_CALL(UF_UDOBJ_free_udo_data(&data));
    return make_an_array(&solid_list, solids);
}
/* 里海 */
DllExport extern void mat_update(tag_t udo, UF_UDOBJ_link_p_t update_cause)
{
    int
        n_lines,
        n_solids;
    tag_t
        *lines,
        *solids;
    double
        border,
        corners[6],
        hover;
    if (UF_CALL(UF_initialize())) return;
    n_solids = ask_udo_linked_solids(udo, &solids);
    UF_CALL(UF_MODL_eval_exp("mat_hover_gap", &hover));
    UF_CALL(UF_MODL_eval_exp("mat_border_size", &border));
    calc_mat_corners(solids, n_solids, hover, border, corners);
    UF_CALL(UF_UDOBJ_ask_owned_objects(udo, &n_lines, &lines));
    create_or_edit_curve_rectangle(corners, lines);
    if (n_solids > 0) UF_free(solids);
    if (n_lines > 0) UF_free(lines);
    UF_terminate();
}
static void register_mat_udo(void)
{
    UF_CALL(UF_UDOBJ_create_class("mat_UDO", "MAT", &MAT_class_id));
    UF_CALL(UF_UDOBJ_register_update_cb(MAT_class_id, mat_update));
}
static tag_t ask_next_udo(tag_t part, UF_UDOBJ_class_t class_id, tag_t udo)
{
    UF_UDOBJ_class_t
        this_class_id;
    UF_UDOBJ_all_data_t
        udo_data;
    while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part,
        UF_user_defined_object_type, &udo)) && (udo != NULL_TAG))
    {
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &udo_data));
        this_class_id = udo_data.class_id;
        UF_CALL(UF_UDOBJ_free_udo_data(&udo_data));
        if (this_class_id == class_id) return udo;
    }
    return NULL_TAG;
}
/* 里海 */
static int filter_body_type(tag_t object, int type[3], void *data,
    UF_UI_selection_p_t select)
{
    int
        body_type;
    UF_CALL(UF_MODL_ask_body_type(object, &body_type));
    if (body_type == *(int *)data)
        return (UF_UI_SEL_ACCEPT);
    else
        return (UF_UI_SEL_REJECT);
}
static int mask_body_type( UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&
        !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static int select_solids(char *prompt, tag_t **solids)
{
    int
        ii,
        n,
        solid = UF_MODL_SOLID_BODY,
        resp;
    UF_CALL(UF_UI_select_with_class_dialog( prompt, "",
        UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp, &n, solids));
    for (ii = 0; ii < n; ii++)
        UF_CALL(UF_DISP_set_highlight((*solids)[ii], FALSE));
    return n;
}
static logical prompt_for_three_numbers(char *prompt, char *item1, char *item2,
    char * item3, double *number1, double *number2, double *number3)
{
    int
        irc,
        resp;
    char
        menu[3][16];
    double
        da[3];
    strncpy(&menu[0][0], item1, 15);
    strncpy(&menu[1][0], item2, 15);
    strncpy(&menu[2][0], item3, 15);
    da[0] = *number1;
    da[1] = *number2;
    da[2] = *number3;
    resp = uc1609(prompt, menu, 3, da, &irc);
    if (resp == 3 || resp == 4)
    {
        *number1 = da[0];
        *number2 = da[1];
        *number3 = da[2];
        return TRUE;
    }
    else return FALSE;
}
static uf_list_p_t make_a_list(int count, tag_t *item_array)
{
    int
        ii;
    uf_list_p_t
        list;
    UF_CALL(UF_MODL_create_list(&list));
    for (ii = 0; ii < count; ii++)
        UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));
    return (list);
}
static void strip_trailing_zeros(char *s)
{
    size_t ii;
    if (strchr(s, '.'))
    {
        for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';
        if (s[ii] == '.') s[ii] = '\0';
    }
}
static logical ask_yes_or_no(char *prompt, logical *deflt)
{
    int
        resp;
    char
        options[2][38] = { "Yes", "No" };
    resp = 2 - *deflt;
    resp = uc1603(prompt, resp, options, 2);
    if ((resp > 4) && (resp < 19))
    {
        *deflt = 6 - resp;
        return TRUE;
    }
    else return FALSE;
}
static int ask_udo_linked_objects_of_type(tag_t udo, int what, tag_t **objects)
{
    unsigned int
        ii;
    int
        subtype,
        type;
    UF_UDOBJ_all_data_t
        data;
    uf_list_p_t
        object_list;
    UF_CALL(UF_MODL_create_list(&object_list));
    UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));
    for (ii = 0; ii < data.num_links; ii++)
    {
        UF_CALL(UF_OBJ_ask_type_and_subtype(data.link_defs[ii].assoc_ug_tag,
            &type, &subtype));
        if (type == what) UF_CALL(UF_MODL_put_list_item(object_list,
                data.link_defs[ii].assoc_ug_tag));
    }
    UF_CALL(UF_UDOBJ_free_udo_data(&data));
    return make_an_array(&object_list, objects);
}
DllExport extern void create_or_edit_mat_udo_feature(void)
{
    logical
        redo = TRUE;
    int
        ii,
        n_children,
        n_parents,
        n_scalars,
        n_solids;
    tag_t
        *children,
        lines[4] = { NULL_TAG, NULL_TAG, NULL_TAG, NULL_TAG },
        exps[3],
        *parents,
        part = UF_PART_ask_display_part(),
        *scalars,
        *solids,
        so_exps[3],
        udo = NULL_TAG,
        udo_feat;
    double
        border = 1,
        corners[6],
        hover = 0,
        minus_z[3] = { 0,0,-1 },
        size = 0.25;
    char
        exp_str[UF_MAX_EXP_LENGTH],
        *limits[2] = { "0", "mat_thickness" };
    uf_list_p_t
        bound,
        list;
    UF_UDOBJ_class_t
        mat_class = get_MAT_class_id();
    UF_UDOBJ_link_t
        *link_defs;
    if (mat_class == 0)
    {
        register_mat_udo();
        mat_class = get_MAT_class_id();
    }
    if ((udo = ask_next_udo(part, mat_class, udo)) != NULL_TAG)
    {
        if (!ask_yes_or_no("Mat exists - Delete and recreate?", &redo)
            || !redo) return;
        UF_CALL(UF_MODL_eval_exp("mat_hover_gap", &hover));
        UF_CALL(UF_MODL_eval_exp("mat_border_size", &border));
        UF_CALL(UF_MODL_eval_exp("mat_thickness", &size));
        n_scalars = ask_udo_linked_objects_of_type(udo, UF_scalar_type, &scalars);
        UF_CALL(UF_UDOBJ_ask_udo_feature_of_udo(udo, &udo_feat));
        UF_CALL(UF_MODL_ask_feat_relatives(udo_feat, &n_parents, &parents,
            &n_children, &children));
        list = make_a_list(n_children, children);
        if (n_children > 0) UF_free(children);
        if (n_parents > 0) UF_free(parents);
        UF_CALL(UF_MODL_put_list_item(list, udo_feat));
        UF_CALL(UF_MODL_delete_feature(list));
        UF_CALL(UF_MODL_delete_list(&list));
        UF_CALL(UF_OBJ_delete_array_of_objects(n_scalars, scalars, NULL));
        UF_free(scalars);
        UF_CALL(UF_MODL_delete_exp("mat_hover_gap"));
        UF_CALL(UF_MODL_delete_exp("mat_border_size"));
        UF_CALL(UF_MODL_delete_exp("mat_thickness"));
    }
    if (((n_solids = select_solids("Select solids to mat", &solids)) > 0)
        && prompt_for_three_numbers("Mat parameters", "Hover gap",
            "Border size", "Thickness", &hover, &border, &size))
    {
        calc_mat_corners(solids, n_solids, hover, border, corners);
        create_or_edit_curve_rectangle(corners, lines);
        sprintf(exp_str, "mat_hover_gap=%f", hover);
        strip_trailing_zeros(exp_str);
        UF_CALL(UF_MODL_create_exp_tag(exp_str, &exps[0]));
        sprintf(exp_str, "mat_border_size=%f", border);
        strip_trailing_zeros(exp_str);
        UF_CALL(UF_MODL_create_exp_tag(exp_str, &exps[1]));
        sprintf(exp_str, "mat_thickness=%f", size);
        strip_trailing_zeros(exp_str);
        UF_CALL(UF_MODL_create_exp_tag(exp_str, &exps[2]));
        UF_CALL(UF_UDOBJ_create_udo_feature(mat_class, &udo, &udo_feat));
        UF_CALL(UF_MODL_update());
        UF_CALL(allocate_memory((n_solids + 3) * sizeof(UF_UDOBJ_link_t),
            (void *) &link_defs));
        for (ii = 0; ii < 3; ii++)
        {
            UF_CALL(UF_SO_create_scalar_exp(exps[ii],
                UF_SO_update_within_modeling, exps[ii], &so_exps[ii]));
            link_defs[ii].link_type = 1;
            link_defs[ii].assoc_ug_tag = so_exps[ii];
            link_defs[ii].object_status = 0;
        }
        for (ii = 3; ii < n_solids + 3; ii++)
        {
            link_defs[ii].link_type = 1;
            link_defs[ii].assoc_ug_tag = solids[ii-3];
            link_defs[ii].object_status = 0;
        }
        UF_CALL(UF_UDOBJ_add_links(udo, n_solids + 3, link_defs));
        UF_free(link_defs);
        UF_CALL(UF_UDOBJ_add_owning_links(udo, 4, lines));
        bound = make_a_list(4, lines);
        UF_CALL(UF_MODL_create_extruded(bound, "0", limits, corners,
            minus_z, UF_NULLSIGN, &list));
        UF_CALL(UF_MODL_delete_list(&bound));
        UF_CALL(UF_MODL_delete_list(&list));
        UF_free(solids);
    }
}
/* 里海 */
void ufusr(char *param, int *retcode, int paramLen)
{
    if (UF_CALL(UF_initialize())) return;
    create_or_edit_mat_udo_feature();
    UF_terminate();
}
/* 里海 */
DllExport extern UF_MB_cb_status_t mat_udo_action(UF_MB_widget_t w,
    UF_MB_data_t client_data, UF_MB_activated_button_p_t button)
{
    if (UF_CALL(UF_initialize())) return UF_MB_CB_CONTINUE;
    create_or_edit_mat_udo_feature();
    UF_terminate();
    return UF_MB_CB_CONTINUE;
}
/* 里海 */
void ufsta(char *param, int *retcode, int paramLen)
{
    UF_MB_action_t
        action_table[] =
        {
            { "mat_udo_feature", mat_udo_action, NULL },
            { NULL, NULL, NULL}
        };
    if (UF_CALL(UF_initialize())) return;
    register_mat_udo();
    UF_CALL(UF_MB_add_actions(action_table));
    UF_terminate();
}

```

#### 代码解析

> 根据代码，该段代码是用于创建或编辑一个名为“mat_UDO”的用户定义对象(UDO)特征的示例代码，主要功能如下：
>
> 1. 定义了UDO的更新回调函数mat_update，用于在相关实体移动或改变大小时更新特征。
> 2. 在create_or_edit_mat_udo_feature函数中，可以选择多个实体，并设置悬停间隙、边界大小和厚度等参数，以创建或编辑一个MAT UDO特征。
> 3. 使用UF_MODL_create_extruded创建一个基于矩形框的拉伸特征，以覆盖选择的实体。
> 4. 通过UF_UDOBJ_add_links将UDO与选择的实体和标量表达式相关联。
> 5. 定义了mat_udo_action函数，用于在菜单栏中添加“Mat”按钮，并调用create_or_edit_mat_udo_feature。
> 6. 使用ufusr和ufsta函数入口点，分别用于NX的start up和action。
>
> 总体而言，该代码提供了UDO特征的创建和编辑功能，通过NX的菜单或命令来调用，并能够自动更新以适应实体变化。
>
