### 【0494】clone export into ugii tmp dir 将克隆导出到UGII临时目录中

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_clone.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, const char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        logical  
            tceng;  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (tceng)  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(fspec, temp_name, dspec,  
                        dspec, dspec));  
        }  
        else  
        {  
            UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
            if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        }  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void build_unique_temp_part_name(char *fspec)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(uc4577(unique));  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4575(tmp_dir, 2, unique, fspec));  
    }  
    /* 里海 */  
    static UF_CLONE_convert_response_t gtac_user_name_func(  
        UF_CLONE_convert_cb_t cb, const char *input_part_specifier,  
        char **output_part_specifier)  
    {  
        WRITE_S(input_part_specifier);  
        UF_CALL(allocate_memory(UF_CFI_MAX_PATH_NAME_SIZE * sizeof(char),  
            (void **)output_part_specifier));  
        build_unique_temp_part_name(*output_part_specifier);  
        WRITE_S(*output_part_specifier);  
        return UF_CLONE_use_supplied;  
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
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
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
            ECHO("Naming failures:\n");  
            for (ii = 0; ii < status->n_failures; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s -> %s - %s\n", status->input_names[ii],  
                    status->output_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_failures, status->input_names);  
            UF_free_string_array(status->n_failures, status->output_names);  
        }  
    }  
    static void report_clone_family_treatment(void)  
    {  
        UF_CLONE_family_treatment_t  
            family_treatment;  
        UF_CALL(UF_CLONE_ask_family_treatment(&family_treatment));  
        switch (family_treatment)  
        {  
            case UF_CLONE_treat_as_lost:  
                ECHO("family_treatment = UF_CLONE_treat_as_lost\n");  
                break;  
            case UF_CLONE_strip_family_status:  
                ECHO("family_treatment = UF_CLONE_strip_family_status\n");  
                break;  
            case UF_CLONE_give_error:  
                ECHO("family_treatment = UF_CLONE_give_error\n");  
                break;  
            default:  
                ECHO("family_treatment = %d (Unrecognized!)\n", family_treatment);  
                break;  
        }  
    }  
    static void report_environment_variable(char *variable_name)  
    {  
        char  
            *variable_value = NULL;  
        if (!UF_CALL(UF_translate_variable(variable_name, &variable_value)))  
            ECHO("Environment Variable %s = %s\n", variable_name, variable_value);  
    }  
    static void report_customer_default(char *default_name)  
    {  
        char  
            *default_value = NULL;  
        if (!UF_CALL(UF_get_customer_default(default_name, 0, &default_value)))  
        {  
            ECHO("Customer Default %s = %s\n", default_name, default_value);  
            UF_free(default_value);  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            in_nxman;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            *cli_name,  
            clone_log[MAX_FSPEC_SIZE+1],  
            *def_dir,  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            load_status;  
        UF_CLONE_naming_failures_t  
            name_fails;  
        UF_CLONE_convert_callback_t  
            callback_removed;  
        UF_CALL(UF_CLONE_register_cvt_callback(UF_CLONE_user_name_convert,  
            gtac_user_name_func, "GTAC User Naming", "Generate unique names",  
            NULL, FALSE));  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        build_similar_temp_filespec(partname, 47, clone_log);  
        UF_CALL(UF_is_ugmanager_active(&in_nxman));  
        if (in_nxman)  
        {  
            UF_CALL(UF_UGMGR_convert_file_name_to_cli(partname, &cli_name));  
            strcpy(partname, cli_name);  
            UF_free(cli_name);  
            UF_CALL(UF_CLONE_initialise(UF_CLONE_export_operation));  
            UF_CALL(UF_CLONE_set_def_action(UF_CLONE_overwrite));  
            UF_CALL(UF_CLONE_set_def_assoc_file_copy(FALSE));  
        }  
        else  
        {  
            UF_CALL(UF_CLONE_initialise(UF_CLONE_clone_operation));  
            UF_CALL(UF_CLONE_set_def_action(UF_CLONE_clone));  
        }  
    /*  In order to export part family members this environment variable  
        must be set.  
    */  
        report_environment_variable("UGII_UGMGR_ALLOW_PFM_IMPORT_EXPORT"); // 1  
    /*  OR this customer default must be OFF (No) 译:这个客户默认必须关闭(否)。 */  
        report_customer_default("UGMGR_CloneImportExportAutoRemovePFM"); // No  
    //  Neither of the the above can be changed on the fly by a program.  
        report_customer_default("UGMGR_ImportExportAutoAddPFTemplates");  
        UF_CALL(UF_CLONE_set_family_treatment(UF_CLONE_strip_family_status));  
        report_clone_family_treatment();  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &def_dir));  
        UF_CALL(UF_CLONE_set_logfile(clone_log));  
        WRITE_S(clone_log);  
        uc4561(clone_log, 0);  
        UF_CALL(UF_CLONE_set_def_naming(UF_CLONE_user_name));  
        UF_CALL(UF_CLONE_add_assembly(partname, &load_status));  
        report_load_status(&load_status);  
        UF_CALL(UF_CLONE_init_naming_failures(&name_fails));  
        UF_CALL(UF_CLONE_perform_clone(&name_fails));  
        report_naming_failures(&name_fails);  
        UF_CALL(UF_CLONE_terminate());  
        UF_CALL(UF_CLONE_remove_cvt_callback(UF_CLONE_user_name_convert,  
            "GTAC User Naming", &callback_removed));  
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
    /*  To run in external native mode NX:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external NX Manager mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，用于执行零件的克隆操作。其主要功能包括：
>
> 1. 定义了一些辅助函数，如报告错误、输出信息到列表窗口等。
> 2. 注册了一个转换回调函数，用于生成唯一的临时零件名称。
> 3. 检查UG Manager是否激活，并初始化克隆操作。
> 4. 查询环境变量和客户默认设置，以确定如何处理零件族。
> 5. 设置日志文件，添加要克隆的零件，并执行克隆操作。
> 6. 报告克隆过程中的命名失败。
> 7. 释放资源并注销转换回调函数。
> 8. 提供了一个ufusr()函数，用于在NX内部调用该应用程序。
> 9. 提供了一个主函数，用于在外部调用该应用程序，并处理命令行参数指定的零件。
>
> 总体来说，这段代码通过NX Open C++ API实现了零件的克隆功能，并考虑了UG Manager模式和外部调用的情况。
>
