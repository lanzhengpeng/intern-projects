### 【0660】create curve mesh 创建曲面网格

#### 代码

```cpp
/*HEAD CREATE_CURVE_MESH CCC UFUN */
#include <stdio.h>
#include <uf.h>
#include <uf_disp.h>
#include <uf_ui.h>
#include <uf_modl.h>
#include <uf_object_types.h>
#include <uf_part.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中的新功能。 */
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
static tag_t select_a_curve(char *prompt)
{
    double
        cp[3];
    int
        resp;
    tag_t
        curve,
        view;
    UF_UI_selection_options_t
        opts;
    UF_UI_mask_t
        mask[5];
    opts.num_mask_triples = 5;
    opts.mask_triples = mask;
    opts.scope = UF_UI_SEL_SCOPE_WORK_PART;
    mask[0].object_type = UF_line_type;
    mask[0].object_subtype = 0;
    mask[0].solid_type = 0;
    mask[1].object_type = UF_circle_type;
    mask[1].object_subtype = 0;
    mask[1].solid_type = 0;
    mask[2].object_type = UF_conic_type;
    mask[2].object_subtype = UF_all_subtype;
    mask[2].solid_type = 0;
    mask[3].object_type = UF_spline_type;
    mask[3].object_subtype = 0;
    mask[3].solid_type = 0;
    mask[4].object_type = UF_point_type;
    mask[4].object_subtype = 0;
    mask[4].solid_type = 0;
    UF_CALL(UF_UI_select_single(prompt, &opts, &resp, &curve, cp, &view));
    if (resp == 4 || resp == 5)
    {
        UF_CALL(UF_DISP_set_highlight(curve, 0));
        return curve;
    }
    else return NULL_TAG;
}
static void do_it(void)
{
    UF_STRING_t
        primary,
        cross,
        spine;
    int
        i,
        c_flag[4],
        end_point = 0,
        emphasis = 3,
        body_type = 0,
        spln_pt = 0;
    UF_FEATURE_SIGN
        boolean = UF_NULLSIGN;
    double
        tol[3];
    tag_t
        body_eid,
        c_face[4];
    tol[0] = .001;
    tol[1] = 0.5;
    tol[2] = 0.02;
    UF_MODL_init_string_list(&primary);
    UF_MODL_init_string_list(&cross);
    UF_MODL_init_string_list(&spine);
    UF_MODL_create_string_list(2,2,&primary); /* 2 strings with 1 curve each 译:对不起，我无法完成这项任务，因为内容已被删除。 */
    UF_MODL_create_string_list(2,2,&cross);
    primary.num = 2; /* number of strings for primary curve 译:主曲线字符串数量 */
    primary.dir[0] = 1;
    primary.dir[1] = 1;
    primary.string[0] = 1; /* number of entities in the first prim string 译:第一原字符串中的实体数量 */
    primary.string[1] = 1; /* number of entities in the second prim string 译:第二主字符串中的实体数量 */
    cross.num = 2; /* number of cross strings 译:跨弦数 */
    cross.dir[0] = 1;
    cross.dir[1] = 1;
    cross.string[0] = 1; /* number of ents in the first cross string 译:第一条交叉弦中的实体数量 */
    cross.string[1] = 1; /* number of ents in the second cross string 译:第二个十字形字符串中的实体数量 */
    for (i=0; i<4; i++)
    {
        c_face[i] = 0;
        c_flag[i] = 0;
    }
    primary.id[0] = select_a_curve("Select first primary curve");
    primary.id[1] = select_a_curve("Select second primary curve");
    cross.id[0] = select_a_curve("Select first cross curve");
    cross.id[1] = select_a_curve("Select second cross curve");
    UF_CALL(UF_MODL_create_curve_mesh(&primary, &cross, &spine, &end_point,
        &emphasis,&body_type,&spln_pt,boolean,tol,c_face,c_flag,&body_eid));
    UF_MODL_free_string_list(&primary);
    UF_MODL_free_string_list(&cross);
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

> 这段代码是一个NX的二次开发程序，主要功能是创建曲线网格。代码流程如下：
>
> 1. 引入了必要的NX API头文件。
> 2. 定义了一个错误报告函数，用于捕获函数调用错误。
> 3. 定义了一个选择曲线的函数，用于提示用户选择一个曲线，并返回曲线的tag。
> 4. 定义了主函数do_it()，用于执行创建曲线网格的操作。
> 5. 在do_it()中，初始化了主曲线、交叉曲线和脊柱曲线的字符串列表。
> 6. 提示用户选择4条曲线，并分别添加到主曲线和交叉曲线的字符串列表中。
> 7. 调用UF_MODL_create_curve_mesh()函数，传入曲线列表、端点、重点等参数，创建曲线网格。
> 8. 释放曲线列表占用的内存。
> 9. 定义了ufusr()函数作为程序的入口，在NX启动时调用。在ufusr()中，初始化NX环境，执行do_it()，然后终止NX环境。
> 10. 定义了ufusr_ask_unload()函数，在程序卸载时调用，立即卸载程序。
>
> 总体来说，这段代码通过二次开发实现了在NX中创建曲线网格的功能，通过交互式选择曲线作为输入，并调用NX的建模API完成曲线网格的创建。
>
