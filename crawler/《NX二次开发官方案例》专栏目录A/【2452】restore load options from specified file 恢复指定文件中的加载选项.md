### 【2452】restore load options from specified file 恢复指定文件中的加载选项

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
    static void report_assem_search_directories(void)  
    {  
        char  
            **dir_list;  
        int  
            ii,  
            num_dir = 0;  
        logical  
            *subs;  
        UF_CALL(UF_ASSEM_ask_search_directories(&num_dir, &dir_list, &subs));  
        for (ii = 0; ii < num_dir; ii++)  
        {  
            ECHO("%s", dir_list[ii]);  
            if (subs[ii]) ECHO("...");  
            ECHO("\n");  
        }  
        UF_free_string_array(num_dir, dir_list);  
        UF_free(subs);  
    }  
    static void report_assem_options(void)  
    {  
        UF_ASSEM_options_t  
            assem_opts;  
        UF_CALL(UF_ASSEM_ask_assem_options(&assem_opts));  
        ECHO("Part Versions\n");  
        switch (assem_opts.load_options)  
        {  
            case UF_ASSEM_load_from_directory:  
                ECHO(" Load = From Folder\n");  
                break;  
            case UF_ASSEM_load_from_search_dirs:  
                ECHO(" Load = From Search Folders\n");  
                break;  
            case UF_ASSEM_load_as_saved:  
                ECHO(" Load = As Saved\n");  
                break;  
            default:  
                ECHO(" assem_opts.load_options = unknown value (%d)",  
                    assem_opts.load_options);  
                break;  
        }  
        switch (assem_opts.load_latest)  
        {  
            case UF_ASSEM_load_exact_version:  
                ECHO(" Load Latest = Off\n");  
                break;  
            case UF_ASSEM_load_latest_version:  
                ECHO(" Load Latest = On\n");  
                break;  
            default:  
                ECHO(" assem_opts.load_latest = unknown value (%d)",  
                    assem_opts.load_latest);  
                break;  
        }  
        if (assem_opts.load_options == UF_ASSEM_load_from_search_dirs)  
            report_assem_search_directories();  
        ECHO("Scope\n");  
        switch(assem_opts.load_components)  
        {  
            case UF_ASSEM_dont_load_components:  
                ECHO(" Load = Structure Only\n");  
                break;  
            case UF_ASSEM_do_load_components:  
                ECHO(" Load = All Components\n");  
                break;  
            case UF_ASSEM_use_last_component_set:  
                ECHO(" Load = As Saved\n");  
                break;  
            case UF_ASSEM_use_last_filter:  
                ECHO(" Load = Re-evaluate Last Component Group\n");  
                break;  
            case UF_ASSEM_specify_filter:  
                ECHO(" Load = Specify Component Group\n");  
                break;  
            default:  
                ECHO(" assem_opts.load_components = unknown value (%d)",  
                    assem_opts.load_components);  
                break;  
        }  
        switch(assem_opts.load_fully)  
        {  
            case UF_ASSEM_load_components_partially:  
                ECHO(" Use Partial Loading = On\n");  
                break;  
            case UF_ASSEM_load_components_fully:  
                ECHO(" Use Partial Loading = Off\n");  
                break;  
            default:  
                ECHO(" assem_opts.load_fully = unknown value (%d)",  
                    assem_opts.load_fully);  
                break;  
        }  
        switch (assem_opts.load_wave_data)  
        {  
            case UF_ASSEM_load_wave_data:  
                ECHO(" Load Interpart Data = On\n");  
                break;  
            case UF_ASSEM_default_load_behaviour:  
                ECHO(" Load Interpart Data = Off\n");  
                break;  
            default:  
                ECHO(" assem_opts.load_wave_data = unknown value (%d)",  
                    assem_opts.load_wave_data);  
                break;  
        }  
        switch (assem_opts.load_wave_parents)  
        {  
            case UF_ASSEM_none:  
                ECHO(" Load Parents = None\n");  
                break;  
            case UF_ASSEM_immediate:  
                ECHO(" Load Parents = Immediate Level Only\n");  
                break;  
            case UF_ASSEM_all:  
                ECHO(" Load Parents = All Levels\n");  
                break;  
            default:  
                ECHO(" assem_opts.load_wave_parents = unknown value (%d)",  
                    assem_opts.load_wave_parents);  
                break;  
        }  
        ECHO("Load Behavior\n");  
        switch (assem_opts.load_substitution)  
        {  
            case UF_ASSEM_do_allow_substitution:  
                ECHO(" Allow Replacement = On\n");  
                break;  
            case UF_ASSEM_dont_allow_substitution:  
                ECHO(" Allow Replacement = Off\n");  
                break;  
            default:  
                ECHO(" assem_opts.load_substitution = unknown value (%d)",  
                    assem_opts.load_substitution);  
                break;  
        }  
        switch (assem_opts.auto_regen_pfm_option)  
        {  
            case UF_ASSEM_auto_generate_pfm:  
                ECHO(" Generate Missing Part Family Members = On\n");  
                break;  
            case UF_ASSEM_dont_auto_generate_pfm:  
                ECHO(" Generate Missing Part Family Members = Off\n");  
                break;  
            default:  
                ECHO(" assem_opts.auto_regen_pfm_option = unknown value (%d)",  
                    assem_opts.auto_regen_pfm_option);  
                break;  
        }  
        switch (assem_opts.failure_action)  
        {  
            case UF_ASSEM_do_abort_on_failure:  
                ECHO(" Cancel Load on Failure = On\n");  
                break;  
            case UF_ASSEM_dont_abort_on_failure:  
                ECHO(" Cancel Load on Failure = Off\n");  
                break;  
            default:  
                ECHO(" assem_opts.failure_action = unknown value (%d)",  
                    assem_opts.failure_action);  
                break;  
        }  
        ECHO("Reference Sets\n");  
        switch (assem_opts.apply_to_all_levels)  
        {  
            case UF_ASSEM_do_apply_to_all_levels:  
                ECHO(" Apply to All Assembly Levels = On\n");  
                break;  
            case UF_ASSEM_dont_apply_to_all_levels:  
                ECHO(" Apply to All Assembly Levels = Off\n");  
                break;  
            default:  
                ECHO(" assem_opts.apply_to_all_levels = unknown value (%d)",  
                    assem_opts.apply_to_all_levels);  
                break;  
        }  
        ECHO("Preferences-> Assembly (misc)\n");  
        switch(assem_opts.emphasize)  
        {  
            case UF_ASSEM_do_not_emphasize_wp:  
                ECHO(" Work Part-> Emphasize = Off\n");  
                break;  
            case UF_ASSEM_emphasize_wp:  
                ECHO(" Work Part-> Emphasize = On\n");  
                break;  
            default:  
                ECHO(" assem_opts.emphasize = unknown value (%d)",  
                    assem_opts.emphasize);  
                break;  
        }  
        switch(assem_opts.maintain_work_part)  
        {  
            case UF_ASSEM_do_maintain_work_part:  
                ECHO(" Work Part-> Maintain = On\n");  
                break;  
            case UF_ASSEM_dont_maintain_work_part:  
                ECHO(" Work Part-> Maintain = Off\n");  
                break;  
            default:  
                ECHO("assem_opts.maintain_work_part = unknown value (%d)",  
                    assem_opts.maintain_work_part);  
                break;  
        }  
        switch(assem_opts.update)  
        {  
            case UF_ASSEM_no_update_report:  
                ECHO(" Display Update Report = Off\n");  
                break;  
            case UF_ASSEM_update_report:  
                ECHO(" Do Not Display Update Report = On\n");  
                break;  
            default:  
                ECHO(" assem_opts.update = unknown value (%d)",  
                    assem_opts.update);  
                break;  
        }  
    }  
    static void load_specified_load_options_def_file(void)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1] = { "*.def" },  
            fspec[MAX_FSPEC_SIZE+1] = { "" };  
        if (!UF_CALL(UF_UI_create_filebox("Enter load options file to restore",  
            "Restore Load Options File", filter, "", fspec, &resp)) &&  
            (resp == UF_UI_OK))  
            UF_CALL(UF_ASSEM_restore_load_options(fspec));  
    }  
    static void do_it(void)  
    {  
        ECHO("Current load options:\n");  
        report_assem_options();  
        load_specified_load_options_def_file();  
        ECHO("\nNewly restored load options:\n");  
        report_assem_options();  
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

> 这段代码是NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个ECHO函数，用于在列表窗口和系统日志中输出信息。
> 2. 定义了一个report_error函数，用于在调用UF函数失败时报告错误信息。
> 3. 定义了一个report_assem_search_directories函数，用于报告装配搜索目录。
> 4. 定义了一个report_assem_options函数，用于报告当前的装配选项。
> 5. 定义了一个load_specified_load_options_def_file函数，用于加载用户指定的装配选项def文件。
> 6. 定义了一个do_it函数，用于报告当前装配选项，加载用户指定的def文件，并报告新的装配选项。
> 7. 定义了ufusr函数，作为程序的入口点，初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志。
> 9. 代码实现了查询当前装配选项、加载指定def文件并应用新的装配选项的功能。
>
> 总体来说，这段代码通过UF函数实现了NX装配选项的查询、加载和报告，是一个典型的NX二次开发示例。
>
