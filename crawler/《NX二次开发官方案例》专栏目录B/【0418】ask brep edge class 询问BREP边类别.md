### 【0418】ask brep edge class 询问BREP边类别

#### 代码

```cpp
/*HEAD ASK_BREP_EDGE_CLASS CCC UFUN */
/*  Although this sample code is a valid example - the function
    UF_BREP_ask_edge_class does not return any values.
    See PR 1060098.
*/
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_object_types.h>
#include <uf_disp.h>
#include <uf_brep.h>
#define ECHO(X)    (printf("%s = %d\n", #X, X))
#define INDENT(X)  for (ii = 0; ii < X; ii++) printf ("  ")
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
    /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本新增的函数，请只提供翻译，无需添加其他内容。 */
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
static void print_topology_tree(UF_BREP_topology_t *child_topology, int depth)
{
    int
        ii,
        edge_class = 999,
        geom_class = 999,
        unique_id;
    INDENT(depth);
    switch (child_topology->type)
    {
        case UF_BREP_SOLID_TOPO:
            printf ("Solid");
            break;
        case UF_BREP_SHEET_TOPO:
            printf ("Sheet");
            break;
        case UF_BREP_TRIMSURF_TOPO:
            printf ("Trimsurf");
            break;
        case UF_BREP_SHELL_TOPO:
            printf ("Shell");
            break;
        case UF_BREP_FACE_TOPO:
            printf ("Face");
            break;
        case UF_BREP_LOOP_TOPO:
            printf ("Loop");
            break;
        case UF_BREP_FIN_TOPO:
            printf ("Fin");
            break;
        case UF_BREP_EDGE_TOPO:
            printf ("Edge");
            break;
        case UF_BREP_VERTEX_TOPO:
            printf ("Vertex");
            break;
        default:
            printf ("Unknown");
            break;
    }
    UF_BREP_ask_identifier(child_topology, &unique_id);
    printf(":  UGtag=%d, id=%d\n", child_topology->tag, unique_id);
    if (child_topology->type == UF_BREP_EDGE_TOPO)
    {
        UF_CALL(UF_BREP_ask_edge_class(child_topology, &edge_class,
            &geom_class));
        ECHO(edge_class);
        ECHO(geom_class);
    }
    for (ii = 0; ii < child_topology->num_children; ii++)
        print_topology_tree(child_topology->children[ii].child, depth+1);
}
static void do_it(void)
{
    int
        nstates;
    tag_t
        body;
    UF_BREP_topology_p_t
        topo;
    UF_BREP_state_p_t
        states;
    while ((body = select_a_body("Select body")) != NULL_TAG)
    {
        UF_CALL(UF_BREP_ask_topology(body, 0, &topo, &nstates, &states));
        print_topology_tree(topo, 0);
        if (nstates > 0) UF_free(states);
        UF_CALL(UF_BREP_release_topology(topo, NULL));
    }
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

> 这段代码是一个NX二次开发示例，用于查询并打印出所选体的BREP边线类别。主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 2. 定义了一个遮罩函数mask_for_bodies，用于设置选择对话框的遮罩，仅允许选择实体。
> 3. 定义了一个选择函数select_a_body，用于弹出一个对话框，让用户选择一个实体。
> 4. 定义了一个递归打印函数print_topology_tree，用于递归打印BREP拓扑树的每个节点及其类别信息。如果是边线节点，还会打印出边线的类别信息。
> 5. 主函数do_it中，循环调用select_a_body函数，获取用户选择的实体，然后调用UF_BREP_ask_topology查询BREP拓扑树，再调用print_topology_tree打印拓扑树信息。
> 6. ufusr函数是NX二次开发程序的入口函数，初始化后调用do_it，最后结束运行。
> 7. ufusr_ask_unload函数用于处理程序卸载。
>
> 该代码通过递归遍历BREP拓扑树，并调用UF_BREP_ask_edge_class查询边线类别，实现了查询并打印出所选体的BREP边线类别的功能。
>
