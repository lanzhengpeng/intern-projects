### 【1705】open latest working revision of specified nxmanager master part 打开指定NXManager主部件的最新工作版本

#### 代码

```cpp
    /*HEAD OPEN_LATEST_WORKING_REVISION_OF_SPECIFIED_NXMANAGER_MASTER_PART CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意：UF_print_syslog是V18版本新增的函数，只回答翻译，不要添加额外内容。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
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
    static logical open_part(char *fspec, tag_t *part)  
    {  
        UF_PART_load_status_t  
            status;  
        UF_CALL(UF_PART_open(fspec, part, &status));  
        if (status.n_parts > 0) report_load_status(&status);  
        return (status.failed);  
    }  
    static logical ask_configured_rev(char *part_number, char *revision_id)  
    {  
        UF_UGMGR_tag_t  
            database_part_tag,  
            part_revision;  
        strcpy(revision_id, "");  
        if (UF_CALL(UF_UGMGR_ask_part_tag(part_number, &database_part_tag)) ||  
            UF_CALL(UF_UGMGR_ask_configured_rev(database_part_tag,  
                &part_revision)) ||  
            UF_CALL(UF_UGMGR_ask_part_revision_id(part_revision, revision_id)))  
            return FALSE;  
        return TRUE;  
    }  
    static void do_it(void)  
    {  
        logical  
            is_active;  
        tag_t  
            part;  
        char  
            current_rule[UF_UGMGR_NAME_SIZE + 1],  
            encoded_name[MAX_FSPEC_SIZE+1],  
            *opened,  
            part_number[UF_UGMGR_NAME_SIZE + 1] = { "" },  
            revision_id[UF_UGMGR_NAME_SIZE + 1];  
        if (UF_CALL(UF_is_ugmanager_active(&is_active)) || !is_active) return;  
        UF_CALL(UF_UGMGR_ask_config_rule(current_rule));  
        UF_CALL(UF_UGMGR_set_config_rule("Latest Working"));  
        while (prompt_for_text("Open Master Part Number", part_number))  
        {  
            if (ask_configured_rev(part_number, revision_id))  
            {  
                UF_CALL(UF_UGMGR_encode_part_filename(part_number, revision_id,  
                    "", "", encoded_name));  
                if (!open_part(encoded_name, &part))  
                {  
                    UF_CALL(UF_PART_file_name_for_display(encoded_name, &opened));  
                    WRITE_S(opened);  
                    UF_free(opened);  
                }  
            }  
        }  
        UF_CALL(UF_UGMGR_set_config_rule(current_rule));  
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

> 这段NX Open API代码的主要功能是：
>
> 1. 首先检查NX Manager是否激活，如果激活则继续，否则退出。
> 2. 获取当前的配置规则，并临时设置为“最新工作版本”。
> 3. 循环提示用户输入要打开的主零件号，如果用户输入了零件号，则：查询该零件号的配置版本号。使用配置版本号编码零件文件名。尝试打开该零件文件，并显示文件名。
> 4. 查询该零件号的配置版本号。
> 5. 使用配置版本号编码零件文件名。
> 6. 尝试打开该零件文件，并显示文件名。
> 7. 最后，恢复之前的配置规则。
>
> 代码的关键点包括：
>
> 这段代码实现了根据用户输入的零件号打开最新工作版本的功能。
>
