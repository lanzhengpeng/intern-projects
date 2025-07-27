### 【1729】perform body geometry checks 执行实体几何检查

#### 代码

```cpp
/*HEAD PERFORM_BODY_GEOMETRY_CHECKS CCC UFUN */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_modl.h>
#include <uf_disp.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog 是 V18 新增的功能。 */
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
static tag_t select_a_body(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog(prompt, "",
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, 0));
        return object;
    }
    else return NULL_TAG;
}
static void set_highlight_object_array(int n, tag_t *objects, int flag)
{
    int ii;
    for (ii = 0; ii < n; ii++)
        UF_CALL(UF_DISP_set_highlight(objects[ii], flag));
}
static void do_body_geometry_checks(tag_t body)
{
    int
        body_type,
        *fault_tokens,
        ii, jj,
        n_tags,
        n_boundaries,
        *num_edges;
    tag_t
        *edge_tags,
        *fault_tags;
    char
        msg[133];
    UF_CALL(UF_MODL_ask_body_structures(body, &n_tags, &fault_tags));
    if (n_tags > 0)
    {
        set_highlight_object_array(n_tags, fault_tags, TRUE);
        sprintf(msg, "%d faces/edges failed data structures check", n_tags);
        uc1601(msg, TRUE);
        set_highlight_object_array(n_tags, fault_tags, FALSE);
        UF_free(fault_tags);
    }
    else
        uc1601("all faces/edges pass data structures check", TRUE);
    UF_CALL(UF_MODL_ask_body_consistency(body, &n_tags, &fault_tokens,
        &fault_tags));
    if (n_tags > 0)
    {
        for (ii = 0; ii < n_tags; ii++)
        {
            UF_CALL(UF_DISP_set_highlight(fault_tags[ii], TRUE));
            UF_get_fail_message(fault_tokens[ii], msg);
            uc1601(msg, TRUE);
            UF_CALL(UF_DISP_set_highlight(fault_tags[ii], FALSE));
        }
        UF_free(fault_tokens);
        UF_free(fault_tags);
    }
    else
        uc1601("all faces/edges pass consistency check", TRUE);
    UF_CALL(UF_MODL_ask_face_face_intersect(body, &n_tags, &fault_tags));
    if (n_tags > 0)
    {
        set_highlight_object_array(n_tags, fault_tags, TRUE);
        sprintf(msg, "%d faces have a face to face intersection", n_tags);
        uc1601(msg, TRUE);
        set_highlight_object_array(n_tags, fault_tags, FALSE);
        UF_free(fault_tags);
    }
    else
        uc1601("no face to face intersections found", TRUE);
    UF_CALL(UF_MODL_ask_body_type(body, &body_type));
    if (body_type == UF_MODL_SHEET_BODY)
    {
        UF_CALL(UF_MODL_ask_body_boundaries(body, &n_boundaries, &num_edges,
            &edge_tags));
        if (n_boundaries > 0)
        {
            jj = 0;
            for (ii = 0; ii < n_boundaries; ii++)
            {
                set_highlight_object_array(num_edges[ii],&edge_tags[jj],TRUE);
                sprintf(msg, "boundary %d has %d edges", ii+1, num_edges[ii]);
                uc1601(msg, TRUE);
                set_highlight_object_array(num_edges[ii],&edge_tags[jj],FALSE);
                jj = jj + num_edges[ii];
            }
            UF_free(num_edges);
            UF_free(edge_tags);
        }
        else
            uc1601("no sheet boundaries found", TRUE);
    }
    else
        uc1601("not a sheet body", TRUE);
}
static void do_it(void)
{
    tag_t
        body;
    while ((body = select_a_body("Select body")) != NULL_TAG)
        do_body_geometry_checks(body);
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

> 这段代码是一个用于NX CAD系统的二次开发程序，其主要功能是进行几何体（body）的几何检查。
>
> 主要功能包括：
>
> 1. 选择一个或多个实体进行几何检查。用户可以通过对话框选择要检查的实体。
> 2. 对每个实体进行以下几何检查：
>
> 1. 检查过程中，会高亮显示有问题的面或边，并显示错误信息。
> 2. 检查结束后，用户可以继续选择新的实体进行检查，直到退出。
> 3. 使用UF_UNLOAD_IMMEDIATELY标志，表示程序在调用后立即卸载。
> 4. 使用UF_print_syslog函数，这是NX 18版本新增的功能，用于打印系统日志。
> 5. 使用UF_DISP_set_highlight函数，用于高亮显示实体。
> 6. 使用UF_MODL模块中的函数，用于查询实体的几何信息。
> 7. 使用UF_UI模块中的函数，用于用户交互。
> 8. 使用UF_CALL宏，用于调用NX API函数，并捕获错误。
>
> 总体来说，这段代码提供了方便的实体几何检查功能，可以用于NX CAD系统的二次开发。
>
