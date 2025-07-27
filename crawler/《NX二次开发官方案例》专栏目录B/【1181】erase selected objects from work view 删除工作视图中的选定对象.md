### 【1181】erase selected objects from work view 删除工作视图中的选定对象

#### 代码

```cpp
/*HEAD ERASE_SELECTED_OBJECTS_FROM_WORK_VIEW CCC UFUN */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_disp.h>
#include <uf_view.h>
#define ECHO(X)    (printf("%s = %d\n", #X, X))
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的功能。 */
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
static int select_objects(char *prompt, tag_t **objects)
{
    int
        cnt,
        ii,
        resp;
    UF_CALL(UF_UI_select_with_class_dialog( prompt, "",
        UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));
    for (ii = 0; ii < cnt; ii++)
        UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));
    return cnt;
}
static void do_it(void)
{
    int
        ii,
        n;
    tag_t
        *objects;
    while ((n = select_objects("Select objects to erase", &objects)) > 0)
    {
        for (ii = 0; ii < n; ii++) ECHO(uc6400("", objects[ii]));
        UF_free(objects);
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

> 根据代码内容，这是用于NX 10的二次开发代码，主要功能是使用对话框选择工作视图中的对象，并将它们从工作视图中移除。
>
> 主要步骤包括：
>
> 1. 包含必要的头文件。
> 2. 定义错误报告函数report_error，用于打印出调用UF函数出错的信息。
> 3. 定义选择对象函数select_objects，用于打开对话框选择工作视图中的对象，并高亮显示所选对象。
> 4. 定义执行函数do_it，用于循环调用select_objects选择对象，然后移除工作视图中的对象。
> 5. 定义ufusr函数，作为程序的入口，用于初始化NX Open，执行do_it，然后终止NX Open。
> 6. 定义ufusr_ask_unload函数，返回立即卸载标志，表示不需要延迟卸载。
>
> 代码中使用了UF_DISP_set_highlight来高亮显示所选对象，然后使用uc6400函数将对象从工作视图中移除。同时，错误报告函数也使用了NX 10新增的UF_print_syslog函数来打印错误信息。
>
