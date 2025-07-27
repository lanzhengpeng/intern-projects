### 【0624】create category 创建类别

#### 代码

```cpp
/*HEAD CREATE_CATEGORY CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_layer.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数。 */
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
        ii;
    tag_t
        category;
    UF_LAYER_category_info_t
        category_info;
    strcpy(category_info.name, "1_TEST");
    category_info.layer_mask[0] = TRUE;
    for (ii = 1; ii < 256; ii++) category_info.layer_mask[ii] = FALSE;
    strcpy(category_info.descr, "name begins with a number");
    UF_CALL(UF_LAYER_create_category(&category_info, &category));
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 错误报告：使用了report_error函数来封装错误报告逻辑，当UF函数调用失败时，会记录文件名、行号、函数名、错误码等信息，并通过UF_print_syslog打印到系统日志中，同时尝试打开NX的Listing窗口并显示错误信息。
> 2. 创建图层类别：do_it函数中，创建了一个名为"1_TEST"的图层类别，其描述为"名称以数字开头"，并指定只有第1个图层被选中，其他图层都不被选中。
> 3. UF初始化和终止：在ufusr函数中，调用UF_initialize进行初始化，然后调用do_it执行创建图层类别的操作，最后调用UF_terminate终止。
> 4. 卸载函数：ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示NX可以在不需要时立即卸载该UF程序。
> 5. 代码规范：代码遵循了良好的编程规范，包括函数注释、错误处理、日志输出等。整体结构清晰，功能单一，适合作为NX二次开发的一个简单示例。
>
