### 【1161】edit selected plane 编辑选定的平面

#### 代码

```cpp
/*HEAD EDIT_SELECTED_PLANE CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_disp.h>
#include <uf_object_types.h>
#include <uf_modl.h>
#include <uf_csys.h>
#include <uf_mtx.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。 */
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
static int mask_for_planes(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_plane_type, 0, 0 };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static tag_t select_a_plane(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog("Select a plane", prompt,
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_planes, NULL, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, 0));
        return object;
    }
    else return NULL_TAG;
}
static logical specify_vector(char *prompt, double vec[3], double point[3])
{
    int
        mode = UF_UI_NO_DEFAULT,
        resp;
    UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,
        vec, point, &resp));
    if (resp == UF_UI_OK) return TRUE;
    else return FALSE;
}
static void do_it(void)
{
    tag_t
        matrix,
        plane;
    double
        mtx[9],
        normal[3],
        origin[3];
    while ((plane = select_a_plane("Edit")) != NULL_TAG)
    {
        if (specify_vector("New Orientation", normal, origin))
        {
            UF_CALL(UF_MODL_edit_plane(plane, origin, normal));
        /*  The following is a work around - see PR 2021085 里海译:以下是一个变通方法 - 请查看PR 2021085 */
            UF_CALL(UF_MTX3_initialize_z(normal, mtx));
            UF_CALL(UF_CSYS_create_matrix(mtx, &matrix));
            UF_CALL(UF_CSYS_edit_matrix_of_object(plane, matrix));
        }
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

> 这是NX Open C++的二次开发代码，用于编辑选定平面的方向。主要功能包括：
>
> 1. 函数报告错误：用于报告UF函数调用时的错误，并输出错误信息。
> 2. 函数mask_for_planes：用于设置选择平面时的过滤条件，只允许选择平面。
> 3. 函数select_a_plane：用于提示用户选择一个平面，并返回平面对象的标签。
> 4. 函数specify_vector：用于提示用户指定一个向量和一点，用于定义平面的新方向。
> 5. 函数do_it：主函数，循环提示用户选择平面，然后编辑其方向。先通过UF_MODL_edit_plane函数编辑平面参数，但由于NX的一个bug，还需通过创建矩阵对象并关联到平面，才能正确更新平面的方向。
> 6. 函数ufusr：入口函数，初始化NX Open C++环境，调用主函数，然后终止环境。
> 7. 函数ufusr_ask_unload：用于卸载动态库。
>
> 总的来说，这段代码实现了选择并编辑NX中平面的方向的二次开发功能。
>
