### 【0894】create points at corners of box bounding selected faceted body 在选定的实体上创建盒子边界角点上的点

#### 代码

```cpp
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_csys.h>
#include <uf_facet.h>
#include <uf_curve.h>
#include <uf_trns.h>
#include <uf_vec.h>
#include <uf_modl.h>
#include <uf_undo.h>
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
/* 里海 */
static int mask_for_faceted_bodies(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_faceted_model_type, UF_all_subtype, 0 };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static tag_t select_a_faceted_body(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog("Select faceted body", prompt,
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faceted_bodies, NULL, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, 0));
        return object;
    }
    else return NULL_TAG;
}
static void map_point_to_csys(tag_t csys, double *pos)
{
    int
        irc;
    tag_t
        matrix;
    double
        abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },
        csys_mx[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},
        mx[12],
        origin[3];
    UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));
    UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys_mx[3]));
    FTN(uf5940)(abs_mx, csys_mx, mx, &irc);
    FTN(uf5941)(pos, mx);
}
static void map_point_from_csys(tag_t csys, double *pos)
{
    int
        irc;
    tag_t
        matrix;
    double
        abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },
        csys_mx[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},
        mx[12],
        origin[3];
    UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));
    UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys_mx[3]));
    FTN(uf5940)(csys_mx, abs_mx, mx, &irc);
    FTN(uf5941)(pos, mx);
}
static int allocate_memory(unsigned int nbytes, void **where)
{
    int
        resp;
    *where = UF_allocate_memory(nbytes, &resp);
    return resp;
}
static void find_facet_extremes(tag_t model, tag_t csys, double *llc,
    double *urc, logical *first)
{
    int
        ii,
        facet_id = UF_FACET_NULL_FACET_ID,
        max_verts,
        n;
    double
        point[3],
        *verts;
    if (!(*first))
    {
        map_point_to_csys(csys, llc);
        map_point_to_csys(csys, urc);
    }
    UF_CALL(UF_FACET_ask_max_facet_verts(model, &max_verts));
    UF_CALL(allocate_memory(max_verts * 3 * sizeof(double), (void **)&verts));
    while (!UF_CALL(UF_FACET_cycle_facets(model, &facet_id)) &&
        (facet_id != UF_FACET_NULL_FACET_ID))
    {
        UF_CALL(UF_FACET_ask_vertices_of_facet(model, facet_id, &n,
            (double (*)[3]) verts));
        for (ii = 0; ii < n; ii++)
        {
            UF_VEC3_copy(&verts[ii*3], point);
            map_point_to_csys(csys, point);
            if (*first)
            {
                UF_VEC3_copy(point, llc);
                UF_VEC3_copy(point, urc);
                *first = FALSE;
            }
            else
            {
                if ( point[0] > urc[0] ) urc[0] = point[0];
                if ( point[1] > urc[1] ) urc[1] = point[1];
                if ( point[2] > urc[2] ) urc[2] = point[2];
                if ( point[0] < llc[0] ) llc[0] = point[0];
                if ( point[1] < llc[1] ) llc[1] = point[1];
                if ( point[2] < llc[2] ) llc[2] = point[2];
            }
        }
    }
    UF_free(verts);
    map_point_from_csys(csys, llc);
    map_point_from_csys(csys, urc);
    return;
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
    WRITE3F(llc);
    WRITE3F(urc);
    UF_VEC3_copy(llc, start);
    map_point_to_csys(csys, start);
    UF_VEC3_copy(urc, end);
    map_point_to_csys(csys, end);
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
static UF_UNDO_mark_id_t set_undo_mark(char *label)
{
    UF_UNDO_mark_id_t
        mark_id;
    UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));
    return mark_id;
}
static logical ask_aligned_or_exact_aligned_bounding_box(
    tag_t object, tag_t csys, double *llc, double *urc, int which)
{
    int
        ii,jj;
    double
        dir[3][3],
        dist[3];
        if (which == 1)
        {
            if (UF_MODL_ask_bounding_box_aligned(object, csys, false, llc,
                dir, dist )) return false;
        }
        else
        {
            if (UF_MODL_ask_bounding_box_exact(object, csys, llc, dir, dist ))
                return false;
        }
        for (ii = 0; ii < 3; ii++)
        {
            urc[ii] = llc[ii];
            for (jj = 0; jj < 3; jj++)
                urc[ii] += dir[jj][ii] * dist[jj];
        }
        return true;
}
static void do_it(void)
{
    logical
        first = TRUE;
    tag_t
        abs_csys,
        matrix,
        model,
        point,
        wcs;
    double
        abs[] = { 1,0,0, 0,1,0, 0,0,1 },
        box[6],
        urc[3],
        llc[3],
        zero[] = { 0, 0, 0 };
    set_undo_mark("Create extreme points");
    while ((model = select_a_faceted_body("Create extreme points")) != NULL_TAG)
    {
    //  Use the method if a rough sized ABS oriented box is OK
        ECHO("%s:\n", "Using UF_MODL_ask_bounding_box");
        UF_CALL(UF_MODL_ask_bounding_box(model, box));
        UF_CALL(UF_CSYS_create_matrix(abs, &matrix));
        UF_CALL(UF_CSYS_create_temp_csys(zero, matrix, &abs_csys));
        display_temporary_box(box, &box[3], abs_csys);
        UF_CALL(UF_CURVE_create_point(box, &point));
        UF_CALL(UF_CURVE_create_point(&box[3], &point));
    // Use this method if you need an exact sized box aligned to another csys
        ECHO("%s:\n", "Using find_facet_extremes with the WCS");
        UF_CALL(UF_CSYS_ask_wcs(&wcs));
        first = TRUE;
        find_facet_extremes(model, wcs, llc, urc, &first);
        display_temporary_box(llc, urc, wcs);
        UF_CALL(UF_CURVE_create_point(llc, &point));
        UF_CALL(UF_CURVE_create_point(urc, &point));
    // for the record this does not work.
        if (ask_aligned_or_exact_aligned_bounding_box(model, wcs, llc, urc, 1))
        {
            UF_DISP_refresh();
            display_temporary_box(llc, urc, wcs);
            UF_CALL(UF_CURVE_create_point(llc, &point));
            UF_CALL(UF_CURVE_create_point(urc, &point));
            uc1601("Using UF_MODL_ask_bounding_box_aligned with the WCS", TRUE);
        }
    // for the record this does not work either.
        if (ask_aligned_or_exact_aligned_bounding_box(model, wcs, llc, urc, 2))
        {
            UF_DISP_refresh();
            display_temporary_box(llc, urc, wcs);
            UF_CALL(UF_CURVE_create_point(llc, &point));
            UF_CALL(UF_CURVE_create_point(urc, &point));
            uc1601("Using UF_MODL_ask_bounding_box_exact with the WCS", TRUE);
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于输出函数调用失败时的错误信息。
> 2. 选择面体函数：定义了一个选择面体的函数select_a_faceted_body，可以弹出对话框让用户选择一个面体。
> 3. 坐标系转换函数：定义了坐标系转换函数map_point_to_csys和map_point_from_csys，用于将点从一个坐标系转换到另一个坐标系。
> 4. 内存分配函数：定义了内存分配函数allocate_memory。
> 5. 面体极点查找函数：定义了面体极点查找函数find_facet_extremes，用于遍历面体的所有面，找出最小和最大点。
> 6. 显示临时框函数：定义了显示临时框函数display_temporary_box，用于在图形窗口显示一个临时框。
> 7. 设置撤销标记函数：定义了设置撤销标记函数set_undo_mark。
> 8. 主函数：定义了主函数do_it，其中首先使用UF_MODL_ask_bounding_box获取面体的对齐包围盒，然后显示临时框。接着使用find_facet_extremes获取精确的包围盒，再次显示临时框。最后尝试使用UF_MODL_ask_bounding_box_aligned和UF_MODL_ask_bounding_box_exact获取对齐和精确的包围盒，但这两个函数调用未能成功。
> 9. ufusr主函数：定义了ufusr主函数，其中初始化NX环境，调用do_it执行主逻辑，然后终止NX环境。
>
> 总体来说，该代码通过NX的API实现了选择面体、坐标系转换、内存分配、面体极点查找、显示临时框等功能，并展示了获取面体包围盒的不同方法。
>
