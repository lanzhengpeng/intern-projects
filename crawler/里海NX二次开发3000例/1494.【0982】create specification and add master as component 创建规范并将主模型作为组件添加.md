### 【0982】create specification and add master as component 创建规范并将主模型作为组件添加

#### 代码

```cpp
    /*HEAD CREATE_SPEC_AND_ADD_MASTER_AS_COMPONENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您提供的文档，UF_print_syslog是V18版本中新增的函数。以下是该函数的中文翻译：

UF_print_syslog：在V18版本中新增，用于打印系统日志消息。

希望以上翻译对您有所帮助。 */  
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
    static void build_copy_partname(char *addon, char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, addon);  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
    }  
    static void report_load_status(UF_PART_load_status_t *status)  
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
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            tceng;  
        tag_t  
            master_part = UF_PART_ask_display_part(),  
            spec_part,  
            instance;  
        double  
            origin[3] = {0, 0, 0},  
            matrix[6] = {1, 0, 0, 0, 1, 0};  
        char  
            master_encoded_name[MAX_FSPEC_SIZE+1],  
            spec_encoded_name[MAX_FSPEC_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        UF_PART_ask_part_name(master_part, master_encoded_name);  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (tceng)  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(master_encoded_name, part_number,  
                part_revision, part_file_type, part_file_name));  
            strcpy(part_file_type, "specification");  
            strcpy(part_file_name, "165-001-01");  
            UF_CALL(UF_UGMGR_encode_part_filename(part_number, part_revision,  
                part_file_type, part_file_name, spec_encoded_name));  
        }  
        else  
            build_copy_partname("_dwg", spec_encoded_name);  
        UF_CALL(UF_PART_new(spec_encoded_name, 1, &spec_part));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(spec_part, master_encoded_name, NULL,  
            NULL, origin, matrix, 0, &instance, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
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

> 这段NX二次开发代码的主要功能是创建一个新零件，并将当前显示的零件作为组件添加到新零件中。具体步骤如下：
>
> 1. 包含了必要的NX API头文件。
> 2. 定义了一个错误报告函数report_error，用于打印出API调用失败的错误信息。
> 3. 定义了一个函数build_copy_partname，用于构建新零件的文件名，格式为"原零件名_dwg"。
> 4. 定义了一个函数report_load_status，用于报告组件加载的状态信息。
> 5. 定义了一个函数do_it，其中实现了创建新零件和添加组件的逻辑：获取当前显示的零件。如果UGManager处于活动状态，则使用UGManager API解码当前零件的文件名，并构建新零件的文件名。如果UGManager不活动，则调用build_copy_partname函数构建新零件的文件名。使用新文件名创建一个新零件。将当前显示的零件作为组件添加到新零件中。如果组件加载有错误，则调用report_load_status函数报告错误信息。
> 6. 获取当前显示的零件。
> 7. 如果UGManager处于活动状态，则使用UGManager API解码当前零件的文件名，并构建新零件的文件名。
> 8. 如果UGManager不活动，则调用build_copy_partname函数构建新零件的文件名。
> 9. 使用新文件名创建一个新零件。
> 10. 将当前显示的零件作为组件添加到新零件中。
> 11. 如果组件加载有错误，则调用report_load_status函数报告错误信息。
> 12. 定义了ufusr函数，作为NX的启动函数，在其中调用do_it函数执行主要逻辑。
> 13. 定义了ufusr_ask_unload函数，用于卸载用户自定义函数，返回立即卸载。
>
> 总体来说，这段代码通过NX的UF_PART和UF_ASSEM等API实现了创建新零件并添加组件的功能。
>
