### 【1591】map colors from specified cdf to current part 将指定CDF中的颜色映射到当前零件

#### 代码

```cpp
/*HEAD MAP_COLORS_FROM_SPECIFIED_CDF_TO_CURRENT_PART CCC UFUN */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_cfi.h>
#include <uf_disp.h>
#include <uf_undo.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是在 V18 版本中新增的。 */
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
static void strip_trailing_spaces(char *s)
{
    int ii;
    for (ii = strlen(s) - 1; s[ii] == ' '; ii--) s[ii] = '\0';
}
static void load_color_definition_file(char *cdf_spec)
{
    int
        ii;
    double
        rgb[3];
    char
        a_line[MAX_LINE_SIZE+1],
        color_name[UF_DISP_MAX_NAME_SIZE+1],
        *ptr;
    FILE
        *cdf;
    if ((cdf = fopen(cdf_spec, "r")) != NULL)
    {
    /*  Read 4 header lines 里海译:对不起，您上传的图片无法识别，请重新上传。 */
        for (ii = 0; ii < 4; ii++)
            fgets(a_line, MAX_LINE_SIZE, cdf);
        ii = 0;
        while ((fgets(a_line, MAX_LINE_SIZE, cdf)) != NULL)
        {
            strncpy(color_name, a_line, UF_DISP_MAX_NAME_SIZE);
            color_name[UF_DISP_MAX_NAME_SIZE] = '\0';
            strip_trailing_spaces(color_name);
            ptr = a_line + 32;
            rgb[0] = atof(ptr);
            ptr = ptr + 11;
            rgb[1] = atof(ptr);
            ptr = ptr + 11;
            rgb[2] = atof(ptr);
            UF_CALL(UF_DISP_set_color(ii, UF_DISP_rgb_model, color_name, rgb));
            ii++;
        }
        fclose(cdf);
        UF_CALL(UF_DISP_load_color_table());
    }
}
static void do_it(void)
{
    int
        new_color,
        old_color,
        resp;
    double
        clr_values[15][3];
    char
        cdf_spec[UF_CFI_MAX_PATH_NAME_SIZE],
        *clr_name[15],
        filter[UF_CFI_MAX_PATH_NAME_SIZE],
        *new_name,
        *root_dir;
    UF_UNDO_mark_id_t
        mark;
    UF_CALL(UF_translate_variable("UGII_ROOT_DIR", &root_dir));
    UF_CALL(uc4575(root_dir, 0, "xxx.cdf", filter));
    strcpy(strstr(filter, "xxx.cdf"), "*.cdf");
    while (!UF_CALL(UF_UI_create_filebox("Enter a Color Definition File Name",
        "Color Definition File Names", filter, "", cdf_spec, &resp)) &&
        (resp == UF_UI_OK))
    {
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark));
        load_color_definition_file(cdf_spec);
        for (old_color = 1; old_color <= 15; old_color++)
            UF_CALL(UF_DISP_ask_color(old_color, UF_DISP_rgb_model,
                &clr_name[old_color-1], &clr_values[old_color-1][0]));
        UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));
        UF_CALL(UF_UNDO_delete_mark(mark, NULL));
        WRITE(cdf_spec);
        WRITE(": \n");
        for (old_color = 1; old_color <= 15; old_color++)
        {
            WRITE(clr_name[old_color-1]);
            WRITE(" => ");
            UF_CALL(UF_DISP_ask_closest_color(UF_DISP_rgb_model,
                clr_values[old_color-1], UF_DISP_CCM_EUCLIDEAN_DISTANCE,
                &new_color));
            UF_CALL(UF_DISP_ask_color(new_color, UF_DISP_rgb_model, &new_name,
                &clr_values[old_color-1][0]));
            WRITE(new_name);
            WRITE("\n");
            UF_free(clr_name[old_color-1]);
            UF_free(new_name);
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

> 这段代码是NX的二次开发代码，主要功能是根据指定的CDF文件，将NX的当前部件的颜色从当前颜色映射到CDF文件中定义的颜色。下面是代码的主要流程：
>
> 1. 定义了一个错误报告函数report_error，用于在调用NX API失败时打印错误信息。
> 2. 定义了strip_trailing_spaces函数，用于去除字符串末尾的空格。
> 3. 定义了load_color_definition_file函数，用于加载CDF文件，读取文件中的颜色定义，并调用NX API设置颜色。
> 4. 定义了do_it函数，首先通过对话框让用户选择CDF文件，然后调用load_color_definition_file函数加载CDF文件中的颜色。接着获取当前部件的15个颜色，撤销加载CDF文件的操作，然后找出每个当前颜色最接近的CDF文件中的颜色，并打印出来。
> 5. 定义了ufusr函数，这是NX二次开发程序的入口函数，初始化NX，调用do_it函数，然后结束NX。
> 6. 定义了ufusr_ask_unload函数，用于在卸载二次开发程序时立即返回。
>
> 总的来说，这段代码通过加载CDF文件中的颜色定义，实现了将NX当前部件的颜色映射到CDF文件中的颜色，实现了颜色的转换功能。
>
