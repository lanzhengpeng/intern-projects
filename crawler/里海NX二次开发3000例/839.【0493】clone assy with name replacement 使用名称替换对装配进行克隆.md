### 【0493】clone assy with name replacement 使用名称替换对装配进行克隆

#### 代码

```cpp
    /*HEAD CLONE_ASSY_WITH_NAME_REPLACEMENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_clone.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中新增的函数，它用于打印系统日志。在V18之前的版本中，并没有提供这个函数。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_naming_failures(UF_CLONE_naming_failures_p_t failures)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (failures->n_failures > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Naming failures:\n");  
            for (ii = 0; ii < failures->n_failures; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_get_fail_message(failures->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(failures->input_names[ii]);  
                UF_UI_write_listing_window(" -> ");  
                UF_UI_write_listing_window(failures->output_names[ii]);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(failures->statuses);  
            UF_free_string_array(failures->n_failures, failures->input_names);  
            UF_free_string_array(failures->n_failures, failures->output_names);  
        }  
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
        name_rule.type = UF_CLONE_replace_string;  
        name_rule.base_string = "T10MD";  
        name_rule.new_string  = "G18MD";  
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
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                report_load_status(&status);  
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

> 这段代码是基于NX的二次开发代码，主要实现以下功能：
>
> 1. 初始化和错误处理：使用UF_initialize进行初始化，并定义了一个report_error函数用于报告错误。
> 2. 构建临时文件名：定义了一个build_similar_temp_filespec函数，用于构建克隆操作的临时文件名。
> 3. 写入字符串到列表窗口：定义了一个write_string_to_listing_window函数，用于将字符串写入列表窗口。
> 4. 报告命名失败：定义了一个report_naming_failures函数，用于报告克隆命名失败的输入和输出名称。
> 5. 克隆操作实现：定义了一个do_it函数，其中包含以下主要步骤：获取当前显示的零件，并设置克隆操作的默认目录设置克隆操作的默认动作、族处理方式、名称规则添加要克隆的装配体执行克隆操作报告命名失败
> 6. 获取当前显示的零件，并设置克隆操作的默认目录
> 7. 设置克隆操作的默认动作、族处理方式、名称规则
> 8. 添加要克隆的装配体
> 9. 执行克隆操作
> 10. 报告命名失败
> 11. UFusr函数：实现了ufusr函数，在NX内部调用时执行do_it函数。
> 12. 报告加载状态：定义了一个report_load_status函数，用于报告零件加载状态。
> 13. main函数：程序入口，用于处理命令行参数，打开零件并执行克隆操作。
> 14. 注释：提供了如何在外部模式和UG/Manager模式下运行程序的说明。
>
> 总体而言，这段代码通过NX提供的API实现了零件的克隆功能，支持从命令行参数加载零件并执行克隆操作。
>
