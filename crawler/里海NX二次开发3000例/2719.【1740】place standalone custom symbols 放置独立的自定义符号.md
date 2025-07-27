### 【1740】place standalone custom symbols 放置独立的自定义符号

#### 代码

```cpp
/*HEAD PLACE_STANDALONE_CUSTOM_SYMBOLS CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_obj.h>
#include <uf_object_types.h>
#include <uf_curve.h>
#include <uf_drf.h>
#include <uf_disp.h>
#include <uf_part.h>
#include <uf_ugfont.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的函数，请仅回答翻译，不要添加其他内容。 */
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
static logical specify_position(char *prompt, double pos[3])
{
    int
        resp = uc1615(prompt, pos);
    if (resp == 5) return TRUE;
    else return FALSE;
}
static tag_t ask_font_table_entity(void)
{
    tag_t
        fte = NULL_TAG,
        part = UF_PART_ask_display_part();
    UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_font_table_type, &fte));
    return (fte);
}
static int ask_font_index_number(char *font_name)
{
    int
        index;
    tag_t
        fte = ask_font_table_entity();
    UF_CALL(UF_UGFONT_add_font(fte, &index, font_name));
    return (index);
}
static void do_it(void)
{
    int
        mpi[100],
        o_mpi[100];
    tag_t
        lines[2],
        symbol;
    double
        anchor[3] = { 1, .5, 0 },
        mpr[70],
        o_mpr[70],
        note_origin[3] = { 1, 0, 0 };
    char
        diameter[27],
        *note_text[1] = { "DUMMY TEXT" },
        radius[27],
        *custom_text = { "HERE!" };
    UF_CURVE_line_t
        line_data[2] = {{{ 0,0,0 }, { 2,1,0}}, {{0,1,0}, {2,0,0}}};
    UF_DRF_custom_symbol_t
        symbol_definition;
    UF_DRF_custom_symbol_text_t
        symbol_text;
    UF_CALL(UF_DRF_initialize_custom_symbol_data(&symbol_definition));
    symbol_definition.geometry = lines;
    symbol_definition.num_geometry = 2;
    symbol_definition.num_text = 1;
    symbol_definition.text = &symbol_text;
    UF_CALL(UF_DRF_initialize_custom_symbol_text_data(&symbol_text));
    symbol_text.title = "X marks the spot";
    symbol_text.simple_text = custom_text;
    UF_CALL(UF_DRF_ask_preferences(mpi, mpr, radius, diameter));
    UF_CALL(UF_DRF_ask_preferences(o_mpi, o_mpr, radius, diameter));
    mpi[30] = 2;  /* top center 里海译:顶部居中 */
    mpi[87] = UF_OBJ_YELLOW;
    mpi[88] = ask_font_index_number("leroy");
    mpi[89] = UF_OBJ_WIDTH_NORMAL;
    mpr[3] = 0;
    mpr[44] = 0.125;
    UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));
    UF_CALL(UF_DISP_set_display(FALSE));
    while (specify_position("Place custom symbol", symbol_definition.origin))
    {
        UF_CALL(UF_CURVE_create_line(&line_data[0], &lines[0]));
        UF_CALL(UF_CURVE_create_line(&line_data[1], &lines[1]));
        UF_CALL(UF_CURVE_create_point(anchor, &symbol_definition.anchor_point));
        UF_CALL(UF_DRF_create_note(1, note_text, note_origin, 0,
            &symbol_text.note_tag));
        UF_CALL(UF_DRF_create_custom_symbol_instance(&symbol_definition,
            &symbol));
        UF_CALL(UF_DISP_add_item_to_display(symbol));
    }
    UF_CALL(UF_DRF_set_preferences(o_mpi, o_mpr, radius, diameter));
    UF_CALL(UF_DISP_set_display(TRUE));
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

> 根据提供的代码，这是一段NX Open C++的二次开发代码，用于在NX中放置一个自定义符号。主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于捕获UF函数调用错误，并打印错误信息到系统日志和列表窗口。
> 2. 定义了一个获取位置函数specify_position，用于提示用户输入符号的位置。
> 3. 定义了获取字体表实体函数ask_font_table_entity，用于获取当前激活的字体表实体。
> 4. 定义了获取字体索引函数ask_font_index_number，用于向字体表中添加指定字体并返回其索引。
> 5. 定义了主函数do_it，用于创建自定义符号实例，包括创建2条线、1个点、1个注释文本，设置符号属性，并添加到显示中。
> 6. 定义了ufusr函数，作为NX二次开发的入口函数，初始化NX Open环境，调用do_it函数，然后终止NX Open环境。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发应用程序可以立即卸载。
>
> 总体来说，这段代码实现了在NX中交互式放置自定义符号的功能。
>
