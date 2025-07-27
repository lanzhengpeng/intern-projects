### 【1070】display bounding box aligned 显示与坐标系对齐的边界框

#### 代码

```cpp
/*HEAD DISPLAY_BOUNDING_BOX_ALIGNED CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_curve.h>
#include <uf_vec.h>
#include <uf_csys.h>
#include <uf_modl.h>
#include <uf_obj.h>
#include <uf_disp.h>
#include <uf_part.h>
#include <uf_ui.h>
#include <uf_object_types.h>
#include <uf_trns.h>
#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
/* qq3123197280 */
static int report_error( char *file, int line, char *call, int irc)
{
    if (irc)
    {
        char err[133],
             msg[133];
        sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",
            irc, line, file);
        UF_get_fail_message(irc, err);
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
/* qq3123197280 */
static void map_abs2csys(tag_t input_csys, double vec[3])
{
    double
        abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },
        csys[12],
        mx[12];
    int
        irc;
    tag_t
        csys_mx;
    UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));
    UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));
    FTN(uf5940)(abs_mx, csys, mx, &irc);
    UF_CALL(irc);
    FTN(uf5941)(vec, mx);
    UF_VEC3_sub(vec, csys, vec);
}
/* qq3123197280 */
static void display_temporary_box(double llc[3], double urc[3], tag_t csys)
{
    double
        dirs[9],
        delta_x[3],
        delta_y[3],
        delta_z[3],
        end[3],
        mag,
        sizes[3],
        start[3];
    tag_t
        csys_mx;
    UF_OBJ_disp_props_t
        props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,
            UF_OBJ_FONT_SOLID, FALSE};
    UF_VEC3_copy(llc, start);
    map_abs2csys(csys, start);
    UF_VEC3_copy(urc, end);
    map_abs2csys(csys, end);
    UF_VEC3_sub(end, start, sizes);
    UF_CALL(UF_CSYS_ask_csys_info(csys, &csys_mx, dirs));
    UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, dirs));
    UF_VEC3_unitize(&dirs[0], 0, &mag, &dirs[0]);
    UF_VEC3_unitize(&dirs[3], 0, &mag, &dirs[3]);
    UF_VEC3_unitize(&dirs[6], 0, &mag, &dirs[6]);
    UF_VEC3_scale(sizes[0], &dirs[0], delta_x);
    UF_VEC3_scale(sizes[1], &dirs[3], delta_y);
    UF_VEC3_scale(sizes[2], &dirs[6], delta_z);
    UF_VEC3_add(llc, delta_x, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        llc, end, &props));
    UF_VEC3_add(llc, delta_y, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        llc, end, &props));
    UF_VEC3_add(llc, delta_z, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        llc, end, &props));
    UF_VEC3_sub(urc, delta_x, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        urc, end, &props));
    UF_VEC3_sub(urc, delta_y, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        urc, end, &props));
    UF_VEC3_sub(urc, delta_z, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        urc, end, &props));
    UF_VEC3_add(llc, delta_x, start);
    UF_VEC3_add(start, delta_y, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        start, end, &props));
    UF_VEC3_add(start, delta_z, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        start, end, &props));
    UF_VEC3_sub(end, delta_x, start);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        start, end, &props));
    UF_VEC3_add(start, delta_y, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        start, end, &props));
    UF_VEC3_sub(end, delta_z, start);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        start, end, &props));
    UF_VEC3_add(start, delta_x, end);
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,
        start, end, &props));
}
/* qq3123197280 */
static int mask_for_bodies(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
/* qq3123197280 */
static tag_t select_a_body(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, 0));
        return object;
    }
    else return NULL_TAG;
}
/* qq3123197280 */
static int mask_for_csys(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_coordinate_system_type, 0, 0 };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
/* qq3123197280 */
static tag_t select_a_csys(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",
        prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, 0));
        return object;
    }
    else return NULL_TAG;
}
/* qq3123197280 */
static void make_bbox_aligned( tag_t body, tag_t csys )
{
    int
        ii,jj;
    double
        min[3] = { 0., 0., 0. },
        dir[3][3] = { 0., 0., 0.,   0., 0., 0.,0.,  0., 0.,},
        dist[3] = { 0., 0., 0. },
        max[3] = { 0., 0., 0. };
    UF_CALL(UF_MODL_ask_bounding_box_aligned( body, csys, FALSE,
                                             min, dir, dist ));
    for (ii = 0; ii < 3; ii++)
    {
        max[ii] = min[ii];
        for (jj = 0; jj < 3; jj++)
            max[ii] += dir[jj][ii] * dist[jj];
    }
    display_temporary_box( min, max, csys);
}
static void do_it(void)
{
    tag_t
        body = NULL_TAG,
        csys = NULL_TAG;
    while ((body = select_a_body("BODY:"))!=NULL_TAG &&
            (csys = select_a_csys("CSYS:"))!=NULL_TAG)
    {
        UF_DISP_refresh();
        make_bbox_aligned( body, csys );
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

> 这段代码是用于在NX中显示与坐标系对齐的实体边界框的二次开发代码。
>
> 主要功能包括：
>
> 1. 提供错误报告函数，用于在函数调用失败时输出错误信息。
> 2. 实现将绝对坐标系下的点转换到指定坐标系的函数。
> 3. 实现显示临时线框盒子的函数。
> 4. 实现选择实体的函数，支持选择类型为实体的实体。
> 5. 实现选择坐标系的函数，支持选择类型为坐标系的坐标系。
> 6. 实现根据实体和坐标系，获取与坐标系对齐的边界框，并在坐标系下显示边界框的函数。
> 7. 在主函数中，循环调用选择实体和选择坐标系的函数，然后调用显示边界框的函数。
> 8. 初始化和终止NX的函数。
> 9. 提供卸载函数，用于卸载该代码。
>
> 总体来说，该代码实现了在NX中显示与坐标系对齐的实体边界框的功能。通过选择实体和坐标系，可以直观地查看实体在指定坐标系下的边界框。
>
