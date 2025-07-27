### 【1996】report assem options report_assem_options报告装配选项

#### 代码

```cpp
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
        for (ii = 0 ; ii < num_dir; ii ++)  
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

```

#### 代码解析

> 这段代码是一个NX Open C++二次开发示例，主要实现了以下几个功能：
>
> 1. 查询并打印装配搜索目录：通过UF_ASSEM_ask_search_directories函数获取装配搜索目录，并打印目录列表。
> 2. 查询并打印装配加载选项：通过UF_ASSEM_ask_assem_options函数获取装配加载选项，并打印各个选项的值，包括装配加载方式、加载最新版本、加载范围、是否允许替换等。
> 3. 处理装配加载选项：针对装配加载选项的不同值，执行不同的打印操作。例如，如果装配加载方式为从搜索目录加载，则调用函数打印搜索目录列表。
> 4. 打印装配更新报告选项：获取装配更新报告选项的值，并打印是否显示更新报告。
> 5. 打印装配工作零件选项：获取装配工作零件选项的值，并打印是否强调工作零件、是否保持工作零件等。
> 6. 打印应用到所有层级选项：获取应用到所有层级选项的值，并打印是否应用到所有层级。
> 7. 释放内存：在打印搜索目录列表后，释放动态分配的内存。
>
> 总体来说，这段代码主要用于查询并打印NX装配加载选项的当前设置，以了解当前的装配加载配置。
>
