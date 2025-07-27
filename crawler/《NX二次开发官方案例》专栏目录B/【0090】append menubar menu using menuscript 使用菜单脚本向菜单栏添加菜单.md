### 【0090】append menubar menu using menuscript 使用菜单脚本向菜单栏添加菜单

#### 代码

```cpp
    /*HEAD APPEND_MENUBAR_MENU_USING_MENUSCRIPT CCC UFUN */  
    /*  
        This program demonstrates adding a menu to the main menubar using  
        menuscript callback actions.  The shared library for this program with  
        the proper platform specific extension and the corresponding menufile  
        containing the following lines should be in the "startup" subdirectory  
        under any directory listed in UGII_CUSTOM_DIRECTORY_FILE or  
        UGII_UG_CUSTOM_DIRECTORY_FILE.  E.g.  %UGII_USER_DIR%\startup.  
    VERSION 120  
    EDIT UG_GATEWAY_MAIN_MENUBAR  
    AFTER UG_WINDOW  
        CASCADE_BUTTON API_CBS  
        LABEL &Sample  
    END_OF_AFTER  
    MENU API_CBS  
        BUTTON GTAC_CB1  
        LABEL Option 1  
        ACTIONS callback_one  
        BUTTON GTAC_CB2  
        LABEL Option 2  
        ACTIONS callback_two  
        BUTTON GTAC_CB3  
        LABEL Option 3  
        ACTIONS callback_three  
    END_OF_MENU  
    */  
    #include <stdio.h>  
    #include <string.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中新增的函数。 */  
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
    /* 里海 */  
    static UF_MB_cb_status_t api_cb1(UF_MB_widget_t widget,  
        UF_MB_data_t client_data, UF_MB_activated_button_p_t call_button )  
    {  
        if (UF_CALL(UF_initialize())) return UF_MB_CB_ERROR;  
        WRITE("Ran first callback...\n");  
        UF_terminate();  
        return UF_MB_CB_CONTINUE;  
    }  
    /* 里海 */  
    static UF_MB_cb_status_t api_cb2(UF_MB_widget_t widget,  
        UF_MB_data_t client_data, UF_MB_activated_button_p_t call_button )  
    {  
        if (UF_CALL(UF_initialize())) return UF_MB_CB_ERROR;  
        WRITE("Ran second callback...\n");  
        UF_terminate();  
        return UF_MB_CB_CONTINUE;  
    }  
    /* 里海 */  
    static UF_MB_cb_status_t api_cb3(UF_MB_widget_t widget,  
        UF_MB_data_t client_data, UF_MB_activated_button_p_t call_button )  
    {  
        if (UF_CALL(UF_initialize())) return UF_MB_CB_ERROR;  
        WRITE("Ran third callback...\n");  
        UF_terminate();  
        return UF_MB_CB_CONTINUE;  
    }  
    /* 里海 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        UF_MB_action_t  
            actionTable[] = {{ "callback_one", api_cb1, NULL },  
                             { "callback_two", api_cb2, NULL },  
                             { "callback_three", api_cb3, NULL },  
                             { NULL, NULL, NULL }};  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_MB_add_actions(actionTable));  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发，在NX主菜单栏中添加自定义菜单的示例代码。主要功能包括：
>
> 1. 定义了3个回调函数api_cb1、api_cb2、api_cb3，分别用于处理菜单项1、2、3的点击事件，并在列表窗口中显示相应信息。
> 2. 定义了一个action表actionTable，将回调函数名称与函数指针关联起来。
> 3. ufsta函数负责初始化NX环境，调用UF_MB_add_actions将action表添加到NX，从而实现自定义菜单项的回调绑定。
> 4. 使用UF_print_syslog和UF_UI_write_listing_window在列表窗口中输出错误信息或回调信息。
> 5. 通过在startup目录下放置menufile文件，在NX启动时自动加载并添加自定义菜单项。
> 6. 使用UF_MB_cb_status_t枚举表示回调函数的返回状态。
> 7. 使用宏UF_CALL和报告错误函数report_error来简化错误处理。
>
> 总体来说，这段代码通过自定义菜单和回调函数的方式，实现了NX界面和自定义代码的交互，为NX二次开发提供了便利。
>
