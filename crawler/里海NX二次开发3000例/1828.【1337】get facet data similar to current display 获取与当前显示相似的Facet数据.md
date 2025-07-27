### 【1337】get facet data similar to current display 获取与当前显示相似的Facet数据

#### 代码

```cpp
/*HEAD GET_FACET_DATA_SIMILAR_TO_CURRENT_DISPLAY CCC UFUN */
/*
   This application demonstrates how to obtain facet data with resolution which
   is roughly the same as see when you turn on Preferences-> Visualization->
   Faceting-> Show Facet Edges.
*/
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_obj.h>
#include <uf_object_types.h>
#include <uf_facet.h>
#include <uf_view.h>
#include <uf_undo.h>
#include <uf_part.h>
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
static tag_t ask_next_visible_solid(tag_t body)
{
    int
        subtype,
        type;
    while (!UF_CALL(UF_VIEW_cycle_objects(NULL_TAG, UF_VIEW_VISIBLE_OBJECTS,
            &body)) && (body != NULL_TAG))
    {
        UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));
        if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype))
            return body;
    }
    return NULL_TAG;
}
static void display_facet(tag_t model, int facet)
{
    int
        ii,
        n;
    double
        verts[5][3];
    UF_OBJ_disp_props_t
        props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,
            UF_OBJ_FONT_SOLID, FALSE};
    UF_CALL(UF_FACET_ask_vertices_of_facet(model, facet, &n, verts));
    for (ii = 0; ii < (n - 1); ii++)
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG,
            UF_DISP_USE_WORK_VIEW, &verts[ii][0], &verts[ii+1][0], &props));
    UF_CALL(UF_DISP_display_temporary_line(NULL_TAG,
        UF_DISP_USE_WORK_VIEW, &verts[n-1][0], &verts[0][0], &props));
}
#define WRITE_F(X) (write_double_to_listing_window(#X, X))
static void write_double_to_listing_window(char *title, double n)
{
    char
        msg[UF_UI_MAX_STRING_LEN+1];
    sprintf(msg, "%s = %f\n", title, n);
    ECHO(msg);
}
#define WRITE_D(X) (write_integer_to_listing_window(#X, X))
static void write_integer_to_listing_window(char *title, int n)
{
    char
        msg[UF_UI_MAX_STRING_LEN+1];
    sprintf(msg, "%s = %d\n", title, n);
    ECHO(msg);
}
static void do_it(void)
{
    int
        facet_cnt,
        facet = UF_FACET_NULL_FACET_ID,
        units;
    tag_t
        body = NULL_TAG,
        model,
        part = UF_PART_ask_display_part();
    double
        cf = 1,
        view_center[3],
        view_scale;
    UF_DISP_system_params_t
        sys_params;
    UF_FACET_parameters_t
        facet_params;
    UF_UNDO_mark_id_t
        mark_id;
    UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));
    UF_CALL(UF_DISP_ask_system_parameters(&sys_params));
    UF_CALL(uc6430("", view_center, &view_scale));
    UF_CALL(UF_PART_ask_units(part, &units));
    if (units == UF_PART_METRIC) cf = 25.4;
    UF_FACET_INIT_PARAMETERS(&facet_params);
    facet_params.specify_surface_tolerance = TRUE;
    facet_params.surface_dist_tolerance =
        cf * sys_params.facet_chord_tol/view_scale;
    facet_params.surface_angular_tolerance = sys_params.facet_angle_tol * DEGRA;
    facet_params.specify_curve_tolerance = TRUE;
    facet_params.curve_dist_tolerance =
        cf * sys_params.facet_edge_tol/view_scale;
    facet_params.curve_angular_tolerance = sys_params.facet_angle_tol * DEGRA;
    WRITE_F(facet_params.surface_dist_tolerance);
    WRITE_F(facet_params.surface_angular_tolerance);
    WRITE_F(facet_params.curve_dist_tolerance);
    WRITE_F(facet_params.curve_angular_tolerance);
    while ((body = ask_next_visible_solid(body)) != NULL_TAG)
    {
        UF_CALL(UF_FACET_facet_solid(body, &facet_params, &model));
        facet_cnt = 0;
        while (!UF_CALL(UF_FACET_cycle_facets(model, &facet))
            && (facet != UF_FACET_NULL_FACET_ID))
        {
            facet_cnt++;
            display_facet(model, facet);
        }
        WRITE_D(facet_cnt);
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));
    }
    UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));
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

> 这段代码是一个NX的二次开发示例，其主要功能是获取与当前显示效果相近的Facet数据。具体来说，代码的主要步骤包括：
>
> 1. 初始化和获取系统参数：代码首先进行NX的初始化，并获取系统参数，包括显示参数和当前显示部件的单位。
> 2. 设置Facet参数：根据系统参数，设置Facet参数，包括曲面和曲线的公差。这些参数决定了Facet数据的精细程度。
> 3. 循环遍历每个实体：代码循环遍历当前显示的每个实体，并对每个实体进行Faceting操作，获取Facet数据。
> 4. 显示Facet边：对每个实体的Facet数据进行循环遍历，并调用显示函数显示Facet边，以实现类似“显示Facet边”的效果。
> 5. 输出Facet数量：对每个实体的Facet数量进行输出。
> 6. 撤销操作：在每个实体Faceting后，进行撤销操作，以恢复原始状态。
> 7. 结束和卸载：代码执行完毕后，进行NX的结束和卸载操作。
>
> 总的来说，这段代码通过循环遍历当前显示的每个实体，获取与显示效果相近的Facet数据，并实现类似“显示Facet边”的效果。这为获取高质量Facet数据提供了参考。
>
