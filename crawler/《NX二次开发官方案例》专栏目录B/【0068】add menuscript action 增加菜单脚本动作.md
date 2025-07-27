### 【0068】add menuscript action 增加菜单脚本动作

#### 代码

```cpp
    /*HEAD ADD_MENUSCRIPT_ACTION CCC UFUN */  
    /*  
        This program demonstrates adding a menuscript callback action.  The  
        shared library for this program with the proper platform specific  
        extension and the corresponding menufile containing the following lines  
        should be in the startup subdirectory of UGII_USER_DIR.  
        NOTE: if you have UGII_OPTION set to LOWER, your registered action name  
              must be all in lower case in order to be found.  
    VERSION 120  
    EDIT UG_GATEWAY_MAIN_MENUBAR  
    AFTER UG_FILE_RUN  
        BUTTON API_CB  
        LABEL Test API callback  
        ACCELERATOR Ctrl+Alt+T  
        ACTIONS my_callback  
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
    static UF_MB_cb_status_t my_cb(UF_MB_widget_t widget,  
        UF_MB_data_t client_data, UF_MB_activated_button_p_t call_button )  
    {  
        if (UF_CALL(UF_initialize())) return UF_MB_CB_ERROR;  
        uc1601("In my callback", TRUE);  
        UF_terminate();  
        return UF_MB_CB_CONTINUE;  
    }  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        UF_MB_action_t  
            actionTable[] = {{ "my_callback", my_cb, NULL },  
                             { NULL, NULL, NULL }};  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_MB_add_actions(actionTable));  
        UF_terminate();  
    }

```

#### 代码解析

> 这段NX二次开发代码的功能是向NX的菜单栏中添加一个新的菜单项。代码的介绍如下：
>
> 1. 包含必要的头文件，如uf.h、uf_exit.h、uf_ui.h、uf_mb.h等，这些头文件包含了NX提供的二次开发接口。
> 2. 定义了一个错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 3. 定义了一个菜单项回调函数my_cb，当用户点击该菜单项时，会调用这个函数。该函数首先初始化NX环境，然后打印一条信息"In my callback"，最后退出NX环境。
> 4. 定义了一个注册函数ufsta，该函数向NX的菜单栏中添加一个名为"my_callback"的菜单项，并指定其回调函数为my_cb。
> 5. 在NX的启动目录下，需要提供一个menupath文件，在该文件中通过UG_GATEWAY_MAIN_MENUBAR指令指定NX的主菜单栏，并在UG_FILE_RUN菜单项之后通过AFTER指令添加一个新的按钮，按钮标签为"Test API callback"，快捷键为Ctrl+Alt+T，并指定其动作名称为"my_callback"。
> 6. 当用户点击该按钮时，会触发my_cb函数的调用，从而实现自定义的功能。
>
> 通过这段代码，开发人员可以向NX的菜单栏中添加自定义的菜单项，实现二次开发功能。
>
