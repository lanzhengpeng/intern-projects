### 【3044】use uistyler to select face then points 使用UIStyler选择一个面，然后选择点

#### 代码

```cpp
/*HEAD USE_UISTYLER_TO_SELECT_FACE_THEN_POINTS CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_styler.h>
#include <uf_object_types.h>
#include <uf_curve.h>
#include <uf_disp.h>
#define IR4067662_CB_COUNT ( 1 + 1 ) /* Add 1 for the terminator 里海译:加1，代表终止符。 */
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog在V18中是新增的。 */
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
static int sel_cb(int num_sel, tag_t *sel_objects, int num_deselected,
    tag_t *deselected_objs,  void *user_data, UF_UI_selection_p_t select)
{
    int
        descriptor,
        subtype,
        type;
    tag_t
        view;
    double
        pos[3];
    UF_OBJ_disp_props_t
        attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,
            UF_OBJ_FONT_SOLID, FALSE};
    UF_UI_mask_t
        mask = { UF_point_type, 0, 0 };
    UF_CALL(UF_UI_ask_sel_descriptor(select, &descriptor));
    if (descriptor & UF_UI_SEL_DESC_SINGLE_POSITION)
    {
        UF_CALL(UF_UI_ask_sel_cursor_pos(select, &view, pos));
        attrib.color = UF_OBJ_WHITE;
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,
            UF_DISP_USE_ACTIVE_PLUS, pos, &attrib, UF_DISP_ASTERISK));
    }
    if (descriptor & UF_UI_SEL_DESC_SINGLE)
    {
        UF_CALL(UF_OBJ_ask_type_and_subtype(sel_objects[0], &type, &subtype));
        if (type == UF_point_type)
        {
            UF_CALL(UF_CURVE_ask_point_data(sel_objects[0], pos));
            attrib.color = UF_OBJ_YELLOW;
            UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,
                UF_DISP_USE_ACTIVE_PLUS, pos, &attrib, UF_DISP_ASTERISK));
        }
        else
        {
            UF_CALL(UF_UI_set_sel_mask(select,
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask));
            UF_CALL(UF_UI_set_sel_type(select,
                UF_UI_SEL_TYPE_SINGLE_SELECTION |
                UF_UI_SEL_TYPE_SINGLE_POSITION));
            UF_CALL(UF_UI_set_prompt("Select points or positions"));
        }
    }
    return( UF_UI_CB_CONTINUE_DIALOG);
}
static int IR4067662_const_cb(int dialog_id, void *client_data,
    UF_STYLER_item_value_type_p_t callback_data)
{
    UF_STYLER_item_value_type_t
        value;
    UF_UI_mask_t
        mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };
    value.item_id = UF_STYLER_DIALOG_INDEX;
    value.item_attr = UF_STYLER_SELECTION;
    UF_CALL(UF_STYLER_ask_value(dialog_id, &value));
    UF_CALL(UF_UI_set_sel_procs(value.value.selection, NULL, sel_cb, NULL));
    UF_CALL(UF_UI_set_sel_mask(value.value.selection,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask));
    return (UF_UI_CB_CONTINUE_DIALOG);
}
static void do_it(void)
{
    int response;
    UF_STYLER_callback_info_t IR4067662_cbs[IR4067662_CB_COUNT] = {
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, IR4067662_const_cb},
     {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };
    UF_CALL(UF_STYLER_create_dialog("4067662.dlg", IR4067662_cbs,
        IR4067662_CB_COUNT, NULL, &response));
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

> 根据代码内容，这是一段用于NX二次开发的代码，其主要功能是创建一个对话框，并设置选择回调函数。具体来说：
>
> 1. 头文件包含：代码包含了NX二次开发常用的头文件，如uf.h、uf_ui.h、uf_styler.h等。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 3. 选择回调函数：定义了一个选择回调函数sel_cb，用于在用户选择点或位置时显示一个临时的点。
> 4. 构造回调函数：定义了一个构造回调函数IR4067662_const_cb，用于在对话框创建时设置选择回调函数和选择过滤条件。
> 5. 主函数：定义了一个主函数do_it，用于创建对话框并设置构造回调函数。
> 6. UFusr函数：定义了UFusr函数，这是NX二次开发的入口函数，其中调用了主函数do_it。
> 7. 卸载函数：定义了卸载函数ufusr_ask_unload，返回UF_UNLOAD_IMMEDIATELY表示可以立即卸载。
>
> 总的来说，这段代码通过创建对话框，设置选择过滤条件，以及定义选择回调函数，实现了在NX中选择点或位置的功能。
>
