### 【1562】load utd file 加载UTD文件

#### 代码

```cpp
Sample Open C program : load utd file
/*HEAD LOAD_UTD_FILE CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
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
static void do_it(void)
{
    int
        tool_num = 0;
    char
        *tool_name = "D:\\\path\\\to\\\my.utd";
    logical
        map_flag = UF_UI_SHOW,
        read_flag;
   /*
   NOTE:  You must specify the complete path to the .utd file.
   If you just specify the filename and put the file in a valid
   "application" directory, it will not be found.
   */
    UF_CALL(UF_UI_create_usertool(tool_num, tool_name, map_flag, &read_flag));
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

> 这段代码是一个NX的二次开发示例，主要功能是加载一个用户定义的工具(UTD)文件。
>
> 代码的主要功能包括：
>
> 1. 包含必要的NX头文件，如uf.h、uf_ui.h等。
> 2. 定义了一个错误报告函数report_error，用于打印错误信息。
> 3. 定义了主函数do_it，在该函数中，指定了UTD文件的路径和名称，并使用UF_UI_create_usertool函数来创建用户工具。同时，设置了显示标志，用于控制是否在NX界面中显示该工具。
> 4. 定义了ufusr函数，这是NX二次开发的入口函数。在函数中初始化NX环境，调用do_it函数加载UTD文件，然后终止NX环境。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示该二次开发模块被卸载时不需要进行清理工作。
>
> 总的来说，这段代码通过调用NX提供的UF接口，实现了加载UTD文件的功能，为NX添加了自定义工具，扩展了NX的功能。代码结构清晰，包含了错误处理和二次开发模块的基本框架。
>
