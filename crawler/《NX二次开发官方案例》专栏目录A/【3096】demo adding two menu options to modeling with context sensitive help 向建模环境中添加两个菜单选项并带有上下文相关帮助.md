### 【3096】demo adding two menu options to modeling with context sensitive help 向建模环境中添加两个菜单选项并带有上下文相关帮助

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_exit.h>  
    #include <uf_mb.h>  
    #include <uf_help.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static logical ask_yes_or_no(char *prompt, logical *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "Yes", "No" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if (resp > 4)  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static UF_MB_cb_status_t GTAC_EXT__action1(  
        UF_MB_widget_t             widget,  
        UF_MB_data_t               client_data,  
        UF_MB_activated_button_p_t call_button )  
    {  
        logical  
            resp = TRUE;  
        if (!UF_CALL(UF_initialize()))  
        {  
            ECHO("%s\n", "pushing help context: gtac_ext_option_1");  
            UF_CALL(UF_HELP_push_context("gtac_ext_option_1"));  
            while (ask_yes_or_no("In Option 1 - Stay here?", &resp));  
            ECHO("%s\n", "Popping help context");  
            UF_CALL(UF_HELP_pop_context());  
            UF_terminate();  
        }  
        return( UF_MB_CB_CONTINUE );  
    }  
    static UF_MB_cb_status_t GTAC_EXT__action2(  
        UF_MB_widget_t             widget,  
        UF_MB_data_t               client_data,  
        UF_MB_activated_button_p_t call_button )  
    {  
        logical  
            resp = TRUE;  
        if (!UF_CALL(UF_initialize()))  
        {  
            ECHO("%s\n", "pushing help context: gtac_ext_option_2");  
            UF_CALL(UF_HELP_push_context("gtac_ext_option_2"));  
            while (ask_yes_or_no("In Option 2 - Stay here?", &resp));  
            ECHO("%s\n", "Popping help context");  
            UF_CALL(UF_HELP_pop_context());  
            UF_terminate();  
        }  
        return( UF_MB_CB_CONTINUE );  
    }  
    static void do_it(void)  
    {  
        char  
            *map_spec;  
        UF_MB_action_t  
            actionTable[] = {  
                { "GTAC_EXT__action1", GTAC_EXT__action1, NULL},  
                { "GTAC_EXT__action2", GTAC_EXT__action2, NULL},  
                { NULL, NULL, NULL } };  
        UF_CALL(UF_MB_add_actions( actionTable ));  
        if (!UF_CALL(UF_find_file("help", "help.map", &map_spec)))  
        {  
            ECHO("Loading help map file: %s\n", map_spec);  
            UF_CALL(UF_HELP_load_map_file(map_spec));  
            UF_free(map_spec);  
        }  
        else  
        {  
            ECHO("Unable to find help\\help.map file");  
        }  
    }  
    extern void ufsta( char *param, int *retcod, int param_len )  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 包含了NX二次开发常用的头文件。
> 2. 定义了ECHO宏，用于在列表窗口输出信息。
> 3. 定义了report_error函数，用于报告函数调用的错误。
> 4. 定义了ask_yes_or_no函数，用于询问用户Yes或No。
> 5. 定义了GTAC_EXT__action1和GTAC_EXT__action2函数，分别对应功能1和功能2。
> 6. 在do_it函数中，加载了帮助映射文件help.map。
> 7. 定义了ufsta函数，作为二次开发的入口函数。在ufsta中，初始化NX系统，调用do_it实现功能，然后终止NX系统。
> 8. ufsta函数作为二次开发的入口，会被NX系统调用。
>
> 总的来说，这段代码实现了NX系统的初始化，加载帮助映射文件，定义了两个功能函数，并通过ufsta函数作为入口来调用这些功能。
>
