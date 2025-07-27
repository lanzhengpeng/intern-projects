### 【3158】run another program from a uistyler action button 从一个Uistyler动作按钮运行另一个程序

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
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
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_api_executable_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            *ufun_dir,  
            *ufun_ext,  
            filter[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_UFUN_DIR, &ufun_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_FILE_UFUN_FLTR, &ufun_ext));  
        combine_directory_and_wildcard(ufun_dir, ufun_ext, filter);  
        UF_free(ufun_dir);  
        UF_free(ufun_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "API Executable", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void run_it(void)  
    {  
        char  
            ufun_prog[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_load_f_p_t  
            go;  
        if (prompt_for_api_executable_file_name("Time Run of api program",  
            ufun_prog) &&  
            !UF_CALL(UF_load_library(ufun_prog, "ufusr", (UF_load_f_p_t *)&go)))  
        {  
            go();  
            UF_CALL(UF_unload_library(ufun_prog));  
        }  
    }  
    #define CHANGE_ACTION_0                ("ACTION_0")  
    static int CHANGE_action_0_act_cb ( int dialog_id,  
        void * client_data, UF_STYLER_item_value_type_p_t callback_data)  
    {  
        run_it();  
        return ( UF_UI_CB_CONTINUE_DIALOG );  
    }  
    static void do_it(void)  
    {  
        int resp;  
        UF_STYLER_callback_info_t CHANGE_cbs[2] = {  
            {CHANGE_ACTION_0, UF_STYLER_ACTIVATE_CB, 1, CHANGE_action_0_act_cb} ,  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL(UF_STYLER_create_dialog("run_program", CHANGE_cbs, 2, NULL,  
            &resp));  
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
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是NX Open C++的一个二次开发示例程序，主要实现了以下功能：
>
> 1. 错误处理：定义了ECHO和report_error函数，用于打印错误信息。
> 2. 文件路径拼接：combine_directory_and_wildcard函数用于拼接目录路径和通配符。
> 3. 对话框提示：prompt_for_api_executable_file_name函数用于弹出一个对话框，提示用户选择一个UFUN程序文件。
> 4. 加载并执行UFUN程序：run_it函数负责加载用户选择的UFUN程序文件，并调用其main函数。
> 5. 创建并显示对话框：CHANGE_action_0_act_cb函数用于创建一个对话框，当用户点击按钮时，会调用run_it函数。
> 6. UFUN程序入口：ufusr函数是UFUN程序的入口，首先调用UF_initialize进行初始化，然后调用do_it显示对话框。
> 7. 卸载处理：ufusr_ask_unload函数用于处理UFUN程序的卸载。
>
> 整体来看，该程序实现了在NX中加载并运行一个用户指定的UFUN程序的功能，并提供了相应的错误处理和用户交互。
>
