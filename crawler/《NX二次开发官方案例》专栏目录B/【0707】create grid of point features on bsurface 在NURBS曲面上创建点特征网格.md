### 【0707】create grid of point features on bsurface 在NURBS曲面上创建点特征网格

#### 代码

```cpp
/*HEAD CREATE_GRID_OF_POINT_FEATURES_ON_BSURFACE CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_so.h>
#include <uf_point.h>
#include <uf_ui.h>
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
#define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)
/* 里海 */
static int mask_for_bsurfs(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PARAMETRIC_FACE };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static tag_t select_a_bsurf(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog("Select a bsurf", prompt,
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bsurfs, NULL, &resp,
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
        inx = 0,
        jnx = 0;
    tag_t
        surface = NULL_TAG,
        u_tag = NULL_TAG,
        v_tag = NULL_TAG,
        point_feat = NULL_TAG;
    double
        u = 0.,
        v = 0.;
    UF_SO_update_option_t 
        update = UF_SO_update_within_modeling;
    surface = select_a_bsurf("Select Surface:");
    if( NULL_TAG == surface ) return;
    for(inx = 0; inx < 5; inx++)
        for(jnx = 0; jnx < 5; jnx++)
        {
            u = 0. + ( .25 * (double) inx );
            v = 0. + ( .25 * (double) jnx );
            UF_CALL( UF_SO_create_scalar_double(surface, update, u, &u_tag));
            UF_CALL( UF_SO_create_scalar_double(surface, update, v, &v_tag));
            UF_CALL( UF_POINT_create_on_surface(surface, u_tag, 
                                                       v_tag, &point_feat ));
        }
}
/* 里海 */
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

> 这是用于在NX曲面模型上创建点特征的二次开发代码。代码的主要功能如下：
>
> 1. 包含了UF模块和UI模块的头文件，用于调用NX提供的API函数。
> 2. 定义了report_error函数，用于在API调用出错时打印错误信息。
> 3. 定义了mask_for_bsurfs函数，用于设置选择过滤器，以便只选择参数曲面。
> 4. 定义了select_a_bsurf函数，用于提示用户选择一个曲面，并返回所选曲面的tag。
> 5. 定义了do_it函数，用于在所选曲面上创建5x5的点阵列。它遍历曲面上的点，并使用UF_SO_create_scalar_double和UF_POINT_create_on_surface API在曲面上创建点特征。
> 6. ufusr函数是二次开发程序的入口函数，在NX启动时自动调用。在该函数中，先调用UF_initialize初始化NX，然后调用do_it执行创建点特征的操作，最后调用UF_terminate终止NX。
> 7. ufusr_ask_unload函数用于设置二次开发程序的卸载模式。
>
> 整体来说，这段代码实现了在用户选择的参数曲面上均匀创建点特征的功能，并包含错误处理、选择过滤等辅助功能。
>
