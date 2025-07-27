### 【2024】report closest color in displayed part 在显示部件中报告最近的颜色

#### 代码

```cpp
/*HEAD REPORT_CLOSEST_COLOR_IN_DISPLAYED_PART CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_obj.h>
#include <uf_disp.h>
#include <uf_disp_types.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的提示，UF_print_syslog 是在 V18 版本中新增的函数。 */
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
#define WRITE_S(X) (write_string_to_listing_window(#X, X))
void write_string_to_listing_window(char *title, char *string)
{
    char
        msg[UF_UI_MAX_STRING_LEN+1];
    UF_CALL(UF_UI_open_listing_window());
    sprintf(msg, "%s = \"%s\"\n", title, string);
    UF_CALL(UF_UI_write_listing_window(msg));
}
/*  Prior to NX 4.0, the returned full color name looked like this:
    full_color_name = "2 (Green)"
    So the method shown here cleans it up and returns just the name.
*/
static void ask_color_name(int color_num, char *color_name)
{
    double
        clr_values[3];
    char
        *full_color_name;
    UF_CALL(UF_DISP_ask_color(color_num, UF_DISP_rgb_model, &full_color_name,
        clr_values));
    if (strchr(full_color_name, '('))
    {
        strcpy(color_name, strchr(full_color_name, '(')+1);
        strcpy(strchr(color_name, ')'), "");
    }
    else
        strcpy(color_name, full_color_name);
    UF_free(full_color_name);
}
static void do_it(void)
{
    int
/* UF_DISP_color_name_e in uf_disp_types.h shows the possible enums 里海译:UF_DISP_color_name_e in uf_disp_types.h shows the possible enums. */
        colorq = UF_DISP_CYAN_NAME,
        col_no;
    char
        color_name[MAX_ENTITY_NAME_SIZE+1],
        msg[MAX_LINE_SIZE+1];
    UF_CALL(UF_DISP_ask_closest_color_in_displayed_part(colorq, &col_no));
    ask_color_name(col_no, color_name);
    sprintf(msg, "The closest color number corresponding to \"UF_DISP_CYAN_NAME\",\
     \n is %d, which is named in this part: %s\n",
        col_no, color_name);
    uc1601(msg, TRUE);
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

> 这段代码是NX二次开发的UFUN函数，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于打印UF函数调用的错误信息。
> 2. 定义了WRITE宏，用于打开并写入列表窗口。
> 3. 定义了WRITE_S函数，用于将字符串写入列表窗口。
> 4. 定义了ask_color_name函数，用于获取指定颜色编号的颜色名称。
> 5. 定义了do_it函数，用于实现主要功能：调用UF_DISP_ask_closest_color_in_displayed_part函数，获取当前显示部件中与指定颜色名称UF_DISP_CYAN_NAME最接近的颜色编号。调用ask_color_name函数，获取该颜色编号的颜色名称。打印输出结果。
> 6. 调用UF_DISP_ask_closest_color_in_displayed_part函数，获取当前显示部件中与指定颜色名称UF_DISP_CYAN_NAME最接近的颜色编号。
> 7. 调用ask_color_name函数，获取该颜色编号的颜色名称。
> 8. 打印输出结果。
> 9. 定义了ufusr函数，作为UFUN的入口点，初始化UF模块，调用do_it函数，然后终止UF模块。
> 10. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了在当前显示的NX部件中，获取与指定颜色名称最接近的颜色编号及其名称，并将结果打印输出。代码结构清晰，功能明确。
>
