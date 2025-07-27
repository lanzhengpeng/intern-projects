### 【0747】create labels and idsymbols 创建标签和ID符号

#### 代码

```cpp
#include <stdio.h>
#include <stdarg.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_obj.h>
#include <uf_object_types.h>
#include <uf_disp.h>
#include <uf_drf.h>
#include <uf_csys.h>
#include <uf_trns.h>
#include <uf_curve.h>
#include <uf_view.h>
#include <uf_modl.h>
#include <uf_vec.h>
#include <uf_part.h>
#include <uf_draw.h>
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
/* 里海 */
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
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_drf_curves, NULL, &resp,
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
static void do_it(void)
{
    int
        i = 0,
        mpi[100],
        part_units;
    UF_VIEW_type_t
        vtype;
    UF_VIEW_subtype_t
        vstype;
    tag_t
        id,
        label,
        part = UF_PART_ask_display_part(),
        work_view;
    double
        cp[3],
        dp[3],
        mpr[70],
        pos[3],
        sf[2] = { 1/25.4, 25.4 };
    char
        dia[27],
        id_text[MAX_ID_SYM_TEXT_LENGTH + 1],
        label_text[1][MAX_LINE_LENGTH + 1],
        rad[27];
    UF_DRF_object_t
        curve;
    UF_DRAW_info_t
        info;
    UF_CALL(UF_DRF_ask_preferences(mpi, mpr, rad, dia));
    UF_CALL(UF_DRF_init_object_structure(&curve));
    while (select_drf_curve_pos("Select item to label", &curve, cp))
    {
        if (select_pos("Indicate origin for label", pos))
        {
            map_abs2wcs(pos);  /* required if view is model view 译:如果视图是模型视图，则需要。 */
            UF_CALL(UF_VIEW_ask_work_view(&work_view));
            UF_CALL(UF_VIEW_ask_type(curve.object_view_tag, &vtype, &vstype));
            if ((work_view != curve.object_view_tag) &&
                (vtype == UF_VIEW_DRAWING_MEMBER_TYPE))
            {
                UF_CALL(UF_VIEW_map_model_to_drawing(curve.object_view_tag,
                    cp, dp));
            /*  In case drawing and part units do not match 译:如果图纸和部件单位不匹配 */
                UF_CALL(UF_DRAW_ask_drawing_info(NULL_TAG, &info));
                UF_CALL(UF_PART_ask_units(part, &part_units));
                if (info.units != part_units)
                    UF_VEC3_scale(sf[2-part_units], dp, dp);
            }
            else
            {
                UF_VEC2_copy(cp, dp);
            }
            if (dp[0] < pos[0])
                mpi[1] = 1;
            else
                mpi[1] = 2;
            UF_CALL(UF_DRF_set_preferences(mpi, mpr, rad, dia));
            sprintf(&label_text[0][0], "Label %d", ++i);
            UF_CALL(UF_DRF_create_label(1, label_text, pos,
                UF_DRF_leader_attach_object, &curve, cp, &label));
            sprintf(id_text, "ID %d", i);
            UF_CALL(UF_DRF_create_id_symbol(UF_DRF_sym_circle, id_text, "",
                pos, UF_DRF_with_leader, UF_DRF_leader_attach_object, &curve,
                cp, &id));
        }
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

> 这段代码是一个NX Open C++的二次开发示例，实现了在NX中自动添加标签和ID符号到用户选择的对象上的功能。代码的主要功能和流程如下：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了两个坐标转换函数map_abs2view和map_view2abs，用于绝对坐标系和视图坐标系的转换。
> 3. 定义了获取对象上的点的函数ask_pos_on_obj，用于获取用户指定点在对象上的精确位置。
> 4. 定义了选择函数select_drf_curve_pos，用于让用户选择要添加标签的对象和位置。
> 5. 定义了主函数do_it，其中通过循环调用选择函数，获取用户选择的对象和位置，然后在该位置添加标签和ID符号。
> 6. 定义了主入口函数ufusr，用于初始化和调用主函数，以及结束程序。
> 7. 定义了卸载函数ufusr_ask_unload，用于立即卸载程序。
>
> 总的来说，这段代码实现了用户交互式的在NX模型中选择对象并添加标签和ID符号的功能，通过二次开发扩展了NX的功能。
>
