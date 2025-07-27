### 【1847】register function to assign new part numbers in nxmanager 在NX Manager中注册一个用于分配新零件编号的函数

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
    #include <uf_exit.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, const char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    DllExport int MY_assign(UF_UGMGR_new_part_no_p_t assign_data)  
    {  
    /* Do NOT try to call UF_initialize here - see PR 4249768 里海译:不要在这里尝试调用UF_initialize - 参见PR 4249768。 */  
        ECHO("%s\n", "Calling MY_assign\nIncoming...");  
        WRITE_S(assign_data->old_item);  
        WRITE_S(assign_data->item_type);  
        WRITE_L(assign_data->part_number_modifiable);  
        WRITE_S(assign_data->new_id);  
        WRITE_L(assign_data->part_name_modifiable);  
        WRITE_S(assign_data->new_name);  
        WRITE_L(assign_data->part_description_modifiable);  
        WRITE_S(assign_data->new_description);  
        WRITE_D(assign_data->ifail);  
    /*  If you want to post a custom dialog, do it like this:  
        if (UF_UI_lock_ug_access(UF_UI_FROM_CUSTOM) == UF_UI_LOCK_SET)  
        {  
            int resp;  
            UF_CALL(UF_STYLER_create_dialog("test.dlg", NULL, 0, NULL, &resp));  
            UF_UI_unlock_ug_access(UF_UI_FROM_CUSTOM);  
        }  
    */  
        assign_data->part_number_modifiable = FALSE;  
        strcpy(assign_data->new_id, "GTAC_TEST");  
        assign_data->part_name_modifiable = TRUE;  
        strcpy(assign_data->new_name, "GTAC Test name");  
        assign_data->part_description_modifiable = TRUE;  
        strcpy(assign_data->new_description, "GTAC Test description");  
        assign_data->ifail = 0;  
        ECHO("%s\n", "Outgoing...");  
        WRITE_S(assign_data->old_item);  
        WRITE_S(assign_data->item_type);  
        WRITE_L(assign_data->part_number_modifiable);  
        WRITE_S(assign_data->new_id);  
        WRITE_L(assign_data->part_name_modifiable);  
        WRITE_S(assign_data->new_name);  
        WRITE_L(assign_data->part_description_modifiable);  
        WRITE_S(assign_data->new_description);  
        WRITE_D(assign_data->ifail);  
        UF_CALL(UF_terminate());  
        return 0;  
    }  
    static void do_it(void)  
    {  
        ECHO("%s\n", "Registering MY_assign");  
        UF_CALL(UF_UGMGR_reg_new_part_no(MY_assign));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_SEL_DIALOG);  
    }

```

#### 代码解析

> 这段代码是NX Open API的一个二次开发示例，主要用于处理NX中的新零件号分配事件。
>
> 主要功能包括：
>
> 1. 定义了ECHO宏，用于输出信息到列表窗口和系统日志。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并在出错时报告错误信息。
> 3. 定义了WRITE_D、WRITE_L、WRITE_S等宏，用于输出变量值到列表窗口。
> 4. 定义了write_string_to_listing_window函数，用于输出字符串变量值到列表窗口。
> 5. 实现了MY_assign函数，用于处理新零件号分配事件，包括输出参数信息，修改部分参数，并再次输出参数信息。
> 6. 定义了do_it函数，用于注册MY_assign函数。
> 7. 实现了ufsta和ufusr函数，用于在NX启动和用户请求时调用do_it函数。
> 8. 实现了ufusr_ask_unload函数，用于在NX卸载插件时显示确认对话框。
>
> 整体来看，这段代码通过注册回调函数的方式，实现了对新零件号分配事件的拦截和处理，并输出了相关信息，是一个典型的NX二次开发示例。
>
