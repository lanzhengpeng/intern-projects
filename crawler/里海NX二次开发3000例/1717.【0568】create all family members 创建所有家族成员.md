### 【0568】create all family members 创建所有家族成员

#### 代码

```cpp
    /*HEAD CREATE_ALL_FAMILY_MEMBERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_fam.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文意，UF_print_syslog是V18版本中的新增功能。 */  
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
    #define WRITE(X)      (UF_UI_write_listing_window(X))  
    static void do_it(void)  
    {  
        logical  
            is_template;  
        int  
            ii,  
            n_fams;  
        tag_t  
            *fams,  
            instance,  
            mem_part,  
            prev,  
            part = UF_PART_ask_display_part();  
        char  
            *part_name,  
            part_spec[MAX_FSPEC_SIZE+1];  
        UF_FAM_family_data_t  
            family_data;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_PART_is_family_template(part, &is_template));  
        if (is_template)  
        {  
            UF_CALL(UF_PART_ask_families(part, &n_fams, &fams));  
            if (n_fams > 0)  
            {  
                UF_CALL(UF_FAM_ask_family_data( fams[0], &family_data));  
                for (ii = 0; ii < family_data.member_count; ii++)  
                {  
                    UF_PART_find_family_instance(fams[0], ii, FALSE, TRUE,  
                        &part_name);  
                    if (!part_name)  
                    {  
                        UF_CALL(UF_PART_create_family_instance(fams[0], ii,  
                            &mem_part, &instance));  
                        UF_PART_ask_part_name(mem_part, part_spec);  
                        WRITE("creating member - ");  
                        WRITE(part_spec);  
                        WRITE("\n");  
                        UF_CALL(UF_ASSEM_set_work_part_quietly(mem_part, &prev));  
                        UF_CALL(UF_PART_save());  
                        UF_CALL(UF_PART_close(mem_part, 0, 1));  
                        UF_CALL(UF_ASSEM_set_work_part_quietly(prev, &mem_part));  
                    }  
                    else  
                    {  
                        WRITE("member exists - ");  
                        WRITE(part_name);  
                        WRITE("\n");  
                        UF_free(part_name);  
                    }  
                }  
            }  
            else  
                WRITE("  No families found!\n");  
        }  
        else  
            WRITE("  Not a part family template!\n");  
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
        To run in external NX/Manager mode:  
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

> 这段NX二次开发代码的主要功能是创建一个零件家族模板的所有实例成员。
>
> 主要流程如下：
>
> 1. 判断当前零件是否为零件家族模板。如果是，则获取该模板的所有家族。
> 2. 遍历每个家族的成员，并检查每个成员是否已存在。如果成员不存在，则创建该成员实例，并保存。
> 3. 如果当前零件不是零件家族模板，则输出提示信息。
>
> 代码中使用了UF_PART、UF_FAM、UF_ASSEM等NX API函数，用于查询家族信息、创建成员实例、设置工作零件等。另外，通过UF_UI接口在NX日志窗口中输出信息。
>
> 此外，代码还支持从命令行参数传入零件路径，批量处理多个零件文件。处理每个零件文件时，会检查其加载状态，只有成功加载的零件才会进行后续处理。
>
> 总的来说，这段代码实现了根据零件家族模板批量创建所有成员实例的功能，适用于零件管理和批量处理。
>
