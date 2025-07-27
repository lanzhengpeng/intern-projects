### 【0399】author html external 作者HTML外部

#### 代码

```cpp
/*HEAD AUTHOR_HTML_EXTERNAL CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <uf.h>
#include <uf_cfi.h>
#include <uf_part.h>
#include <uf_web.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */
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
static void build_temp_filespec(char *fspec, int ftype)
{
    tag_t
        part = UF_PART_ask_display_part();
    char
        dspec[UF_CFI_MAX_PATH_NAME_SIZE],
        part_name[UF_CFI_MAX_FILE_NAME_SIZE],
        part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],
        *tmp_dir;
    UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));
    UF_PART_ask_part_name(part, part_fspec);
    UF_CALL(uc4576(part_fspec, 2, dspec, part_name));
    UF_CALL(uc4578(part_name, 2, part_name));
    UF_CALL(uc4575(tmp_dir, ftype, part_name, fspec));
}
static void do_it(void)
{
    char
        html_spec[MAX_FSPEC_SIZE+1];
    build_temp_filespec(html_spec, 0);
    UF_CALL(UF_WEB_author_html(html_spec, "/full/path/to/template.htt", FALSE));
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
static void check_load_status(UF_PART_load_status_p_t status)
{
    char
        msg[133];
    int
        ii;
    for (ii=0; ii<status->n_parts; ii++)
    {
        UF_get_fail_message(status->statuses[ii], msg);
        printf("    %s - %s\n", status->file_names[ii], msg);
    }
    UF_free(status->statuses);
    UF_free_string_array(status->n_parts, status->file_names);
}
/*  To run in external native mode UG:
        program.exe /path/to/part1.prt /path/to/part2.prt [...]
    To run in external UG/Manager or IMAN mode:
        program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]
*/
int main( int argc, char *argv[] )
{
    int
        cnt = 0;
    tag_t
        part;
    char
        new_fspec[MAX_FSPEC_SIZE+1],
        part_name[MAX_FSPEC_SIZE+1];
    UF_PART_load_status_t
        status;
    if (!UF_CALL(UF_initialize()))
    {
        UF_CALL(uc4624(0, argc, argv));
        while (uc4621(part_name) == 1)
        {
            printf("%d.  %s\n", ++cnt, part_name);
            if (UF_CALL(UF_PART_open(part_name, &part, &status))
                || status.n_parts > 0) check_load_status(&status);
            else
            {
                do_it();
                UF_PART_close_all();
            }
        }
        printf("\nProcessed %d parts.\n", cnt);
        UF_CALL(UF_terminate());
    }
    return 0;
}

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用NX API时捕获错误并打印错误信息。
> 2. 定义了一个构建临时文件路径的函数build_temp_filespec，用于根据当前显示的部件名称在临时目录下构建一个临时HTML文件路径。
> 3. 定义了主函数do_it，用于调用NX的UF_WEB_author_html接口，根据一个HTML模板文件生成当前部件的HTML报告，并保存到临时文件路径下。
> 4. 定义了ufusr函数，作为NX的二次开发入口函数，在初始化NX环境后调用do_it函数，然后终止NX环境。
> 5. 在main函数中，首先初始化NX环境，然后遍历命令行传入的部件文件路径，打开每个部件，调用do_it函数生成HTML报告，最后关闭所有部件并终止NX环境。
> 6. 使用UF_PART_load_status_t结构体来获取部件的加载状态，并打印加载失败的部件和错误信息。
> 7. 提供了在UG/Manager模式下运行的示例命令行。
> 8. 使用UF_UNLOAD_IMMEDIATELY标志来允许立即卸载二次开发库。
>
> 这段代码的主要目的是实现一个简单的NX二次开发程序，用于根据部件文件生成HTML报告。
>
