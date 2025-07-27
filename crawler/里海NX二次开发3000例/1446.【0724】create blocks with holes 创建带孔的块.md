### 【0724】create blocks with holes 创建带孔的块

#### 代码

```cpp
/*HEAD CREATE_BLOCKS_WITH_HOLES CCC UFUN */
#include <stdio.h>
#include <stdlib.h>
#include <uf.h>
#include <uf_object_types.h>
#include <uf_part.h>
#include <uf_ui.h>
#include <uf_curve.h>
#include <uf_disp.h>
#include <uf_modl.h>
#include <uf_obj.h>
#include <uf_vec.h>
#include <uf_trns.h>
#include <uf_view.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */
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
static void ask_face_dir(double *loc, tag_t face, double *z_dir)
{
    double
        parm[2],
        pnt[3],
        dirU2[3],
        dirV1[3],
        dirV2[3],
        radii[2],
        x_dir[3];
    UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pnt));
    UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, x_dir, dirV1, dirU2,
        dirV2, z_dir, radii));
    UF_VEC3_negate(z_dir, z_dir);
}
static void map_view2abs(double *c)
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
    UF_CALL( irc );
    FTN(uf5941)(c, mx);
}
static tag_t select_placement_face(char *prompt, double *loc, double *z_dir)
{
    int
        resp;
    tag_t
        face,
        line,
        view;
    double
        dist;
    UF_CURVE_line_t
        lp;
    UF_UI_mask_t
        mask;
    UF_UI_selection_options_t
        opts;
    opts.num_mask_triples = 1;
    opts.mask_triples = &mask;
    opts.scope = UF_UI_SEL_SCOPE_WORK_PART;
    mask.object_type = UF_solid_type;
    mask.object_subtype = 0;
    mask.solid_type = UF_UI_SEL_FEATURE_PLANAR_FACE;
    UF_CALL(UF_UI_select_single(prompt, &opts, &resp, &face, loc, &view));
    if (resp == 4 || resp == 5)
    {
        UF_CALL(UF_DISP_set_highlight(face, FALSE));
        lp.start_point[0] = loc[0];
        lp.start_point[1] = loc[1];
        lp.start_point[2] = loc[2] + 1000;
        lp.end_point[0] = loc[0];
        lp.end_point[1] = loc[1];
        lp.end_point[2] = loc[2] - 1000;
        map_view2abs(lp.start_point);
        map_view2abs(lp.end_point);
        UF_CALL(UF_CURVE_create_line(&lp, &line));
        UF_CALL(UF_MODL_ask_minimum_dist(face,line,0,loc,0,loc,&dist,loc,loc));
        UF_CALL(UF_OBJ_delete_object(line));
        ask_face_dir(loc, face, z_dir);
        return face;
    }
    else return NULL_TAG;
}
static int prompt_hole_parms( char *angle, char *depth, char *dia)
{
    char
        menu[3][16] = { "diameter", "depth", "tip angle" };
    double
        values[3];
    int
        irc,
        junk;
    sscanf(dia, "%lf", &values[0]);
    sscanf(depth, "%lf", &values[1]);
    sscanf(angle, "%lf", &values[2]);
    irc = uc1609("Enter simple hole parameters", menu, 3, values, &junk);
    if (irc == 3 || irc == 4)
    {
        sprintf(dia, "%f", values[0]);
        sprintf(depth, "%f", values[1]);
        sprintf(angle, "%f", values[2]);
        return TRUE;
    }
    else return FALSE;
}
static void create_simple_holes(void)
{
    char
        angle[20] = { "0.0" },
        depth[20] = { "1.0" },
        dia[20] = { "1.0" };
    double
        loc[3],
        z_dir[3];
    tag_t
        face,
        hole;
    while ((face = select_placement_face("Select face for hole", loc, z_dir)) &&
           (prompt_hole_parms(angle, depth, dia)))
    {
        UF_CALL(UF_MODL_create_simple_hole(loc, z_dir, dia, depth, angle,
            face, NULL_TAG, &hole));
    }
}
static int prompt_block_parms(char *sides[3])
{
    char
        menu[3][16] = { "height", "width", "length" };
    double
        values[3];
    int
        junk,
        i,
        irc;
    for (i=0; i<3; i++) sscanf(sides[i], "%lf", &values[i]);
    irc = uc1609("Enter block parameters", menu, 3, values, &junk);
    if (irc == 3 || irc == 4)
    {
        for (i=0; i<3; i++) sprintf(sides[i], "%f", values[i]);
        return TRUE;
    }
    else return FALSE;
}
static void create_blocks(void)
{
    char
        *sides[3];
    double
        corner[3];
    int
        i,
        sel[2] = { 0, 0 };
    tag_t
        block;
    for (i=0; i<3; i++) sides[i] = (char *)calloc(20, sizeof(char));
    for (i=0; i<3; i++) sprintf(sides[i], "%f", 1.0);
    while ((prompt_block_parms(sides)) &&
          ((uc1616("Select corner point", sel, 0, corner)) == 5))
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, corner, sides, &block));
}
static void do_it(void)
{
    create_blocks();
    create_simple_holes();
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

> 这段NX二次开发代码的主要功能是创建带孔的方块。具体来说，代码实现了以下功能：
>
> 1. 包含了常用的NX二次开发头文件，定义了错误报告函数report_error。
> 2. 定义了获取面法向量的函数ask_face_dir。
> 3. 定义了将视图坐标转换为绝对坐标的函数map_view2abs。
> 4. 定义了选择放置孔的面并获取孔参数的函数select_placement_face。
> 5. 定义了提示输入孔参数的函数prompt_hole_parms。
> 6. 定义了创建简单孔的函数create_simple_holes。
> 7. 定义了提示输入方块参数的函数prompt_block_parms。
> 8. 定义了创建方块的函数create_blocks。
> 9. 主函数do_it依次调用create_blocks和create_simple_holes，实现创建带孔方块的功能。
> 10. ufusr是NX二次开发的标准入口函数，初始化后调用do_it，最后结束初始化。
> 11. ufusr_ask_unload声明了卸载函数，表示立即卸载。
>
> 综上所述，这段代码通过用户交互方式实现了在NX中创建带孔方块的功能，涉及了NX的几何建模、参数化、选择等二次开发接口。
>
