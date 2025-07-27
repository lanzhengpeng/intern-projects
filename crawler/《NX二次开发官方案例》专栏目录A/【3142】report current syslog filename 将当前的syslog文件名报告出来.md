### 【3142】report current syslog filename 将当前的syslog文件名报告出来

#### 代码

```cpp
/*HEAD REPORT_CURRENT_SYSLOG_FILENAME CCC UFUN */
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */
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
static void do_it(void)
{
        char
            *syslog = NULL;
    /* this function is available beginning with NX 2.0.5 里海译:从NX 2.0.5开始，此功能可用。 */
    UF_CALL( UF_ask_syslog_filename( &syslog ));
    WRITE( syslog );
    WRITE( "\n" );
    UF_free( syslog );
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

> 这是段NX二次开发代码，其主要功能是获取NX系统日志文件名并打印出来。代码的关键部分如下：
>
> 1. 定义了一个报告错误的函数report_error，用于打印出函数调用失败时的错误码和错误信息。
> 2. 定义了一个宏WRITE，用于打开NX的列表窗口并写入信息。
> 3. 定义了一个函数do_it，该函数使用UF_ask_syslog_filename函数获取系统日志文件名，然后使用WRITE宏打印出来。
> 4. 定义了ufusr函数，这是NX二次开发的标准入口函数。在该函数中初始化NX，调用do_it函数执行主要功能，然后终止NX。
> 5. 定义了ufusr_ask_unload函数，用于在卸载该二次开发时立即卸载。
>
> 总体来说，这段代码实现了获取和打印NX系统日志文件名的功能，并采用了错误处理和日志打印等二次开发常用的方法。
>
