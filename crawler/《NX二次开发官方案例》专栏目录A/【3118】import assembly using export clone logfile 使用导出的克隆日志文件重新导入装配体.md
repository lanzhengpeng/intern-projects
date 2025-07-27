### 【3118】import assembly using export clone logfile 使用导出的克隆日志文件重新导入装配体

#### 代码

```cpp
    /*  
        This application can be used to re-import an assembly into  
        Teamcenter using the same .clone log file which was created when it  
        was exported using the Default Export Action of Overwrite.  Before  
        using the program edit the .clone file and change AUTO_TRANSLATE to  
        USER_NAME on line 5.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
    #include <uf_clone.h>  
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
    static logical prompt_for_clone_logfile_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1];  
        strcpy(filter, "*.clone");  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Clone Log File",  
            filter, "", fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void report_naming_failures(UF_CLONE_naming_failures_p_t status)  
    {  
        int  
            ii;  
        if (status->n_failures > 0)  
        {  
            ECHO("Naming failures:\n");  
            for (ii = 0; ii < status->n_failures; ii++)  
                ECHO("  %s -> %s - %s\n", status->input_names[ii],  
                    status->output_names[ii], status->statuses[ii]);  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_failures, status->input_names);  
            UF_free_string_array(status->n_failures, status->output_names);  
        }  
    }  
    static void report_logfile_failure(UF_CLONE_log_file_failure_t *failure)  
    {  
        if (failure->line_number == -1) return;  
        ECHO("Error on Line %d:  input_part_name: %s invalid_token: %s\n",  
            failure->line_number, failure->input_part_name,  
            failure->invalid_token);  
        if (failure->input_part_name) UF_free(failure->input_part_name);  
        if (failure->invalid_token) UF_free(failure->invalid_token);  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        char  
            exp_log[MAX_FSPEC_SIZE+1] = { "" },  
            exp_dir[MAX_FSPEC_SIZE+1] = { "" },  
            log[MAX_FSPEC_SIZE+1] = { "" },  
            imp_log[MAX_FSPEC_SIZE+1] = { "" };  
        UF_ASSEM_options_t  
            assem_opts;  
        UF_CLONE_naming_failures_t  
            naming_failures;  
        UF_CLONE_log_file_failure_t  
            logfile_failure;  
        UF_PART_load_status_t  
            load_status;  
        if (!prompt_for_clone_logfile_name("Export Clone after manual edits",  
            exp_log)) return;  
        UF_CALL(uc4576(exp_log, 0, exp_dir, log));  
        UF_CALL(uc4577(log));  
        strcat(log, ".clone");  
        UF_CALL(uc4575(exp_dir, 0, log, imp_log));  
        UF_CALL(UF_ASSEM_ask_assem_options(&assem_opts));  
        if (assem_opts.load_options != UF_ASSEM_load_from_directory)  
        {  
            assem_opts.load_options = UF_ASSEM_load_from_directory;  
            UF_CALL(UF_ASSEM_set_assem_options(&assem_opts));  
        }  
        UF_CALL(UF_CLONE_initialise(UF_CLONE_import_operation));  
        UF_CALL(UF_CLONE_set_logfile(imp_log));  
        UF_CALL(UF_CLONE_init_naming_failures(&naming_failures));  
        UF_CALL(UF_CLONE_init_log_file_failure(&logfile_failure));  
        UF_CALL(UF_CLONE_load_logfile(exp_log, &naming_failures,  
            &logfile_failure, &load_status));  
        report_logfile_failure(&logfile_failure);  
        report_naming_failures(&naming_failures);  
        report_load_status(&load_status);  
        if (ask_yes_or_no("Validate", "Perform Clone?"))  
        {  
            UF_CALL(UF_CLONE_init_naming_failures(&naming_failures));  
            UF_CALL(UF_CLONE_perform_clone(&naming_failures));  
        }  
        UF_CALL(UF_CLONE_terminate());  
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

> 这段代码是一个用于将Teamcenter中的装配体重新导入NX的二次开发程序。
>
> 主要功能包括：
>
> 1. 提示用户选择一个导出的.clone日志文件。
> 2. 根据日志文件中的信息，生成重新导入用的.clone文件。
> 3. 初始化装配体导入选项，设置为从目录加载。
> 4. 初始化导入操作，设置重新导入用的.clone文件。
> 5. 读取导出的.clone文件，获取重命名失败和加载状态信息。
> 6. 如果用户确认，执行重新导入操作。
> 7. 终止导入操作。
> 8. 在NX中卸载该程序。
>
> 该程序通过读取导出时生成的.clone日志文件，使用相同的信息重新导入装配体到NX，实现了装配体的重新导入功能。
>
