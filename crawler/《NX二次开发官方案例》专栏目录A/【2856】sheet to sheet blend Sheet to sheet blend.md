### 【2856】sheet to sheet blend Sheet to sheet blend

#### 代码

```cpp
/*HEAD SHEET_TO_SHEET_BLEND CCC UFUN */
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能，仅回答翻译，不要废话。

UF_print_syslog在V18版本中是一个新的功能。 */
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
static int filter_body_type(tag_t object, int type[3], void *data,
    UF_UI_selection_p_t select)
{
    int
        body_type;
    UF_CALL(UF_MODL_ask_body_type(object, &body_type));
    if (body_type == *(int *)data)
        return (UF_UI_SEL_ACCEPT);
    else
        return (UF_UI_SEL_REJECT);
}
static int mask_body_type( UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&
        !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static tag_t select_a_sheet(char *prompt)
{
    double
        cp[3];
    int
        sheet = UF_MODL_SHEET_BODY,
        resp;
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog( prompt, "",
        UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, 0));
        return object;
    }
    else return NULL_TAG;
}
static void do_it(void)
{
    int
        n_bodies,
        n_faces;
    tag_t
        blend,
        *bodies,
        *faces,
        first_set,
        second_set;
    UF_MODL_blend_faces_create_data_t
        create_data;
    create_data.first_set = &first_set;
    create_data.first_set_size = 1;
    create_data.flip_first_normal = FALSE;
    create_data.second_set = &second_set;
    create_data.second_set_size = 1;
    create_data.flip_second_normal = TRUE;
    create_data.propagate = FALSE;
    create_data.cliff_edges = NULL;
    create_data.n_cliff_edges = 0;
    create_data.thls = NULL;
    create_data.n_thls = 0;
    create_data.proj_on_first_set = FALSE;
    create_data.end_overflow = TRUE;
    create_data.blend_tolerance = "0.0004";
    create_data.trim_option = UF_TRIM_AND_ATTACH;
    create_data.radius_type = UF_CONSTANT;
    create_data.default_radius = "1.0";
    create_data.law_parameters = NULL;
    if (((first_set = select_a_sheet("Select first set sheet")) != NULL_TAG)
     && ((second_set = select_a_sheet("Select second set sheet")) != NULL_TAG))
    {
        UF_CALL(UF_MODL_create_blend_faces(&create_data, NULL, &blend,
            &n_bodies, &bodies, &n_faces, &faces));
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

> 这段代码是用于NX的二次开发，实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个过滤函数filter_body_type，用于在图形选择时只选择片体。
> 3. 定义了一个遮罩函数mask_body_type，用于设置图形选择时的过滤条件，只选择片体。
> 4. 定义了一个选择函数select_a_sheet，用于提示用户选择一个片体。
> 5. 定义了一个执行函数do_it，用于执行以下操作：选择两个片体作为混合面的第一集合和第二集合创建混合面，并设置混合面的参数，如翻转法线方向、投影方向、修剪选项等输出混合面创建后的体和面信息
> 6. 选择两个片体作为混合面的第一集合和第二集合
> 7. 创建混合面，并设置混合面的参数，如翻转法线方向、投影方向、修剪选项等
> 8. 输出混合面创建后的体和面信息
> 9. 主函数ufusr初始化NX环境，执行do_it函数，然后终止NX环境。
> 10. 提供了一个卸载函数ufusr_ask_unload，用于在插件卸载时立即卸载。
>
> 总的来说，这段代码实现了选择两个片体并创建混合面的功能。
>
