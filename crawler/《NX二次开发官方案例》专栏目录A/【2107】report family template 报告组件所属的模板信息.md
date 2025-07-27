### 【2107】report family template 报告组件所属的模板信息

#### 代码

```cpp
    /*HEAD REPORT_FAMILY_TEMPLATE CCC UFUN */  
    /*  These methods are obsoleted in V16 by UF_PART_ask_template_filename 里海译:这些方法在V16版本中已被UF_PART_ask_template_filename取代而过时。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，V18版本新增了一个函数UF_print_syslog。这个函数用于打印系统日志。 */  
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
    static void report_family_template(tag_t member_part)  
    {  
        logical  
            is_member;  
        int  
            member_index;  
        unsigned int  
            sub_file_id,  
            version;  
        tag_t  
            family,  
            instance,  
            template_part;  
        char  
            *file_data,  
            fspec[MAX_FSPEC_SIZE+1],  
            *handle;  
        UF_PART_ask_part_name(member_part, fspec);  
        printf("%s is", fspec);  
        UF_CALL(UF_PART_is_family_instance(member_part, &is_member));  
        if (is_member)  
        {  
            UF_CALL(UF_PART_ask_family_instance(member_part, &instance));  
            UF_CALL(UF_FAM_ask_instance_data(instance, &family, &member_index));  
            UF_CALL(UF_OBJ_ask_owning_part(family, &template_part));  
            if (template_part != NULL_TAG)  
            {  
                UF_PART_ask_part_name(template_part, fspec);  
            }  
            else  
            {  
                handle = UF_TAG_ask_handle_of_tag(family);  
                UF_CALL(UF_TAG_decompose_handle(handle, &file_data, &sub_file_id,  
                    &version));  
                strcpy(fspec, &file_data[2]);  
                strcpy(strstr(fspec, " "), "");  
                UF_free(file_data);  
                UF_free(handle);  
            }  
            printf(" a member of the family defined in %s\n", fspec);  
        }  
        else  
            printf(" NOT a family member\n");  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        report_family_template(part);  
    }  
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

> 这是一段用于报告NX零件是否为模板家族成员的二次开发代码。主要功能包括：
>
> 1. 定义了report_error函数，用于打印错误信息。
> 2. 定义了report_family_template函数，用于查询给定零件是否为模板家族成员，如果是，则打印模板文件名。
> 3. do_it函数获取当前工作零件，并调用report_family_template函数进行查询。
> 4. ufusr函数初始化NX，调用do_it函数，然后终止NX。
> 5. ufusr_ask_unload函数返回立即卸载标志。
> 6. 注释表明，部分函数在V16版本后已被新函数取代，但这段代码仍然有效。
> 7. 使用UF_PRINT_SYSLOG等函数输出信息到日志窗口。
> 8. 通过UF_PART系列函数查询零件信息。
> 9. 通过UF_FAM系列函数查询家族信息。
> 10. 通过UF_OBJ系列函数查询对象信息。
>
> 这段代码主要用于查询零件是否为模板家族成员，如果是，则打印模板文件名，从而实现报告模板家族成员关系的目的。
>
