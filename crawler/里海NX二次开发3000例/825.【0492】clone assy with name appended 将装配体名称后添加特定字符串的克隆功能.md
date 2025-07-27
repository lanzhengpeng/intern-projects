### 【0492】clone assy with name appended 将装配体名称后添加特定字符串的克隆功能

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_clone.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
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
    static void report_naming_failures(UF_CLONE_naming_failures_p_t status)  
    {  
        int  
            ii;  
        ECHO("%d Naming failures\n", status->n_failures);  
        if (status->n_failures > 0)  
        {  
            for (ii = 0; ii < status->n_failures; ii++)  
                ECHO(" %d.  %s -> %s - %s\n", ii+1, status->input_names[ii],  
                    status->output_names[ii], status->statuses[ii]);  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_failures, status->input_names);  
            UF_free_string_array(status->n_failures, status->output_names);  
        }  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void do_it(void)  
    {  
        logical  
            nxman;  
        char  
            clone_log[MAX_FSPEC_SIZE+1],  
            part_dir[MAX_FSPEC_SIZE+1],  
            part_spec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_CLONE_name_rule_def_t  
            name_rule;  
        UF_CLONE_naming_failures_t  
            naming_failures;  
        UF_PART_load_status_t  
            load_status;  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        UF_CALL(UF_CLONE_terminate());  
        UF_CALL(UF_CLONE_initialise(UF_CLONE_clone_operation));  
        UF_CALL(UF_PART_ask_part_name(part, part_spec));  
        if (!nxman)  
        {  
            UF_CALL(uc4576(part_spec, 2, part_dir, part_name));  
            UF_CALL(UF_CLONE_set_def_directory(part_dir));  
        }  
        UF_CALL(UF_CLONE_set_def_action(UF_CLONE_clone));  
        UF_CALL(UF_CLONE_set_family_treatment(UF_CLONE_treat_as_lost));  
        name_rule.type = UF_CLONE_append_string;  
        name_rule.base_string = "notused";  
        name_rule.new_string  = "_cloned";  
        UF_CLONE_init_naming_failures(&naming_failures);  
        UF_CALL(UF_CLONE_set_name_rule(&name_rule, &naming_failures));  
        report_naming_failures(&naming_failures);  
        UF_CALL(UF_CLONE_set_def_naming(UF_CLONE_naming_rule));  
        if (!UF_CALL(UF_CLONE_add_assembly(part_spec, &load_status)))  
        {  
            build_similar_temp_filespec(part_spec, 47, clone_log);  
            WRITE_S(clone_log);  
            UF_CALL(UF_CLONE_set_logfile(clone_log));  
            UF_CLONE_init_naming_failures(&naming_failures);  
            UF_CALL(UF_CLONE_perform_clone(&naming_failures));  
            report_naming_failures(&naming_failures);  
        }  
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
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
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
        UF_ASSEM_options_t  
            options;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            UF_CALL(UF_ASSEM_ask_assem_options(&options));  
            options.load_components = UF_ASSEM_dont_load_components;  
            UF_CALL(UF_ASSEM_set_assem_options(&options));  
            while (uc4621(part_name) == 1)  
            {  
                ECHO("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            ECHO("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是对NX部件进行克隆操作，具体步骤如下：
>
> 1. 初始化NX环境。
> 2. 获取当前显示的部件，并查询其文件名。
> 3. 设置默认的克隆操作为复制。
> 4. 设置默认的目录为当前部件的所在目录。
> 5. 设置名称规则，在原名称后加上"_cloned"。
> 6. 将当前部件添加到克隆操作中。
> 7. 检查名称冲突，并报告冲突信息。
> 8. 设置日志文件。
> 9. 执行克隆操作，并报告名称冲突信息。
> 10. 完成克隆后，关闭所有部件。
> 11. 重复以上步骤，对命令行传入的多个部件文件进行克隆。
> 12. 释放NX环境。
>
> 代码中使用了NX提供的UF接口，通过调用这些接口实现打开、查询、设置、克隆等操作。同时，代码中包含错误处理和日志输出功能。这段代码是一个典型的NX二次开发应用示例。
>
