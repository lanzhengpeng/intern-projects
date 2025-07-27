### 【2905】specify a csys 指定一个坐标系

#### 代码

```cpp
/*****************************************************************************
**
** specify_a_csys.c
**
** Description:
**     Contains Unigraphics entry points for the application.
**
** Notes:
** PR-8423236 uc1630 / UF_UI_specify_csys cannot select interpart face
** Find until NX11.0.2, Fixed NX1899
** 
*****************************************************************************/
/* Include files 里海译:Include files 翻译为“包含文件”。 */
#include <stdarg.h>
#include <stdio.h>
#include <uf.h>
#include <uf_ui_types.h>
#include <uf_ui.h>
static void ECHO(char *format, ...)
{
    char msg[UF_UI_MAX_STRING_BUFSIZE];
    va_list args;
    va_start(args, format);
    vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);
    va_end(args);
    UF_UI_open_listing_window();
    UF_UI_write_listing_window(msg);
    UF_print_syslog(msg, FALSE);
}
#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
static int report_error( char *file, int line, char *call, int irc)
{
    if (irc)
    {
        char err[133];
        UF_get_fail_message(irc, err);
        ECHO("*** ERROR code %d at line %d in %s:\n",
            irc, line, file);
        ECHO("+++ %s\n", err);
        ECHO("%s;\n", call);
    }
    return(irc);
}
/*****************************************************************************
**  Activation Methods
*****************************************************************************/
/*  Explicit Activation
**      This entry point is used to activate the application explicitly, as in
**      "File->Execute UG/Open->User Function..." */
extern DllExport void ufusr( char *parm, int *returnCode, int rlen )
{
    /* Initialize the API environment 里海译:初始化API环境 */
    if( UF_CALL(UF_initialize()) ) 
    {
        /* Failed to initialize 里海译:初始化失败 */
        return;
    }
    /* TODO: Add your application code here 里海译:翻译为：TODO：在此处添加您的应用程序代码 */
    int option = 13, resp = 0, i;
    double csys_matrix[9];
    double origin[3];
    tag_t csys_tag = NULL_TAG;
    resp = uc1630("uc1630", &option, csys_matrix, origin);
    if (resp == 3)
    {
        ECHO("uc1630 used option %d\n", option);
        ECHO(" Matrix:\n");
        for (i = 0; i < 3; i++)
            ECHO("  %d, %d, %d\n", csys_matrix[i * 3], csys_matrix[i * 3 + 1], csys_matrix[i * 3 + 2]);
        ECHO(" Origin\n:");
        ECHO("  %d, %d, %d\n", origin[0], origin[1], origin[2]);
    }
    resp = UF_UI_specify_csys("UF_UI_specify_csys", &option, csys_matrix, origin, &csys_tag);
    if (resp == 3)
    {
        ECHO("UF_UI_specify_csys used option %d\n", option);
        ECHO(" Matrix:\n");
        for (i = 0; i < 3; i++)
            ECHO("  %d, %d, %d\n", csys_matrix[i * 3], csys_matrix[i * 3 + 1], csys_matrix[i * 3 + 2]);
        ECHO(" Origin:\n");
        ECHO("  %d, %d, %d\n", origin[0], origin[1], origin[2]);
    }
    /* Terminate the API environment 里海译:终止API环境 */
    UF_CALL(UF_terminate());
}
/*****************************************************************************
**  Utilities
*****************************************************************************/
/* Unload Handler
**     This function specifies when to unload your application from Unigraphics.
**     If your application registers a callback (from a MenuScript item or a
**     User Defined Object for example), this function MUST return
**     "UF_UNLOAD_UG_TERMINATE". */
extern int ufusr_ask_unload( void )
{
    return( UF_UNLOAD_IMMEDIATELY );
}

```

#### 代码解析

> 这段代码是用于Unigraphics NX的二次开发，主要包括以下内容：
>
> 1. 包含文件：引入了必要的头文件，如Unigraphics API的UF.h和UF_UI.h。
> 2. 错误处理函数：定义了report_error函数，用于打印出函数调用失败时的错误信息。
> 3. UFusr主函数：作为程序的入口点，用于初始化和终止Unigraphics API，并调用自定义函数uc1630和UF_UI_specify_csys。其中，UF_UI_specify_csys函数用于指定用户坐标系。
> 4. 卸载处理函数：定义了ufusr_ask_unload函数，用于在程序卸载时处理回调等。
> 5. ECHO函数：用于打印日志信息到Unigraphics的输出窗口和日志文件。
> 6. 代码注释：提供了详细的中文注释，有助于理解代码的作用。
>
> 总体来说，这段代码实现了在Unigraphics NX中指定用户坐标系的功能，采用了标准的Unigraphics二次开发流程，包括API的初始化、自定义函数的调用、以及API的终止。注释的中文翻译有助于更好地理解代码。
>
