### 【1066】disallow file new in nxmanager only 禁止在NX Manager中创建新文件

#### 代码

```cpp
    /*HEAD DISALLOW_FILE_NEW_IN_NXMANAGER_ONLY CCC UFUN */  
    /*  
        This program demonstrates adding a menuscript callback action to the  
        File-> New option which will only allow the STANDARD action to  
        happen when the program is being run in native mode.  
    VERSION 120  
    EDIT UG_GATEWAY_MAIN_MENUBAR  
    MODIFY  
        BUTTON UG_FILE_NEW  
        ACTIONS/PRE this_callback  
    END_OF_MODIFY  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_exit.h>  
    #include <uf_ui.h>  
    #include <uf_mb.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。其功能为在系统日志中打印信息。 */  
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
    /* qq3123197280 */  
    static UF_MB_cb_status_t this_cb(UF_MB_widget_t widget,  
        UF_MB_data_t client_data, UF_MB_activated_button_p_t call_button )  
    {  
        logical  
            in_nxmanager;  
        if (UF_CALL(UF_initialize())) return UF_MB_CB_ERROR;  
        UF_CALL(UF_is_ugmanager_active(&in_nxmanager));  
        if (in_nxmanager)  
            uc1601("Please create new item using Portal interface", TRUE);  
        UF_terminate();  
        if (in_nxmanager)  
            return UF_MB_CB_CANCEL;  
        else  
            return UF_MB_CB_CONTINUE;  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        UF_MB_action_t  
            actionTable[] = {{ "this_callback", this_cb, NULL },  
                             { NULL, NULL, NULL }};  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_MB_add_actions(actionTable));  
        UF_terminate();  
    }

```

#### 代码解析

> 这段NX Open API二次开发代码实现了以下功能：
>
> 1. 修改菜单项行为：通过修改UG_GATEWAY_MAIN_MENUBAR菜单栏中的UG_FILE_NEW按钮，添加了一个预回调函数this_callback，以在点击“文件”->“新建”菜单项时执行。
> 2. 自定义回调函数：自定义了回调函数this_callback，用于检查NX是否处于UGManager管理模式。若处于管理模式，则弹窗提示使用Portal界面创建新项目，并返回取消状态，阻止标准新建操作；若不在管理模式，则返回继续状态，允许执行标准新建操作。
> 3. 错误处理和日志打印：通过report_error函数实现了错误处理，能够打印错误信息到系统日志和控制台窗口。另外，使用了UF_print_syslog函数在系统日志中打印信息，这是NX 12.0新增的函数。
> 4. 菜单动作的动态添加：通过ufsta函数动态添加了菜单动作表，将this_callback函数关联到“this_callback”动作名。
> 5. NX模式检查：利用UF_is_ugmanager_active函数判断NX是否处于UGManager管理模式。
> 6. 函数命名约定：遵循NX Open API的函数命名约定，如UF_MB_add_actions。
>
> 总的来说，这段代码通过二次开发修改了NX的菜单行为，实现了在UGManager管理模式下禁用标准新建操作，只允许通过Portal界面创建新项目。
>
