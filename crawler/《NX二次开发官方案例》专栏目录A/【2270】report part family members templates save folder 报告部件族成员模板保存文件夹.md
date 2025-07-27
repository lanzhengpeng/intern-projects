### 【2270】report part family members templates save folder 报告部件族成员模板保存文件夹

#### 代码

```cpp
    /*HEAD REPORT_PART_FAMILY_MEMBERS_TEMPLATES_SAVE_FOLDER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
    #include <uf_fam.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的内容，UF_print_syslog是V18版本新增的功能。

因此，针对问题“UF_print_syslog is new in V18”，我的回答是：

UF_print_syslog是V18版本新增的功能。 */  
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
    static void load_family_template(tag_t family)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1],  
            template_part[MAX_FSPEC_SIZE+1];  
        UF_print_syslog("\n!!! Please ignore error messages between here - \n\n",  
            FALSE);  
    /*  This will fail and write an error to the syslog but will perform  
        a load on demand on the template part  
    */  
        UF_ASSEM_ask_component_data(family, template_part, refset_name,  
                instance_name, origin, csys_matrix, transform);  
        UF_print_syslog("\n!!! - and here!\n\n", FALSE);  
    }  
    static void do_it(void)  
    {  
        logical  
            nxman,  
            is_member;  
        int  
            index;  
        tag_t  
            family = NULL_TAG,  
            instance,  
            part = UF_PART_ask_display_part();  
        char  
            cli_name[MAX_FSPEC_SIZE+1],  
            *encoded_name,  
            *folder = NULL,  
            item_id[UF_UGMGR_PARTNO_SIZE+1],  
            part_rev[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            *template_item,  
            template_part[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_is_family_instance(part, &is_member));  
        if (!is_member)  
        {  
            WRITE("This is not a family member part.\n");  
            return;  
        }  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (!nxman)  
        {  
            UF_CALL(UF_PART_ask_family_instance(part, &instance));  
            UF_CALL(UF_FAM_ask_instance_data(instance, &family, &index));  
            UF_CALL(UF_PART_ask_template_filename(part, template_part));  
            load_family_template(family);  
            UF_CALL(UF_PART_ask_family_save_dir(family, &folder));  
            WRITE("Part family members are saved in ");  
            WRITE(folder);  
            WRITE("\n");  
            UF_free(folder);  
        }  
        else  
        {  
            UF_CALL(UF_PART_ask_template_filename(part, template_part));  
            UF_CALL(UF_PART_file_name_for_display(template_part, &template_item));  
            sprintf(cli_name, "@DB/%s", template_item);  
            UF_free(template_item);  
            UF_CALL(UF_UGMGR_convert_name_from_cli(cli_name, &encoded_name));  
            UF_CALL(UF_UGMGR_decode_part_file_name(encoded_name, item_id,  
                part_rev, part_file_type, part_file_name));  
            WRITE("See Family Members folder under ");  
            WRITE(item_id);  
            WRITE("\n");  
        }  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
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
                if (status.n_parts > 0) check_load_status(&status);  
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

> 这段代码是一个NX二次开发程序，其主要功能是查询并打印NX零件家族成员的保存路径。
>
> 关键功能包括：
>
> 1. 使用UF_PART_is_family_instance来判断当前打开的零件是否为零件家族成员。
> 2. 通过UF_FAM_ask_instance_data获取零件家族的相关数据。
> 3. 使用UF_PART_ask_template_filename获取零件家族模板零件的文件名。
> 4. 使用UF_PART_ask_family_save_dir查询零件家族成员的保存路径。
> 5. 若处于UG/Manager环境下，会获取零件在UG/Manager中的路径。
> 6. 在外部模式下，可以接受多个零件路径作为参数，逐个打开并执行上述操作。
> 7. 使用宏UF_CALL封装UF函数调用，并在出错时打印错误信息。
> 8. 使用UF_print_syslog输出提示信息，提示用户忽略某些错误信息。
> 9. 使用UF_UI系列函数在NX日志窗口输出信息。
>
> 总体来说，该程序通过查询NX零件家族信息，实现了获取零件家族成员保存路径的功能，并考虑了不同运行环境下的情况。
>
