### 【3032】update all scenarios 更新所有场景

#### 代码

```cpp
    /*HEAD UPDATE_ALL_SCENARIOS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    static void update_all_scenarios(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            old_part,  
            sf_part;  
        char  
            **scenario_names;  
        UF_PART_load_status_t  
            status;  
        UF_CALL(UF_SF_locate_scenarios(&n, &scenario_names));  
        for (ii = 0; ii < n; ii++)  
        {  
            if ((UF_PART_is_loaded(scenario_names[ii])) != 1)  
            {  
                UF_CALL(UF_PART_open_quiet(scenario_names[ii], &sf_part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
            }  
            UF_CALL(UF_ASSEM_set_work_part_quietly(sf_part, &old_part));  
            UF_CALL(UF_SF_update_scenario());  
            UF_CALL(UF_PART_save());  
            UF_CALL(UF_ASSEM_set_work_part_quietly(old_part, &sf_part));  
            UF_CALL(UF_PART_close(sf_part, 0, 1));  
        }  
        UF_free_string_array(n, scenario_names);  
    }  
    static void do_it(void)  
    {  
        update_all_scenarios();  
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

> 这段NX二次开发代码的主要功能是更新所有场景的装配。具体来说，代码的主要流程包括：
>
> 1. 初始化NX环境。
> 2. 查找所有场景，并对每个场景进行以下操作：如果场景尚未加载，则加载场景。设置工作部件为场景的装配部件。更新场景。保存场景。恢复工作部件为原始部件。关闭场景。
> 3. 如果场景尚未加载，则加载场景。
> 4. 设置工作部件为场景的装配部件。
> 5. 更新场景。
> 6. 保存场景。
> 7. 恢复工作部件为原始部件。
> 8. 关闭场景。
> 9. 释放场景名称数组。
> 10. 终止NX环境。
>
> 代码中还包括错误报告和加载状态报告的辅助函数。通过命令行参数，代码可以处理多个部件文件或数据库部件，并在NX图形窗口或控制台中输出处理结果。整体来看，这是一个典型的NX二次开发示例，实现了批量更新场景的功能。
>
