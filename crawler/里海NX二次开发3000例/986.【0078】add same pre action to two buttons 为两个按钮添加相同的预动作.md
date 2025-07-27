### 【0078】add same pre action to two buttons 为两个按钮添加相同的预动作

#### 代码

```cpp
    /*HEAD CALL_SAME_ACTION_FROM_TWO_BUTTONS CCC UFUN */  
    /*  
        This program demonstrates adding a menuscript callback action to two  
        different menu options.  The shared library for this program with the  
        proper platform specific extension and the corresponding menufile  
        containing the following lines should be in the startup subdirectory  
        of a directory listed in UGII_CUSTOM_DIRECTORY_FILE.  
    VERSION 120  
    EDIT UG_GATEWAY_MAIN_MENUBAR  
    MODIFY  
        BUTTON UG_FILE_OPEN  
        ACTIONS/PRE my_callback  
        BUTTON UG_FILE_NEW  
        ACTIONS/PRE my_callback  
    END_OF_MODIFY  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_exit.h>  
    #include <uf_ui.h>  
    #include <uf_mb.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    /* 里海 */  
    static UF_MB_cb_status_t my_cb(UF_MB_widget_t widget,  
        UF_MB_data_t client_data, UF_MB_activated_button_p_t call_button )  
    {  
        if (UF_CALL(UF_initialize())) return UF_MB_CB_ERROR;  
        ECHO("Selected button: %s\n", call_button->name);  
        UF_terminate();  
        return UF_MB_CB_CONTINUE;  
    }  
    /* 里海 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        UF_MB_action_t  
            actionTable[] = {{ "ButtonName", my_cb, NULL },  
                             { NULL, NULL, NULL }};  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_MB_add_actions(actionTable));  
        UF_terminate();  
    }

```

#### 代码解析

> 根据代码内容，这是一段用于NX Open的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个ECHO函数，用于在列表窗口和系统日志中输出信息，方便调试。
> 2. 定义了一个report_error函数，用于在函数调用出错时报告错误信息。
> 3. 定义了一个my_cb函数，作为菜单按钮的回调函数。当按钮被点击时，该函数会被调用，并输出被点击的按钮名称。
> 4. 定义了一个ufsta函数，该函数会调用UF_MB_add_actions函数，将my_cb函数注册为菜单按钮的回调函数。注册后，当按钮被点击时，my_cb会被调用。
> 5. 在注释中说明，需要在menufile中修改UG_FILE_OPEN和UG_FILE_NEW按钮，添加一个预动作my_callback，这个动作对应于my_cb函数。
> 6. 通过修改menufile并包含这段代码，当点击UG_FILE_OPEN或UG_FILE_NEW按钮时，my_cb会被调用，并输出被点击的按钮名称。
>
> 总的来说，这段代码实现了一个简单的NX Open二次开发示例，演示了如何为菜单按钮添加回调函数，并在按钮被点击时执行自定义操作。
>
