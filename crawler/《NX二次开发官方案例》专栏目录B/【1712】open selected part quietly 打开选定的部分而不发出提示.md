### 【1712】open selected part quietly 打开选定的部分而不发出提示

#### 代码

```cpp
/*HEAD OPEN_SELECTED_PART_QUIETLY CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_part.h>
#include <uf_exit.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中新增的功能，用于打印系统日志。该功能允许用户打印系统日志文件中的内容，而无需直接访问系统日志文件。它是一个新添加的函数，用于方便地打印系统日志信息。 */
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
static void report_load_status(UF_PART_load_status_p_t status)
{
    char
        msg[133];
    int
        ii;
    if (status->n_parts > 0)
    {
        UF_UI_open_listing_window();
        UF_UI_write_listing_window("Load notes:\n");
        for (ii = 0; ii < status->n_parts; ii++)
        {
            UF_UI_write_listing_window("  ");
            UF_UI_write_listing_window(status->file_names[ii]);
            UF_UI_write_listing_window(" - ");
            UF_get_fail_message(status->statuses[ii], msg);
            UF_UI_write_listing_window(msg);
            UF_UI_write_listing_window("\n");
        }
        UF_UI_write_listing_window("\n");
        UF_free(status->statuses);
        UF_free_string_array(status->n_parts, status->file_names);
    }
}
static void do_it(void)
{
    int
        resp;
    logical
        disp = FALSE;
    tag_t
        part;
    char
        file_name[MAX_FSPEC_SIZE + 1] = { "" };
    UF_PART_load_status_t
        status;
    if (!(UF_CALL(UF_UI_ask_open_part_filename(file_name, &disp, &resp))) &&
         (resp == UF_UI_OK) &&
         (UF_CALL(UF_PART_open_quiet(file_name, &part, &status)))
         || status.n_parts > 0)
        report_load_status(&status);
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

> 这是NX二次开发代码，其主要功能是使用户可以打开指定的NX部件文件，并在打开失败时显示错误信息。
>
> 主要特点如下：
>
> 1. 引入了头文件，用于获取NX提供的二次开发接口函数。
> 2. 定义了一个错误报告函数，用于在函数调用失败时显示错误信息。
> 3. 定义了一个报告加载状态函数，用于显示部件文件加载的状态信息。
> 4. 主函数do_it中，首先提示用户选择要打开的部件文件名，然后尝试静默打开指定的部件文件。若打开失败，则报告错误信息。
> 5. 使用NX提供的UI接口，在列表窗口中显示错误信息，以便用户查看。
> 6. 在ufusr函数中，初始化NX二次开发环境，调用do_it函数执行主功能，最后终止NX二次开发环境。
> 7. 定义了卸载函数ufusr_ask_unload，在卸载NX二次开发环境时调用。
>
> 总体来说，这段代码实现了基本的打开NX部件文件功能，并能够友好地处理打开失败的情况，使用户可以了解失败原因。
>
