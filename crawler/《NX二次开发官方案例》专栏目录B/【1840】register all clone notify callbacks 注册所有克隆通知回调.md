### 【1840】register all clone notify callbacks 注册所有克隆通知回调

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_clone.h>  
    #include <stdarg.h>  
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
    //  There has got to be a better way!  
    static char *get_clone_notify_enum_name(UF_CLONE_notify_cb_t what)  
    {  
        switch (what)  
        {  
            case    UF_CLONE_initialise_cb:  
            return "UF_CLONE_initialise_cb";  
            case    UF_CLONE_terminate_cb:  
            return "UF_CLONE_terminate_cb";  
            case    UF_CLONE_beg_assy_load_cb:  
            return "UF_CLONE_beg_assy_load_cb";  
            case    UF_CLONE_end_assy_load_cb:  
            return "UF_CLONE_end_assy_load_cb";  
            case    UF_CLONE_beg_assy_nc_load_cb:  
            return "UF_CLONE_beg_assy_nc_load_cb";  
            case    UF_CLONE_end_assy_nc_load_cb:  
            return "UF_CLONE_end_assy_nc_load_cb";  
            case    UF_CLONE_beg_part_load_cb:  
            return "UF_CLONE_beg_part_load_cb";  
            case    UF_CLONE_end_part_load_cb:  
            return "UF_CLONE_end_part_load_cb";  
            case    UF_CLONE_beg_part_load_nc_cb:  
            return "UF_CLONE_beg_part_load_nc_cb";  
            case    UF_CLONE_end_part_load_nc_cb:  
            return "UF_CLONE_end_part_load_nc_cb";  
            case    UF_CLONE_beg_perform_cb:  
            return "UF_CLONE_beg_perform_cb";  
            case    UF_CLONE_end_perform_cb:  
            return "UF_CLONE_end_perform_cb";  
            case    UF_CLONE_beg_apply_defs_cb:  
            return "UF_CLONE_beg_apply_defs_cb";  
            case    UF_CLONE_end_apply_defs_cb:  
            return "UF_CLONE_end_apply_defs_cb";  
            case    UF_CLONE_beg_set_def_action_cb:  
            return "UF_CLONE_beg_set_def_action_cb";  
            case    UF_CLONE_end_set_def_action_cb:  
            return "UF_CLONE_end_set_def_action_cb";  
            case    UF_CLONE_beg_set_action_cb:  
            return "UF_CLONE_beg_set_action_cb";  
            case    UF_CLONE_end_set_action_cb:  
            return "UF_CLONE_end_set_action_cb";  
            case    UF_CLONE_beg_set_name_rule_cb:  
            return "UF_CLONE_beg_set_name_rule_cb";  
            case    UF_CLONE_end_set_name_rule_cb:  
            return "UF_CLONE_end_set_name_rule_cb";  
            case    UF_CLONE_beg_set_def_naming_cb:  
            return "UF_CLONE_beg_set_def_naming_cb";  
            case    UF_CLONE_end_set_def_naming_cb:  
            return "UF_CLONE_end_set_def_naming_cb";  
            case    UF_CLONE_beg_set_naming_cb:  
            return "UF_CLONE_beg_set_naming_cb";  
            case    UF_CLONE_end_set_naming_cb:  
            return "UF_CLONE_end_set_naming_cb";  
            case    UF_CLONE_beg_set_name_cb:  
            return "UF_CLONE_beg_set_name_cb";  
            case    UF_CLONE_end_set_name_cb:  
            return "UF_CLONE_end_set_name_cb";  
            case    UF_CLONE_beg_set_def_pdm_name_cb:  
            return "UF_CLONE_beg_set_def_pdm_name_cb";  
            case    UF_CLONE_end_set_def_pdm_name_cb:  
            return "UF_CLONE_end_set_def_pdm_name_cb";  
            case    UF_CLONE_beg_set_pdm_name_cb:  
            return "UF_CLONE_beg_set_pdm_name_cb";  
            case    UF_CLONE_end_set_pdm_name_cb:  
            return "UF_CLONE_end_set_pdm_name_cb";  
            case    UF_CLONE_beg_set_def_item_type_cb:  
            return "UF_CLONE_beg_set_def_item_type_cb";  
            case    UF_CLONE_end_set_def_item_type_cb:  
            return "UF_CLONE_end_set_def_item_type_cb";  
            case    UF_CLONE_beg_set_item_type_cb:  
            return "UF_CLONE_beg_set_item_type_cb";  
            case    UF_CLONE_end_set_item_type_cb:  
            return "UF_CLONE_end_set_item_type_cb";  
            case    UF_CLONE_beg_set_def_pdm_desc_cb:  
            return "UF_CLONE_beg_set_def_pdm_desc_cb";  
            case    UF_CLONE_end_set_def_pdm_desc_cb:  
            return "UF_CLONE_end_set_def_pdm_desc_cb";  
            case    UF_CLONE_beg_set_pdm_desc_cb:  
            return "UF_CLONE_beg_set_pdm_desc_cb";  
            case    UF_CLONE_end_set_pdm_desc_cb:  
            return "UF_CLONE_end_set_pdm_desc_cb";  
            case    UF_CLONE_beg_set_def_co_cb:  
            return "UF_CLONE_beg_set_def_co_cb";  
            case    UF_CLONE_end_set_def_co_cb:  
            return "UF_CLONE_end_set_def_co_cb";  
            case    UF_CLONE_beg_set_co_cb:  
            return "UF_CLONE_beg_set_co_cb";  
            case    UF_CLONE_end_set_co_cb:  
            return "UF_CLONE_end_set_co_cb";  
            case    UF_CLONE_beg_part_co:  
            return "UF_CLONE_beg_part_co";  
            case    UF_CLONE_end_part_co:  
            return "UF_CLONE_end_part_co";  
            case    UF_CLONE_beg_set_def_ci_cb:  
            return "UF_CLONE_beg_set_def_ci_cb";  
            case    UF_CLONE_end_set_def_ci_cb:  
            return "UF_CLONE_end_set_def_ci_cb";  
            case    UF_CLONE_beg_set_ci_cb:  
            return "UF_CLONE_beg_set_ci_cb";  
            case    UF_CLONE_end_set_ci_cb:  
            return "UF_CLONE_end_set_ci_cb";  
            case    UF_CLONE_beg_part_ci:  
            return "UF_CLONE_beg_part_ci";  
            case    UF_CLONE_end_part_ci:  
            return "UF_CLONE_end_part_ci";  
            case    UF_CLONE_beg_part_clone_cb:  
            return "UF_CLONE_beg_part_clone_cb";  
            case    UF_CLONE_end_part_clone_cb:  
            return "UF_CLONE_end_part_clone_cb";  
            case    UF_CLONE_beg_occ_report_cb:  
            return "UF_CLONE_beg_occ_report_cb";  
            case    UF_CLONE_beg_nonocc_report_cb:  
            return "UF_CLONE_beg_nonocc_report_cb";  
            case    UF_CLONE_report_cb:  
            return "UF_CLONE_report_cb";  
            case    UF_CLONE_end_report_cb:  
            return "UF_CLONE_end_report_cb";  
            default:  
                ECHO("unknown UF_CLONE_notify_cb_t = %d", what);  
                return "Unknown";  
        }  
    }  
    static UF_CLONE_notify_response_t GTAC_clone_cb(UF_CLONE_notify_cb_t cb,  
        const char * input_part_specifier, const void* data)  
    {  
        ECHO("in %s\n", get_clone_notify_enum_name(cb));  
        return UF_CLONE_continue;  
    }  
    static void do_it(void)  
    {  
        char  
            description[133],  
            *enum_name;  
        UF_CLONE_notify_cb_t  
            ii;  
        for (ii = UF_CLONE_initialise_cb; ii <= UF_CLONE_end_report_cb; ii++)  
        {  
            enum_name = get_clone_notify_enum_name(ii);  
            sprintf_s(description, sizeof(description), "GTAC Testing for %s",  
                enum_name);  
            ECHO("Registering %s callback - %s\n", enum_name, description);  
            UF_CALL(UF_CLONE_register_ntfy_callback(ii, GTAC_clone_cb,  
                get_clone_notify_enum_name(ii), description, NULL, TRUE));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是NX Open的一个示例，用于演示如何为NX克隆功能注册通知回调。
>
> 主要功能包括：
>
> 1. 定义了一个ECHO宏，用于输出日志信息到NX日志窗口。
> 2. 定义了一个report_error函数，用于在函数调用失败时输出错误信息。
> 3. 定义了一个get_clone_notify_enum_name函数，用于获取克隆通知枚举类型的名称。
> 4. 定义了一个GTAC_clone_cb函数，作为回调函数，当发生克隆事件时会调用这个函数。这个函数只是简单输出事件名称。
> 5. 定义了一个do_it函数，用于为每个克隆通知类型注册GTAC_clone_cb回调函数。
> 6. 定义了一个ufusr函数，这是NX Open程序的入口函数。在这个函数中初始化NX Open，调用do_it函数注册回调，然后终止NX Open。
> 7. ufusr函数是NX Open程序的入口点。
>
> 总的来说，这段代码的主要作用是演示如何在NX克隆过程中注册回调函数，并输出日志信息。这是一个典型的NX Open二次开发示例。
>
