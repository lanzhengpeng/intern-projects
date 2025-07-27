### 【0746】create label to selected faces silhouette 在选择的面对轮廓上创建标签

#### 代码

```cpp
/*HEAD CREATE_LABEL_TO_SELECTED_FACES_SILHOUETTE CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_object_types.h>
#include <uf_disp.h>
#include <uf_drf.h>
#include <uf_draw.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 译:根据文档描述，UF_print_syslog是在V18版本中新增的一个函数。因此，针对这个问题的翻译为：注意：UF_print_syslog是在V18版本中新增的。 */
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
/* 里海 */
static int mask_for_faces(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static tag_t select_drf_face(char *prompt, tag_t *view, double *cp)
{
    int
        resp;
    tag_t
        object;
    UF_CALL(UF_UI_set_cursor_view(0));
    UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,
        UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp,
        &object, cp, view));
    UF_CALL(UF_UI_set_cursor_view(1));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, FALSE));
        return object;
    }
    else return NULL_TAG;
}
static logical select_pos(char *prompt, double pos[3])
{
    int
        resp = uc1615(prompt, pos);
    if (resp == 5) return TRUE;
    else return FALSE;
}
static void do_it(void)
{
    int
        ii = 0,
        n_sils;
    tag_t
        face,
        label,
        *sils;
    double
        cp[3],
        pos[3];
    char
        label_text[1][MAX_LINE_LENGTH + 1];
    UF_DRF_object_t
        curve;
    UF_CALL(UF_DRF_init_object_structure(&curve));
    while ((face = select_drf_face("Select face to label",
        &curve.object_view_tag, cp)) != NULL_TAG)
    {
        if (UF_CALL(UF_DRAW_ask_face_sils_in_view(face, curve.object_view_tag,
            &n_sils, &sils)) || (n_sils == 0)) continue;
        curve.object_tag = sils[0];
        UF_free(sils);
        if (select_pos("Indicate origin for label", pos))
        {
            sprintf(&label_text[0][0], "Label %d", ++ii);
            UF_CALL(UF_DRF_create_label(1, label_text, pos,
                UF_DRF_leader_attach_object, &curve, cp, &label));
        }
    }
}
/* 里海 */
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

> 这是段NX二次开发代码，其主要功能是选择一个面，然后在视图中为其添加一个标签。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于打印错误信息。
> 2. 定义选择面函数select_drf_face，用于选择一个面并获取其在当前视图中的轮廓线。
> 3. 定义选择位置函数select_pos，用于提示用户输入一个位置。
> 4. 主函数do_it中，通过循环调用select_drf_face选择面，获取轮廓线，然后调用select_pos选择标签位置，最后调用UF_DRF_create_label创建标签。
> 5. 定义ufusr函数，初始化NX系统，调用do_it执行主功能，最后终止NX系统。
> 6. 定义卸载函数ufusr_ask_unload，用于在卸载时立即卸载。
>
> 该代码通过选择面，获取轮廓线，选择标签位置，然后创建标签的方式，实现了为面添加标签的功能。
>
