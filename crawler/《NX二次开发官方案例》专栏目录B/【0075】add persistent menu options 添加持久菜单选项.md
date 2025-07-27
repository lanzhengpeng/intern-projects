### 【0075】add persistent menu options 添加持久菜单选项

#### 代码

```cpp
    /*  
        This program demonstrates adding a persistent menu to the top  
        menubar where the options on the menu can be called whenever  
        a part is active during the NX session.  
        The corresponding menufile looks like this:  
    VERSION 121  
    EDIT UG_GATEWAY_MAIN_MENUBAR  
    BEFORE UG_HELP  
        CASCADE_BUTTON  GTAC_OPTIONS  
        LABEL GTAC Opts  
    END_OF_BEFORE  
    MENU GTAC_OPTIONS  
        BUTTON GTAC_ONE  
        LABEL Report Current Part  
        SENSITIVITY OFF  
        ACTIONS GTAC_cust_op1  
        BUTTON GTAC_TWO  
        LABEL Report Current Application  
        SENSITIVITY OFF  
        ACTIONS GTAC_cust_op2  
    END_OF_MENU  
        Place both the menufile and the shared library built from this code  
        in any startup folder under a folder listed in  
        UGII_CUSTOM_DIRECTORY_FILE or UGII_UG_CUSTOM_DIRECTORY_FILE, e.g.  
        UGII_USER_DIR\startup.  
    To run these from Toolbar buttons, just name the menufile defined buttons in  
    the .tbr file.  For example:  
        TITLE  My Tools  
        VERSION 160  
        DOCK TOP  
        BUTTON  GTAC_ONE  
        BUTTON  GTAC_TWO  
    Do not use the ACTION statement in the Toolbar file as it is already defined by  
    the menuscript file.  A toolbar ACTION cannot be a custom callback.  The  
    menuscript mechanism must be used to define the ACTIONS for the BUTTON.  
    If you want only the toolbar buttons and not the menu bar options to be seen,  
    add a final line to the .men file:  
        HIDE GTAC_OPTIONS  
    This will hide the drop down menu, but the toolbar buttons will still be  
    active.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_exit.h>  
    #include <uf_mb.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static UF_MB_cb_status_t GTAC_option1(  
        UF_MB_widget_t             widget,  
        UF_MB_data_t               client_data,  
        UF_MB_activated_button_p_t call_button )  
    {  
        tag_t  
            part = NULL_TAG;  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        if (UF_CALL(UF_initialize()) ||  
            (UF_UI_lock_ug_access( UF_UI_FROM_CUSTOM ) != UF_UI_LOCK_SET))  
                return (UF_MB_CB_CONTINUE);  
        ECHO("Selected button GTAC_ONE - active part is ");  
        part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, fspec);  
        ECHO(fspec);  
        ECHO("\n");  
        UF_UI_unlock_ug_access(UF_UI_FROM_CUSTOM);  
        UF_terminate();  
        return (UF_MB_CB_CONTINUE);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    static void report_current_application(void)  
    {  
        int  
            ii,  
            module,  
            a_module;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            module_name[133],  
            ufh_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        UF_CALL(UF_ask_application_module(&module));  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf.h", ufh_file));  
        if ((in_file = fopen(ufh_file, "r")) == NULL)  
        {  
            WRITE_D(module);  
            ECHO("(uf.h not found - cannot get module name)\n");  
            return;  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_APP_%s (%d) /* %[^*] ", define,  
                &a_module, module_name)) == 3)  
            {  
                if (a_module == module)  
                {  
                    ECHO(module_name);  
                    ECHO("\n");  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
    }  
    static UF_MB_cb_status_t GTAC_option2(  
        UF_MB_widget_t             widget,  
        UF_MB_data_t               client_data,  
        UF_MB_activated_button_p_t call_button )  
    {  
        if (UF_CALL(UF_initialize()) ||  
            (UF_UI_lock_ug_access( UF_UI_FROM_CUSTOM ) != UF_UI_LOCK_SET))  
                return (UF_MB_CB_CONTINUE);  
        ECHO("Selected button GTAC_TWO - from:  ");  
        report_current_application();  
        UF_UI_unlock_ug_access( UF_UI_FROM_CUSTOM );  
        UF_terminate();  
        return( UF_MB_CB_CONTINUE );  
    }  
    static void check_button_sensitivity(UF_callback_reason_e_t reason,  
        const void* part, void* user_data)  
    {  
        int  
            buttons[2],  
            ii,  
            toggle;  
        if (UF_CALL(UF_initialize())) return;  
        switch (UF_PART_ask_num_parts())  
        {  
            case 0:  
                toggle = UF_MB_OFF;  
                break;  
            case 1:  
                if (reason == UF_close_part_reason)  
                    toggle = UF_MB_OFF;  
                else  
                    toggle = UF_MB_ON;  
                break;  
            default:  
                toggle = UF_MB_ON;  
        }  
        UF_CALL(UF_MB_ask_button_id("GTAC_ONE", &buttons[0]));  
        UF_CALL(UF_MB_ask_button_id("GTAC_TWO", &buttons[1]));  
        for (ii = 0; ii < 2; ii++)  
        {  
            UF_CALL(UF_MB_set_button_sensitivity(buttons[ii], toggle));  
        }  
        UF_terminate();  
    }  
    /* 里海 */  
    extern void ufsta( char *param, int *retcod, int param_len )  
    {  
        UF_MB_action_t  
            actionTable[] =  
                { { "GTAC_cust_op1", GTAC_option1, NULL },  
                  { "GTAC_cust_op2", GTAC_option2, NULL },  
                  { NULL, NULL, NULL } };  
        UF_registered_fn_p_t  
            f_id;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_MB_add_actions( actionTable ));  
        UF_CALL(UF_add_callback_function(UF_create_part_reason,  
            check_button_sensitivity, NULL, &f_id));  
        UF_CALL(UF_add_callback_function(UF_open_part_reason,  
            check_button_sensitivity, NULL, &f_id));  
        UF_CALL(UF_add_callback_function(UF_close_part_reason,  
            check_button_sensitivity, NULL, &f_id));  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_UG_TERMINATE);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发示例，主要实现了以下功能：
>
> 1. 添加菜单和按钮：在NX的顶部菜单栏添加了一个名为"GTAC Options"的下拉菜单，并在其中添加了两个按钮"Report Current Part"和"Report Current Application"。
> 2. 定义按钮回调函数：为这两个按钮定义了回调函数GTAC_option1和GTAC_option2。GTAC_option1会获取并打印当前显示的部件名称，而GTAC_option2则会获取并打印当前NX模块的名称。
> 3. 设置按钮的敏感性：通过回调函数check_button_sensitivity，根据当前打开部件的数量来动态设置两个按钮的敏感性。如果没有打开部件，则将按钮设置为不可用。
> 4. 使用自定义动作：通过UF_MB_add_actions函数，将自定义的回调函数与菜单项的动作关联起来，实现了自定义按钮动作。
> 5. 添加部件状态变化回调：通过UF_add_callback_function，添加了部件打开、关闭和创建时的回调函数，用于在部件状态变化时更新按钮的敏感性。
> 6. 提供卸载函数：通过ufusr_ask_unload函数提供了卸载库的接口。
>
> 总的来说，这段代码展示了如何在NX中添加自定义菜单和按钮，并实现与NX部件状态绑定的自定义功能。它利用NX提供的菜单脚本和回调机制，实现了一个简单的二次开发示例。
>
