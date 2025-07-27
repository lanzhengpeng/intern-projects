### 【0760】create master and specification or manifestation in nxmanager 在NX Manager中创建主模型和规范或表示

#### 代码

```cpp
    /*HEAD CREATE_MASTER_AND_SPECIFICATION_OR_MANIFESTATION_IN_TCENG CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_ugmgr.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        ECHO(msg);  
    }  
    static void create_block(void)  
    {  
        int  
            units;  
        tag_t  
            block,  
            part = UF_PART_ask_display_part();  
        double  
            zero[3] = { 0, 0, 0 };  
        char  
            *inch_size[3] = { "4.0", "4.0", "4.0" },  
            *metric_size[3] = { "100", "100", "100" };  
        UF_CALL(UF_PART_ask_units(part, &units));  
        if (units == UF_PART_ENGLISH)  
            UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, zero, inch_size, &block));  
        else  
            UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, zero, metric_size, &block));  
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
    static void do_it(void)  
    {  
        int  
            resp;  
        tag_t  
            instance,  
            master,  
            spec;  
        double  
            origin[3] = {0, 0, 0},  
            matrix[6] = {1, 0, 0, 0, 1, 0};  
        char  
            master_cli_name[MAX_FSPEC_SIZE+1] = {""},  
            spec_cli_name[MAX_FSPEC_SIZE+1] = {""},  
            *part_type,  
            template_cli_name[MAX_FSPEC_SIZE+1] = {""};  
        UF_PART_load_status_t  
            error_status;  
        uc1601("Specify name for new Master", TRUE);  
        if (UF_CALL(UF_UI_ugmgr_ask_create_part_name(master_cli_name, &part_type,  
            template_cli_name, &resp)) || (resp != UF_UI_OK)) return;  
        WRITE_S(master_cli_name);  
        WRITE_S(part_type);  
        WRITE_S(template_cli_name);  
    /*  UF_UI_ugmgr_ask_create_part_name does NOT return selected template name  
        see PR 5442455 */  
        uc1601("Specify Template for new Master", TRUE);  
        if (UF_CALL(UF_UI_ask_open_part_filename(template_cli_name, NULL, &resp))  
            || (resp != UF_UI_OK)) return;  
        WRITE_S(template_cli_name);  
        UF_CALL(UF_UGMGR_new_part_from_template(master_cli_name, part_type,  
            template_cli_name, &master));  
        UF_free(part_type);  
        create_block();  
        UF_CALL(UF_PART_save());  
        uc1601("Specify name for new Specification or Manifestation", TRUE);  
        if (UF_CALL(UF_UI_ugmgr_ask_create_part_name(spec_cli_name, &part_type,  
            template_cli_name, &resp)) || (resp != UF_UI_OK)) return;  
        WRITE_S(spec_cli_name);  
        WRITE_S(part_type);  
        WRITE_S(template_cli_name);  
    /*  UF_UI_ugmgr_ask_create_part_name does NOT return selected template name  
        see PR 5442455 */  
        uc1601("Specify Template for new Specification or Manifestation", TRUE);  
        if (UF_CALL(UF_UI_ask_open_part_filename(template_cli_name, NULL, &resp))  
            || (resp != UF_UI_OK)) return;  
        WRITE_S(template_cli_name);  
        UF_CALL(UF_UGMGR_new_part_from_template(spec_cli_name, part_type,  
            template_cli_name, &spec));  
        UF_free(part_type);  
        UF_CALL(UF_ASSEM_add_part_to_assembly(spec, master_cli_name, NULL, NULL,  
            origin, matrix, 0, &instance, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
        UF_CALL(UF_PART_save());  
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

```

#### 代码解析

> 这段代码是NX二次开发代码，主要用于在NX中创建Master和Specification或Manifestation。
>
> 主要功能包括：
>
> 1. 定义了错误报告宏函数，用于输出错误信息。
> 2. 创建一个块体模型作为模板。
> 3. 使用UI函数获取用户输入的Master名称和模板文件名，并创建Master模型。
> 4. 获取用户输入的Specification名称和模板文件名，并创建Specification模型。
> 5. 将Specification添加到Assembly中。
> 6. 保存模型。
> 7. ufusr函数作为主入口函数，调用do_it函数执行上述功能。
> 8. 提供了卸载函数。
>
> 通过UI交互获取用户输入，使用UF函数创建和装配模型，最后保存。整体流程清晰，注释详细，易于理解。
>
