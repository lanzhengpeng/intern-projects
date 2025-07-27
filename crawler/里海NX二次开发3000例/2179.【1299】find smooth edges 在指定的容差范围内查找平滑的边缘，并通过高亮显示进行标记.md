### 【1299】find smooth edges 在指定的容差范围内查找平滑的边缘，并通过高亮显示进行标记

#### 代码

```cpp
/*HEAD FIND_SMOOTH_EDGES CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_obj.h>
#include <uf_disp.h>
#include <uf_modl.h>
#include <uf_object_types.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，我翻译如下：

UF_print_syslog 是 V18 版本新增的函数。 */
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
static tag_t select_a_face(char *prompt)
{
    double
        cp[3];
    int
        irc;
    tag_t
        face,
        view;
    UF_UI_selection_options_t
        opts;
    UF_UI_mask_t
        mask;
    opts.num_mask_triples = 1;
    opts.mask_triples = &mask;
    opts.scope = UF_UI_SEL_SCOPE_WORK_PART;
    mask.object_type = UF_solid_type;
    mask.object_subtype = 0;
    mask.solid_type = UF_UI_SEL_FEATURE_ANY_FACE;
    UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &face, cp, &view));
    if (irc == 4 || irc == 5)
    {
        UF_CALL(UF_DISP_set_highlight(face, FALSE));
        return face;
    }
    else return NULL_TAG;
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
    strcpy(&menu[0][0], item);
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
        ii,
        n_edges,
        n_faces;
    tag_t
        edge,
        face;
    uf_list_p_t
        edge_list,
        face_list;
    double
        tolerance = .5;
    logical
        smooth;
    while (((face = select_a_face("Select a face")) != NULL_TAG) &&
            (prompt_for_a_number("Smoothness", "tolerance", &tolerance)))
    {
        UF_CALL(UF_MODL_ask_face_edges(face, &edge_list));
        UF_CALL(UF_MODL_ask_list_count(edge_list, &n_edges));
        for (ii = 0; ii < n_edges; ii++)
        {
            UF_CALL(UF_MODL_ask_list_item(edge_list, ii, &edge));
            UF_CALL(UF_MODL_ask_edge_faces(edge, &face_list));
            UF_CALL(UF_MODL_ask_list_count(face_list, &n_faces));
            if (n_faces == 2)
            {
                UF_CALL(UF_MODL_ask_edge_smoothness(edge, tolerance, &smooth));
                if (smooth) UF_CALL(UF_DISP_set_highlight(edge, TRUE));
            }
            UF_CALL(UF_MODL_delete_list(&face_list));
        }
        UF_CALL(UF_MODL_delete_list(&edge_list));
    }
    UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));
}
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

> 这段代码是NX的二次开发代码，主要功能是选择一个面，然后高亮显示该面上所有平滑的边。
>
> 主要功能包括：
>
> 1. 使用UF_MODL_ask_face_edges函数获取面上的所有边。
> 2. 使用UF_MODL_ask_edge_faces函数判断边连接的两个面。
> 3. 如果边连接两个面，则使用UF_MODL_ask_edge_smoothness函数判断边的平滑性。
> 4. 如果边平滑，则使用UF_DISP_set_highlight函数高亮显示。
> 5. 重复选择面和平滑性判断，直到用户取消选择。
> 6. 使用UF_PART_cleanup函数清理高亮显示。
>
> 代码中包含错误处理和日志输出功能。作者使用了UF_CALL宏来简化错误处理，并在出错时输出日志。此外，作者还使用了UF_print_syslog函数输出日志，这是NX18版本新增的函数。
>
> 总体来说，这段代码实现了一个实用的二次开发功能，采用了NX提供的API，并且包含了完善的错误处理和日志输出功能。
>
