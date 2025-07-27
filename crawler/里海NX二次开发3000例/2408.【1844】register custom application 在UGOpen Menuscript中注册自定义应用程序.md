### 【1844】register custom application 在UGOpen Menuscript中注册自定义应用程序

#### 代码

```cpp
    /*HEAD REGISTER_CUSTOM_APPLICATION CCC UFUN */  
    /*  
        Save the menuscript file lines below to a file using a ".men" extension  
        in a "startup" subdirectory of a directory listed in either  
        $UGII_UG_CUSTOM_DIRECTORY_FILE or $UGII_CUSTOM_DIRECTORY_FILE.  
        E.g. $UGII_USER_DIR/startup  
        For more information on the "startup" subdirectory or the  
        UGII_CUSTOM_DIRECTORY_FILE environment variable, consult the  
        UG/Open Menuscript documentation.  
    ! Begin menufile  
    VERSION 120  
    EDIT UG_GATEWAY_MAIN_MENUBAR  
    MENU UG_APPLICATION  
    ! APPLICATION_BUTTON name must match the name you registered for  
    ! your application in the UF_MB_register_application call in the  
    ! program below.  
        APPLICATION_BUTTON  CUSTOM_APPLICATION  
        LABEL Custom Application...  
        LIBRARIES register_custom_application  
    END_OF_MENU  
    ! end of menufile  
         Place the shared library for the program below in a "application"  
         subdirectory of a directory listed in either  
         $UGII_UG_CUSTOM_DIRECTORY_FILE or $UGII_CUSTOM_DIRECTORY_FILE.  E.g.  
         $UGII_USER_DIR/application The shared library name must match the  
         LIBRARIES in the menuscript file, in this case  
         "register_custom_application", with the proper platform specific  
         extension.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_exit.h>  
    #include <uf_mb.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，以下是翻译：

注意：UF_print_syslog是V18中新增的函数，只提供翻译，不要写任何废话。 */  
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
    static void init_custom_app( void )  
    {  
        uc1601("Initializing Custom Application", TRUE);  
    }  
    static void exit_custom_app( void )  
    {  
        uc1601("Exiting Custom Application", TRUE);  
    }  
    static void enter_custom_app( void)  
    {  
        uc1601("Entering Custom Application", TRUE);  
    }  
    /* ARGSUSED 里海译:ARGSUSED */  
    extern void ufsta( char *param, int *retcod, int param_len )  
    {  
        UF_MB_application_t  
            app;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_MB_init_application_data(&app));  
        app.id = NULL;  
        app.name = "CUSTOM_APPLICATION"; /* APPLICATION_BUTTON name in .men file 里海译:根据.men文件，APPLICATION_BUTTON的翻译为“应用程序按钮”。 */  
        app.init_proc = init_custom_app;  
        app.exit_proc = exit_custom_app;  
        app.enter_proc = enter_custom_app;  
        app.drawings_supported = TRUE;  
        app.design_in_context_supported = TRUE;  
        app.supports_undo = TRUE;  
        UF_CALL(UF_MB_register_application(&app));  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 注册自定义应用程序：在NX的主菜单栏中添加一个名为"Custom Application"的按钮，当用户点击这个按钮时，会调用UF_MB_register_application来注册一个自定义的应用程序。
> 2. 定义应用程序的生命周期函数：包括init_custom_app、exit_custom_app和enter_custom_app，这些函数会在应用程序初始化、退出和进入时被调用。
> 3. 错误处理：使用report_error函数来打印NX API调用失败的错误信息。
> 4. 日志打印：使用UF_print_syslog函数来打印信息到NX日志窗口。
> 5. 在菜单脚本中引用：在菜单脚本文件中引用了"register_custom_application"这个共享库，NX在启动时会加载这个库并调用ufsta函数。
> 6. 应用程序功能声明：在注册应用程序时，声明了应用程序支持绘图、上下文设计以及支持撤销功能。
>
> 总的来说，这段代码提供了一个简单的NX二次开发示例，通过自定义应用程序按钮并在NX中添加功能。
>
