### 【2472】rotate point coordinates about selected line 关于选择线的点坐标旋转

#### 代码

```cpp
/*HEAD ROTATE_POINT_COORDINATES_ABOUT_SELECTED_LINE CCC UFUN */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_object_types.h>
#include <uf_disp.h>
#include <uf_curve.h>
#include <uf_vec.h>
#include <uf_trns.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，翻译如下：

注意：UF_print_syslog是V18版本新增的。

翻译完成，不需要添加任何额外内容。 */
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
static int mask_for_lines(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask[2] = { { UF_line_type, 0, 0 },
                    { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static tag_t select_a_line(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog(prompt, "",
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, 0));
        return object;
    }
    else return NULL_TAG;
}
static logical specify_point(char *prompt, double loc[3])
{
    int
        ia2[2] = { 0,0 },
        ip3 = 0,
        irc;
    irc = uc1616(prompt, ia2, ip3, loc);
    if (irc == 5) return TRUE;
    else return FALSE;
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
        resp;
    tag_t
        line,
        point;
    double
        angle = { 90.0 },
        loc[3],
        mx[12],
        vec[3];
    UF_CURVE_line_t
        line_coords;
    while ((( line = select_a_line("Select line")) != NULL_TAG) &&
        specify_point("Specify point", loc) &&
        prompt_for_a_number("Enter rotation angle", "degrees", &angle))
    {
        UF_CALL(UF_CURVE_ask_line_data(line, &line_coords));
        UF_VEC3_sub(line_coords.end_point, line_coords.start_point, vec);
        FTN(uf5945)(line_coords.start_point, vec, &angle, mx, &resp);
        if (!resp)
        {
            FTN(uf5941)(loc, mx);
            UF_CALL(UF_CURVE_create_point(loc, &point));
        }
        else
            uc1601("uf5945 failed", TRUE);
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

> 这是用于在NX中通过选择一条线和一个点来旋转该线周围的点集的二次开发代码。主要功能包括：
>
> 1. 选择一条线：用户可以交互式选择一条线作为旋转轴。
> 2. 指定一个点：用户可以指定一个点作为旋转点。
> 3. 输入旋转角度：用户可以输入旋转角度，默认为90度。
> 4. 旋转点集：根据选择的线和点以及输入的角度，旋转该线周围的点集。
> 5. 创建点：旋转后创建一个新点，并高亮显示。
> 6. 错误处理：使用UF_CALL宏进行错误处理，打印错误信息。
> 7. 用户交互：使用UF_UI进行用户交互，打开选择对话框等。
> 8. 数据获取：使用UF_CURVE获取线数据，使用UF_VEC进行向量运算。
> 9. 点创建：使用UF_CURVE创建点。
> 10. 视图显示：使用UF_DISP高亮显示选择对象。
>
> 综上所述，这段代码实现了在NX中通过交互式选择和输入，对指定线周围的点集进行旋转的功能。
>
