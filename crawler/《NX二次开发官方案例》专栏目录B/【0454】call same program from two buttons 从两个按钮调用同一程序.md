### 【0454】call same program from two buttons 从两个按钮调用同一程序

#### 代码

```cpp
    /*HEAD CALL_SAME_PROGRAM_FROM_TWO_BUTTONS CCC UFUN */  
    /*  
        This program demonstrates adding menuscript callback actions.  The  
        shared library for this program with the proper platform specific  
        extension and the corresponding menufile containing the following lines  
        should be in the startup subdirectory of UGII_USER_DIR.  
    VERSION 120  
    EDIT UG_GATEWAY_MAIN_MENUBAR  
    AFTER UG_FILE_RUN  
        BUTTON API_CB1  
        LABEL Test first API callback  
        ACTIONS my_callback1  
        BUTTON API_CB2  
        LABEL Test second API callback  
        ACTIONS my_callback2  
    END_OF_AFTER  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18中的新增功能，请只回答翻译，无需添加任何评论。 */  
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
    static void do_it(int from_where)  
    {  
        char  
            msg[133];  
        sprintf(msg, "In program called by button %d", from_where);  
        uc1601(msg, TRUE);  
    }  
    static UF_MB_cb_status_t my_cb1(UF_MB_widget_t widget,  
        UF_MB_data_t client_data, UF_MB_activated_button_p_t call_button )  
    {  
        if (UF_CALL(UF_initialize())) return UF_MB_CB_ERROR;  
        do_it(1);  
        UF_terminate();  
        return UF_MB_CB_CONTINUE;  
    }  
    static UF_MB_cb_status_t my_cb2(UF_MB_widget_t widget,  
        UF_MB_data_t client_data, UF_MB_activated_button_p_t call_button )  
    {  
        if (UF_CALL(UF_initialize())) return UF_MB_CB_ERROR;  
        do_it(2);  
        UF_terminate();  
        return UF_MB_CB_CONTINUE;  
    }  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        UF_MB_action_t  
            actionTable[] = {{ "my_callback1", my_cb1, NULL },  
                             { "my_callback2", my_cb2, NULL },  
                             { NULL, NULL, NULL }};  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_MB_add_actions(actionTable));  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是一个NX Open菜单脚本示例，主要实现了以下功能：
>
> 1. 定义了两个按钮API_CB1和API_CB2，它们分别添加到UG_GATEWAY_MAIN_MENUBAR的UG_FILE_RUN菜单后面。
> 2. 为这两个按钮定义了回调函数my_cb1和my_cb2，当按钮被点击时，会执行对应的回调函数。
> 3. 在回调函数中，会调用UF_initialize初始化NX Open环境，然后调用do_it函数输出一条信息，最后调用UF_terminate终止NX Open环境。
> 4. ufsta函数负责向NX Open环境注册这两个回调函数，使按钮能够响应点击事件。
> 5. report_error函数用于输出错误信息。
> 6. do_it函数用于输出按钮编号。
>
> 通过这个示例，演示了如何在NX菜单栏中添加自定义按钮，并为按钮添加回调函数，实现与NX交互的功能。
>
