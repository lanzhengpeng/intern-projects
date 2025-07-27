### 【0916】create specified part in nxmanager 创建NXManager中指定的部件

#### 代码

```cpp
    /*HEAD CREATE_SPECIFIED_PART_IN_NXMANAGER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
    #include <uf_attr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是在V18中新增的功能，用于打印系统日志信息。 */  
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
    static void do_it(void)  
    {  
        logical  
            in_ugmgr;  
        int  
            resp;  
        tag_t  
            part = NULL_TAG;  
        char  
            new_cli_name[MAX_FSPEC_SIZE+1],  
            *new_encoded_name,  
            new_part_number[UF_UGMGR_PARTNO_SIZE+1],  
            new_part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            new_part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            new_part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_name[UF_UGMGR_NAME_SIZE+1],  
            part_desc[UF_UGMGR_DESC_SIZE+1],  
            *part_type,  
            template_cli_name[MAX_FSPEC_SIZE+1],  
            *template_encoded_name,  
            template_part_number[UF_UGMGR_PARTNO_SIZE+1],  
            template_part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            template_part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            template_part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_ATTR_value_t  
            value;  
        UF_CALL(UF_is_ugmanager_active(&in_ugmgr));  
        if (!in_ugmgr) return;  
        if (!UF_CALL(UF_UI_ugmgr_ask_create_part_name(new_cli_name, &part_type,  
                template_cli_name, &resp)) && (resp == UF_UI_OK))  
        {  
            WRITE_S(new_cli_name);  
            UF_CALL(UF_UGMGR_convert_name_from_cli(new_cli_name,  
                &new_encoded_name));  
            WRITE_S(new_encoded_name);  
            UF_CALL(UF_UGMGR_decode_part_filename(new_encoded_name,  
                new_part_number, new_part_revision, new_part_file_type,  
                new_part_file_name));  
            WRITE_S(new_part_number);  
            WRITE_S(new_part_revision);  
            WRITE_S(new_part_file_type);  
            WRITE_S(new_part_file_name);  
            WRITE_S(part_type);  
            WRITE_S(template_cli_name);  
            if (strcmp(template_cli_name, ""))  
            {  
                UF_CALL(UF_UGMGR_convert_name_from_cli(template_cli_name,  
                    &template_encoded_name));  
                WRITE_S(template_encoded_name);  
                UF_CALL(UF_UGMGR_decode_part_filename(template_encoded_name,  
                    template_part_number, template_part_revision,  
                    template_part_file_type, template_part_file_name));  
                WRITE_S(template_part_number);  
                WRITE_S(template_part_revision);  
                WRITE_S(template_part_file_type);  
                WRITE_S(template_part_file_name);  
                UF_CALL(UF_UGMGR_new_part_from_template(new_encoded_name,  
                    part_type, template_encoded_name, &part));  
                UF_free(template_encoded_name);  
            }  
            else  
            {  
                UF_CALL(UF_UGMGR_new_part_from_template(new_encoded_name,  
                    part_type, "", &part));  
            }  
            UF_free(part_type);  
            UF_free(new_encoded_name);  
            if (part != NULL_TAG)  
            {  
                value.value.string = part_name;  
                UF_CALL(UF_ATTR_read_value(part, "DB_PART_NAME", UF_ATTR_string,  
                    &value));  
                WRITE_S(part_name);  
                value.value.string = part_desc;  
                UF_CALL(UF_ATTR_read_value(part, "DB_PART_DESC", UF_ATTR_string,  
                    &value));  
                WRITE_S(part_desc);  
            }  
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

> 这是段用于在NX Manager中创建新部件的NX Open C++二次开发代码。以下是代码的主要功能和流程：
>
> 1. 包含NX Open C++头文件，定义了UF_CALL宏用于错误处理。
> 2. 定义了report_error函数用于打印错误信息。
> 3. 定义了write_string_to_listing_window函数用于向日志窗口输出信息。
> 4. 定义了do_it函数用于创建新部件。
> 5. do_it函数首先检查NX Manager是否激活。
> 6. 调用UF_UI_ugmgr_ask_create_part_name获取新部件的名称和类型。
> 7. 调用UF_UGMGR_convert_name_from_cli等函数解析部件名称，获取部件号、版本、文件类型等信息。
> 8. 根据是否指定了模板，调用UF_UGMGR_new_part_from_template创建新部件。
> 9. 读取新部件的名称和描述，输出到日志窗口。
> 10. ufusr函数是NX二次开发的入口函数，调用do_it函数执行创建部件的操作。
> 11. ufusr_ask_unload函数用于卸载二次开发，返回立即卸载。
> 12. 整个流程通过NX Open C++ API调用，实现了在NX Manager中创建新部件的功能。代码结构清晰，包含了错误处理和日志输出功能。
>
