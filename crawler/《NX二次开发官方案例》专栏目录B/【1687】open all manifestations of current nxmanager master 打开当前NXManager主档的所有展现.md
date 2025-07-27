### 【1687】open all manifestations of current nxmanager master 打开当前NXManager主档的所有展现

#### 代码

```cpp
    /*HEAD OPEN_ALL_MANIFESTATIONS_OF_CURRENT_TCENG_MASTER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

请注意，UF_print_syslog是在V18版本中新增的，请只提供翻译，无需其他评论。 */  
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
    static void do_it(void)  
    {  
        logical  
            active;  
        int  
            ii,  
            jj,  
            file_count,  
            revision_count;  
         tag_t  
            part = UF_PART_ask_display_part();  
         UF_UGMGR_tag_t  
            db_part_tag,  
            *revisions;  
        char  
            encoded_name[MAX_FSPEC_SIZE + 1],  
            display_name[MAX_FSPEC_SIZE + 1],  
            **file_types,  
            **file_names,  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            revision_id[UF_UGMGR_NAME_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        UF_CALL(UF_is_ugmanager_active(&active));  
        if (!active)  
        {  
            WRITE("this is a not a UGMGR session\n");  
            return;  
        }  
        UF_PART_ask_part_name(part, encoded_name);  
        UF_CALL(UF_UGMGR_decode_part_filename(encoded_name, part_number,  
            part_revision, part_file_type, part_file_name));  
        UF_CALL(UF_UGMGR_ask_part_tag(part_number, &db_part_tag));  
        UF_CALL(UF_UGMGR_list_part_revisions(db_part_tag, &revision_count,  
            &revisions));  
        for (ii = 0; ii < revision_count; ii++)  
        {  
            UF_CALL(UF_UGMGR_ask_part_revision_id(revisions[ii], revision_id));  
            if (strcmp(revision_id, part_revision)) continue;  
            UF_CALL(UF_UGMGR_list_part_rev_files(revisions[ii], &file_count,  
                &file_types, &file_names));  
            for (jj = 0; jj < file_count; jj++)  
            {  
                if (strcmp(file_types[jj], "manifestation")) continue;  
                UF_CALL(UF_UGMGR_encode_part_filename(part_number,  
                    part_revision, file_types[jj], file_names[jj], encoded_name));  
                UF_CALL(UF_PART_name_for_display(encoded_name, display_name));  
                WRITE("Opening ");  
                WRITE(display_name);  
                UF_CALL(UF_PART_open_quiet(encoded_name, &part, &status));  
                report_load_status(&status);  
            }  
            UF_free_string_array(file_count, file_types);  
            UF_free_string_array(file_count, file_names);  
        }  
        UF_free(revisions);  
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

> 这段代码是NX Open API编写的二次开发代码，其主要功能是打开当前活动零件的所有版本在UGMGR中的表现。
>
> 主要步骤包括：
>
> 1. 首先判断是否为UGMGR会话，如果不是则直接返回。
> 2. 获取当前活动零件的信息，包括零件名、版本等。
> 3. 获取该零件在UGMGR中的所有版本信息。
> 4. 遍历每个版本，获取每个版本的所有文件信息。
> 5. 过滤出类型为"manifestation"的文件，即表现文件。
> 6. 对每个表现文件，先进行解码获取文件名，再进行编码获取NX中的文件名。
> 7. 打开每个表现文件，并报告加载状态。
> 8. 释放所有动态分配的内存。
> 9. 最后，在UFusr函数中初始化和终止NX Open API。
> 10. 提供一个卸载函数，用于立即卸载该代码。
>
> 通过这段代码，可以打开并查看当前活动零件在UGMGR中的所有版本的表现。
>
