### 【0721】create idsymbols 创建ID符号

#### 代码

```cpp
/*HEAD CREATE_IDSYMBOLS CCC UFUN */
#include <stdio.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_obj.h>
#include <uf_object_types.h>
#include <uf_disp.h>
#include <uf_drf.h>
#include <uf_curve.h>
#include <uf_modl.h>
#include <uf_view.h>
#include <uf_csys.h>
#include <uf_draw.h>
#include <uf_part.h>
#include <uf_vec.h>
#include <uf_trns.h>
#include <uf_mtx.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是 V18 版本中新增的功能。 */
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
static void map_abs2view(double c[3])
{
    char
        vname[30];
    double
        abs[9] = { 0,0,0, 1,0,0, 0,1,0 },
        vw[12] = { 0,0,0 },
        mx[12];
    int
        irc;
    UF_CALL(uc1653(vname));
    UF_CALL(uc6433(vname, &vw[3]));
    FTN(uf5940)(abs, vw, mx, &irc);
    FTN(uf5941)(c, mx);
}
static void map_view2abs(double c[3])
{
    char
        vname[30];
    double
        abs[9] = { 0,0,0, 1,0,0, 0,1,0 },
        vw[12] = { 0,0,0 },
        mx[12];
    int
        irc;
    UF_CALL(uc1653(vname));
    UF_CALL(uc6433(vname, &vw[3]));
    FTN(uf5940)(vw, abs, mx, &irc);
    FTN(uf5941)(c, mx);
}
static void ask_pos_on_obj(tag_t obj, double loc[3])
{
    tag_t
        line;
    double
        cp[3],
        dist;
    UF_CURVE_line_t
        lp;
    map_abs2view(loc);
    lp.start_point[0] = loc[0];
    lp.start_point[1] = loc[1];
    lp.start_point[2] = loc[2] + 1000;
    lp.end_point[0] = loc[0];
    lp.end_point[1] = loc[1];
    lp.end_point[2] = loc[2] - 1000;
    map_view2abs(lp.start_point);
    map_view2abs(lp.end_point);
    UF_CALL(UF_CURVE_create_line(&lp, &line));
    UF_CALL(UF_MODL_ask_minimum_dist(obj, line, 0, cp, 0, cp, &dist, loc, cp));
    UF_CALL(UF_OBJ_delete_object(line));
}
static int mask_for_drf_curves(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask[7] = { { UF_line_type, 0, 0 },
                    { UF_circle_type, 0, 0 },
                    { UF_conic_type, 0, 0 },
                    { UF_spline_type, 0, 0 },
                    { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },
                    { UF_solid_silhouette_type, 0, 0 },
                    { UF_section_edge_type, 0, 0 }};
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 7, mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static logical select_drf_curve_pos(char *prompt, UF_DRF_object_p_t curve,
    double cp[3])
{
    int
        resp;
    UF_CALL(UF_UI_set_cursor_view(0));
    UF_CALL(UF_UI_select_with_single_dialog(prompt, "",
        UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_curves, NULL, &resp,
        &curve->object_tag, cp, &curve->object_view_tag));
    UF_CALL(UF_UI_set_cursor_view(1));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        ask_pos_on_obj(curve->object_tag, cp);
        UF_CALL(UF_DISP_set_highlight(curve->object_tag, 0));
        return TRUE;
    }
    else return FALSE;
}
static logical select_pos(char *prompt, double pos[3])
{
    int
        resp = uc1615(prompt, pos);
    if (resp == 5) return TRUE;
    else return FALSE;
}
static void map_abs2wcs(double cc[3])
{
    double
        abs[9] = { 0,0,0, 1,0,0, 0,1,0 },
        wcs[12],
        mx[12];
    int
        irc;
    tag_t
        wcs_ent,
        wcs_mx;
    UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));
    UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));
    UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));
    FTN(uf5940)(abs, wcs, mx, &irc);
    FTN(uf5941)(cc, mx);
}
static void fix_it(double pos[3], tag_t member_view)
{
    double
        dwg_ref[3],
        dsize[2],
        factor = 1,
        mdl_ref[3],
        mx[9],
        scale,
        tmp_pos[3];
    tag_t
        dsp_prt_tag,
        pnt;
    int
        dcode,
        dunits,
        n,
        punits;
    char
        cur_dwg[MAX_ENTITY_NAME_SIZE] = { "" },
        member_view_name[MAX_ENTITY_NAME_SIZE];
    /* Get the view reference point and scale 译:获取视图参考点和缩放 */
    UF_CALL(uc5027(member_view, member_view_name, &n));
    UF_CALL(uc6442(member_view_name, mdl_ref, &scale));
    /* Get the drawing reference point 译:获取绘图参考点 */
    UF_CALL(uc6483(cur_dwg, member_view_name, dwg_ref));
    /* Get view rotation matrix 译:获取视图旋转矩阵 */
    UF_CALL(uc6433(member_view_name, mx));
    /* Get drawing units (1 = inches, 2 = mm) 译:获取绘图单位（1=英寸，2=毫米） */
    UF_CALL(uc6479(cur_dwg, &dunits, &dcode, dsize));
    if (dunits == 2) factor = 25.4;
    /* Get part units (1 = mm, 2 = inches) 译:获取部件单位（1 = 毫米，2 = 英寸） */
    dsp_prt_tag = UF_PART_ask_display_part( );
    UF_CALL(UF_PART_ask_units( dsp_prt_tag, &punits ));
    if (punits == 1) factor = factor / 25.4;
    /* Create a point on the drawing 译:在绘图上创建一个点。 */
    UF_CALL(UF_CURVE_create_point(pos, &pnt));
    /* Convert to drawing units 译:将单位转换为绘图单位 */
    UF_VEC3_scale(factor, pos, pos);
    /* Add the drawing reference point 译:Add the drawing reference point。 */
    UF_VEC3_add(pos, dwg_ref, pos);
    /* Convert back to part units 译:翻译为：转换回部件单位。 */
    UF_VEC3_scale(1/factor, pos, pos);
    /* Map through the transform of the view rotation matrix 译:通过视图旋转矩阵变换的映射 */
    UF_MTX3_vec_multiply_t(pos, mx, tmp_pos);
    /* Scale down by the view's drawing scale 译:缩小视图的绘制比例。 */
    UF_VEC3_scale(1/scale, tmp_pos, pos);
    /* Add the view reference point 译:将视图参考点添加到场景中 */
    UF_VEC3_add(pos, mdl_ref, pos);
    /* Create a point in the member view 译:在成员视图中创建一个点 */
    UF_CALL(UF_CURVE_create_point(pos, &pnt));
    UF_CALL(uc6401("", pnt));
    UF_CALL(uc6402(member_view_name, pnt));
}
static void do_it(void)
{
    char
        id_text[MAX_ID_SYM_TEXT_LENGTH + 1],
        label_text[1][MAX_LINE_LENGTH + 1];
    double
        cp[3],
        pos[3];
    int
        i = 0;
    tag_t
        id,
        label;
    UF_DRF_object_t
        curve;
    UF_CALL(UF_DRF_init_object_structure(&curve));
    while (select_drf_curve_pos("Select item to label", &curve, cp))
    {
        if (select_pos("Indicate origin for label", pos))
        {
            map_abs2wcs(pos);  /* required if view is model view 译:如果视图是模型视图，则需要翻译。 */
            sprintf(&label_text[0][0], "Label %d", ++i);
            sprintf(id_text, "ID %d", i);
            UF_CALL(UF_DRF_create_label(1, label_text, pos,
                UF_DRF_leader_attach_object, &curve, cp, &label));
            UF_CALL(UF_DRF_create_id_symbol(UF_DRF_sym_circle, id_text, "",
                pos, UF_DRF_with_leader, UF_DRF_leader_attach_object, &curve,
                cp, &id));
            fix_it(cp, curve.object_view_tag);  /* may help on dwg 译:may help on dwg的翻译是“可能有助于dwg文件”。 */
            sprintf(&label_text[0][0], "Label %d", ++i);
            sprintf(id_text, "ID %d", i);
            UF_CALL(UF_DRF_create_label(1, label_text, pos,
                UF_DRF_leader_attach_object, &curve, cp, &label));
            UF_CALL(UF_DRF_create_id_symbol(UF_DRF_sym_circle, id_text, "",
                pos, UF_DRF_with_leader, UF_DRF_leader_attach_object, &curve,
                cp, &id));
        }
    }
}
void ufusr(char *param, int *retcode, int paramLen)
{
    if (!UF_CALL(UF_initialize()))
    {
        do_it();
        UF_CALL(UF_terminate());
    }
}
int ufusr_ask_unload(void)
{
    return (UF_UNLOAD_IMMEDIATELY);
}

```

#### 代码解析

> 这段代码实现了在NX CAD软件中创建曲线标签和ID符号的功能。
>
> 主要功能包括：
>
> 1. 选择要添加标签的曲线。
> 2. 输入标签原点位置。
> 3. 在指定位置创建标签和ID符号，并附着在曲线上。
> 4. 调整视图变换，以适应不同视图和单位。
> 5. 重复执行，直到用户取消。
>
> 主要技术点：
>
> 1. 使用UF_DRF模块创建标签和ID符号。
> 2. 使用UF_UI模块进行用户交互。
> 3. 使用UF_MODL模块计算曲线上的最近点。
> 4. 使用视图变换矩阵进行视图转换。
> 5. 使用UF_VEC模块进行向量运算。
> 6. 使用UF_CURVE模块创建点。
> 7. 使用UF_PART模块获取当前部件信息。
> 8. 使用UF_CSYS模块获取坐标系信息。
>
> 该代码实现了在NX CAD软件中自动添加曲线标签和ID符号的功能，提高了设计效率。
>
