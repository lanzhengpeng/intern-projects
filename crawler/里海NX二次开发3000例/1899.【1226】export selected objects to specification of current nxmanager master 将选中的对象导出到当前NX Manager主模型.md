### 【1226】export selected objects to specification of current nxmanager master 将选中的对象导出到当前NX Manager主模型

#### 代码

```cpp
    /*HEAD EXPORT_SELECTED_OBJECTS_TO_SPECIFICATION_OF_CURRENT_TCENG_MASTER CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，UF_print_syslog 是 V18 版本中的新功能。 */  
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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    static logical find_specification_of_master(tag_t master, char *encoded_spec)  
    {  
        logical  
            active,  
            found = FALSE;  
        int  
            ii,  
            jj,  
            file_count,  
            revision_count;  
         tag_t  
            a_part;  
         UF_UGMGR_tag_t  
            db_part_tag,  
            *revisions;  
        char  
            a_part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            a_part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            a_part_number[UF_UGMGR_PARTNO_SIZE+1],  
            a_part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            encoded_name[MAX_FSPEC_SIZE + 1],  
            **file_types,  
            **file_names,  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            revision_id[UF_UGMGR_NAME_SIZE+1];  
        UF_CALL(UF_is_ugmanager_active(&active));  
        if (!active) return FALSE;  
        UF_PART_ask_part_name(master, encoded_name);  
        UF_CALL(UF_UGMGR_decode_part_filename(encoded_name, part_number,  
            part_revision, part_file_type, part_file_name));  
    /*  First check for specification already open in session 里海译:首先检查会话中是否已打开规范。 */  
        file_count = UF_PART_ask_num_parts();  
        for (ii = 0; ii < file_count; ii++)  
        {  
            a_part = UF_PART_ask_nth_part(ii);  
            UF_PART_ask_part_name(a_part, encoded_spec);  
            UF_CALL(UF_UGMGR_decode_part_filename(encoded_spec, a_part_number,  
                a_part_revision, a_part_file_type, a_part_file_name));  
            if (!strcmp(part_number, a_part_number) &&  
                !strcmp(part_revision, a_part_revision) &&  
                !strcmp(a_part_file_type, "specification"))  
                    return TRUE;  
        }  
    /*  Second check database for specification 里海译:检查数据库中的规范（第二次检查） */  
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
                if (strcmp(file_types[jj], "specification")) continue;  
                UF_CALL(UF_UGMGR_encode_part_filename(part_number,  
                    part_revision, file_types[jj], file_names[jj], encoded_spec));  
                found = TRUE;  
                break;  
            }  
            UF_free_string_array(file_count, file_types);  
            UF_free_string_array(file_count, file_names);  
            break;  
        }  
        UF_free(revisions);  
        return found;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *objects,  
            part = UF_PART_ask_display_part();  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_export_options_t  
            options = { FALSE, UF_PART_maintain_params, UF_PART_copy_exp_deeply };  
        if (!find_specification_of_master(part, part_name)) return;  
        if ((n = select_objects("export", &objects)) > 0)  
        {  
            UF_CALL(UF_PART_export_with_options( part_name, n, objects, &options));  
            UF_free(objects);  
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

```

#### 代码解析

> 这段代码是一个NX的二次开发代码，其主要功能是将当前活动零件中的选定对象导出到与当前活动TC工程师主模型相关的规范文件中。
>
> 代码的关键点包括：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息到系统日志和列表窗口。
> 2. 定义了一个选择对象函数select_objects，用于弹出一个选择对象对话框，让用户选择要导出的对象。
> 3. 定义了一个查找规范函数find_specification_of_master，用于根据主模型名称找到对应的规范文件名。
> 4. 定义了一个执行导出函数do_it，首先调用find_specification_of_master找到规范文件名，然后调用select_objects让用户选择对象，最后调用UF_PART_export_with_options将对象导出到规范文件。
> 5. 定义了ufusr函数，作为NX二次开发的入口函数，初始化后调用do_it执行导出操作，完成后终止。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发模块可以立即被NX卸载。
>
> 总的来说，这段代码通过NX的UF函数实现了从当前活动零件导出选定对象到对应规范文件的功能。
>
