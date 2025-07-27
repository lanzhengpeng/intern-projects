### 【0511】clone import selected native assembly to nxmanager 导入选定的本地组件到NXManager中

#### 代码

```cpp
    /*HEAD CLONE_IMPORT_SELECTED_NATIVE_ASSEMBLY_TO_NXMANAGER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_clone.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是在V18版本中新增的。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
    static logical prompt_for_part_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir,  
            *p_ext;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_PART_OPEN_FLTR, &p_ext));  
        combine_directory_and_wildcard(p_dir, p_ext, filter);  
        UF_free(p_dir);  
        UF_free(p_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Part Name", filter, "", fspec,  
            &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static logical file_exists(char *file_spec)  
    {  
        int  
            in_session,  
            missing;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &missing));  
        in_session = UF_PART_is_loaded(file_spec);  
        if (missing && (in_session != 1) && (in_session != 2))  
            return FALSE;  
        else  
            return TRUE;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void report_naming_failures(UF_CLONE_naming_failures_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_failures > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Naming failures:\n");  
            for (ii = 0; ii < status->n_failures; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->input_names[ii]);  
                UF_UI_write_listing_window(" -> ");  
                UF_UI_write_listing_window(status->output_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_failures, status->input_names);  
            UF_free_string_array(status->n_failures, status->output_names);  
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
    static void write_file_to_listing_window(char *fspec)  
    {  
        char  
            a_line[MAX_LINE_SIZE+2];  
        FILE  
            *the_file;  
        WRITE(fspec);  
        WRITE(":\n\n");  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL)  
                UF_UI_write_listing_window(a_line);  
            fclose(the_file);  
        }  
    }  
    static void do_it(void)  
    {  
        char  
            clone_logfile_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            leafname[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[UF_CFI_MAX_FILE_NAME_SIZE],  
            top_assembly[UF_CFI_MAX_PATH_NAME_SIZE],  
            top_dir_path[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_load_status_t  
            load_status;  
        UF_CLONE_naming_failures_t  
            naming_failures;  
        if (!prompt_for_part_name("Import assembly", top_assembly) ||  
            !file_exists(top_assembly)) return;  
        UF_CALL(uc4576(top_assembly, 2, top_dir_path, partname));  
        UF_CALL(uc4574(top_assembly, 2, leafname));  
        strcat(leafname, "_clone");  
        UF_CALL(uc4575(top_dir_path, 47, leafname, clone_logfile_name));  
        UF_CALL(UF_CLONE_initialise(UF_CLONE_import_operation));  
        UF_CALL(UF_CLONE_set_family_treatment(UF_CLONE_give_error));  
        UF_CALL(UF_CLONE_set_def_action(UF_CLONE_use_existing));  
        UF_CALL(UF_CLONE_set_def_naming(UF_CLONE_autogen));  
        UF_CALL(UF_CLONE_init_naming_failures(&naming_failures));  
        UF_CALL(UF_CLONE_set_dryrun(FALSE));  
        UF_CALL(UF_CLONE_add_assembly(top_assembly, &load_status));  
        report_load_status(&load_status);  
        if (!load_status.failed)  
        {  
            UF_CALL(UF_CLONE_set_logfile(clone_logfile_name));  
            UF_CALL(UF_CLONE_set_dryrun(TRUE));  
            UF_CALL(UF_CLONE_perform_clone(&naming_failures));  
            report_naming_failures(&naming_failures);  
            write_file_to_listing_window(clone_logfile_name);  
            if (ask_yes_or_no(top_assembly, "Perform clone operation?"))  
            {  
                UF_CALL(UF_CLONE_set_dryrun(FALSE));  
                UF_CALL(UF_CLONE_perform_clone(&naming_failures));  
                report_naming_failures(&naming_failures);  
            }  
        }  
        UF_CALL(UF_CLONE_terminate());  
    }  
    /* 里海 */  
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

> 这段NX二次开发代码的功能是导入一个NX部件文件，并创建其克隆。主要功能包括：
>
> 1. 提示用户选择要导入的NX部件文件，并检查文件是否存在。
> 2. 获取部件文件所在的目录和文件名，并构造日志文件名。
> 3. 初始化克隆操作，设置家族处理为错误，默认行为为使用现有，默认命名方式为自动生成。
> 4. 添加要导入的部件文件，获取加载状态并报告。
> 5. 如果加载成功，设置日志文件，执行预演克隆并报告命名失败情况。
> 6. 提示用户确认是否执行克隆操作，若确认则执行克隆并报告命名失败情况。
> 7. 释放资源。
> 8. ufusr函数是NX调用的主函数，完成初始化、执行主功能、终止等操作。
> 9. ufusr_ask_unload函数返回立即卸载标志。
>
> 总体来说，这段代码实现了导入NX部件并创建其克隆的功能，提供了详细的日志记录和用户交互提示。
>
