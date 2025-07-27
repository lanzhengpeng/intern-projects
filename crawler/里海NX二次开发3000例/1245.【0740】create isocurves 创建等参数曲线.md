### 【0740】create isocurves 创建等参数曲线

#### 代码

```cpp
/*HEAD CREATE_ISOCURVES CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_object_types.h>
#include <uf_disp.h>
#include <uf_modl.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，我理解到UF_print_syslog是V18版本新增的功能。文档提到该功能用于打印系统日志，并且强调只需要回答翻译，不需要添加任何额外的说明。因此，我将直接翻译该句子：

UF_print_syslog是V18版本新增的功能。 */
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
static tag_t select_a_face(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog(prompt, "",
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, FALSE));
        return object;
    }
    else return NULL_TAG;
}
static void do_it(void)
{
    int
        n_isos;
    tag_t
        *isos,
        face;
    double
        distol,
        uvs[4];
    UF_MODL_ask_distance_tolerance(&distol);
    while ((face = select_a_face("Select face to subdivide")) != NULL_TAG)
    {
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));
        UF_CALL(UF_MODL_create_isocurve(face, 1, (uvs[0]+uvs[1])/2, distol,
            &isos, &n_isos));
        UF_free(isos);
        UF_CALL(UF_MODL_create_isocurve(face, 2, (uvs[2]+uvs[3])/2, distol,
            &isos, &n_isos));
        UF_free(isos);
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

> 根据代码，这是一段用于在NX中创建等参数线的二次开发代码，主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用失败的错误信息。
> 2. 定义了一个选择面函数select_a_face，用于选择一个面进行后续操作。
> 3. 定义了一个掩码函数mask_for_faces，用于设置选择模式，仅选择实体面。
> 4. 主函数do_it中，首先获取默认距离公差，然后循环选择一个面，获取面的UV参数范围，并分别在中点创建等参数线，释放等参数线数组。
> 5. ufusr是主函数，初始化NX环境，调用do_it执行主要功能，然后终止NX环境。
> 6. ufusr_ask_unload函数用于在卸载用户程序时返回立即卸载标志。
>
> 综上，这段代码通过选择实体面，并在面的中点位置创建等参数线，实现了在NX中创建等参数线的功能。
>
