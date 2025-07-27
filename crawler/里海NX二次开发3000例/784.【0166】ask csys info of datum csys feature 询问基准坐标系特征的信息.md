### 【0166】ask csys info of datum csys feature 询问基准坐标系特征的信息

#### 代码

```cpp
/*HEAD ASK_CSYS_INFO_OF_DATUM_CSYS_FEATURE CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_csys.h>
#include <uf_modl.h>
#include <uf_ui.h>
#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
/* 里海 */
static int report_error( char *file, int line, char *call, int irc)
{
    if (irc)
    {
        char err[133],
             msg[133];
        sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",
            irc, line, file);
        UF_get_fail_message(irc, err);
    /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是在V18版本中新增的功能，其作用是打印系统日志。

UF_print_syslog函数在V18版本中新增，用于打印系统日志。 */
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
#define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))
#define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))
static void write_double_array_to_listing_window(char *name, double *array,
        int rows, int cols)
{
    int
        ii,
        jj,
        kk = 0;
    char
        blanks[UF_UI_MAX_STRING_LEN+1],
        msg[UF_UI_MAX_STRING_LEN+1];
    UF_CALL(UF_UI_open_listing_window());
    memset(blanks, ' ', strlen(name) + 3);
    blanks[strlen(name) + 3] = '\0';
    for (ii = 0; ii < rows; ii++)
    {
        if (ii == 0) sprintf(msg, "%s = ", name);
        else sprintf(msg, "%s", blanks);
        if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));
        for (jj = 0; jj < cols; jj++)
        {
            sprintf(msg, "%f", array[kk++]);
            UF_CALL(UF_UI_write_listing_window(msg));
            if (jj < cols - 1)
            {
                sprintf(msg, ", ");
                UF_CALL(UF_UI_write_listing_window(msg));
            }
        }
        if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));
    }
}
static void display_colored_conehead(double origin[3], double vec[3], int color)
{
    UF_DISP_conehead_attrb_s
        attrb;
    UF_DISP_get_conehead_attrb(&attrb);
    attrb.color = color;
    UF_DISP_set_conehead_attrb(&attrb);
    UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);
    UF_DISP_reset_conehead_attrb();
}
static void report_csys_info(tag_t csys)
{
    tag_t
        matrix;
    double
        origin[3],
        vectors[9];
    if (UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin))) return;
    WRITE3F(origin);
    UF_CALL(UF_CSYS_ask_matrix_values(matrix, vectors));
    WRITE9F(vectors);
    display_colored_conehead(origin, &vectors[0], UF_OBJ_RED);
    display_colored_conehead(origin, &vectors[3], UF_OBJ_GREEN);
    display_colored_conehead(origin, &vectors[6], UF_OBJ_BLUE);
}
static tag_t select_a_feature(char *prompt)
{
    int
        cnt,
        resp;
    tag_t
        first,
        *feats;
    UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));
    if (cnt)
    {
        first = feats[0];
        UF_free(feats);
        return (first);
    }
    else return (NULL_TAG);
}
static void do_it(void)
{
    tag_t
        d_csys,
        csys,
        origin,
        daxes[3],
        dplanes[3];
    while ((d_csys = select_a_feature("Select a DATUM_CSYS")) != NULL_TAG)
    {
    /*  The following function is a void, so do not UF_CALL it 译:这个函数是void类型，不要调用它。 */
        UF_MODL_ask_datum_csys_components(d_csys,&csys,&origin,daxes,dplanes);
        if (csys != NULL_TAG) report_csys_info(csys);
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

> 这段代码是用于在NX中获取和显示DATUM_CSYS特征的坐标系信息的二次开发代码。
>
> 主要功能包括：
>
> 1. 使用UF_CSYS_ask_csys_info()函数获取DATUM_CSYS的坐标系信息，包括坐标系矩阵和原点。
> 2. 使用UF_CSYS_ask_matrix_values()函数获取坐标系矩阵的值。
> 3. 在图形窗口中显示坐标系的原点和三个轴，并使用不同颜色来区分。
> 4. 提供一个选择DATUM_CSYS的对话框，用户可以选择一个DATUM_CSYS，然后显示其坐标系信息。
> 5. 包含错误处理和日志记录功能，以便在函数调用失败时打印错误信息。
> 6. 包含一个简单的坐标系信息显示函数，用于在图形窗口中显示坐标轴。
> 7. 使用UF_MODL_ask_datum_csys_components()函数获取DATUM_CSYS的组件信息。
> 8. 在主函数中循环选择DATUM_CSYS并显示其坐标系信息。
> 9. 包含标准NX二次开发代码结构，包括初始化、功能实现和终止等部分。
>
> 总体来说，这段代码通过NX提供的坐标系相关函数，实现了获取和图形化显示DATUM_CSYS坐标系信息的功能。
>
