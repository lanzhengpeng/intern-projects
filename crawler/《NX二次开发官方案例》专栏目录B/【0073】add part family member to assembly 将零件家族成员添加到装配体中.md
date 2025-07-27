### 【0073】add part family member to assembly 将零件家族成员添加到装配体中

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_csys.h>  
    #include <uf_fam.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static logical prompt_for_existing_part_name(char *prompt, char *fspec)  
    {  
        logical  
            unused;  
        int  
            resp;  
        UF_CALL(UF_UI_set_prompt(prompt));  
        if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
            || (resp == UF_UI_CANCEL)) return FALSE;  
        return TRUE;  
    }  
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
        }  
    }  
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:Cancel */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:More... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 译:翻译为：挑选一个。 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void report_instance_status(UF_ASSEM_instance_status_t *instance_status)  
    {  
        WRITE_L(instance_status->child_changed);  
        WRITE_D(instance_status->current_child);  
        WRITE_D(instance_status->previous_child);  
        WRITE_S(instance_status->info);  
        if (instance_status->info != NULL) UF_free(instance_status->info);  
    }  
    static void do_it(void)  
    {  
        logical  
            is_template;  
        int  
            index,  
            n_families;  
        tag_t  
            *families,  
            instance,  
            part = UF_PART_ask_display_part(),  
            template_part;  
        double  
            origin[3],  
            matrix[9];  
        char  
            template_spec[MAX_FSPEC_SIZE+1] = { "" };  
        UF_PART_load_status_t  
            error_status;  
        UF_ASSEM_instance_intent_t  
            intent;  
        UF_ASSEM_instance_status_t  
            status;  
        UF_FAM_intent_data_t  
            fam_intent;  
        UF_FAM_family_data_t  
            family_data;  
        UF_FAM_member_data_t  
            member_data;  
        intent.fam_intent = &fam_intent;  
        if (prompt_for_existing_part_name("Template", template_spec))  
        {  
            ask_wcs_info(origin, matrix);  
            UF_CALL(UF_ASSEM_add_part_to_assembly(part, template_spec, NULL, NULL,  
                origin, matrix, 0, &instance, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
            template_part = UF_ASSEM_ask_prototype_of_occ(  
                UF_ASSEM_ask_part_occ_of_inst(  
                UF_ASSEM_ask_root_part_occ(part), instance));  
            UF_CALL(UF_PART_is_family_template(template_part, &is_template));  
            if (!is_template) return;  
            UF_CALL(UF_PART_ask_families(template_part, &n_families, &families));  
            if (n_families <= 0) return;  
            intent.fam_intent->family = families[0];  
            UF_CALL(UF_FAM_ask_family_data(families[0], &family_data));  
            UF_CALL(UF_FAM_ask_member_column_data(families[0], 0, &member_data));  
            index = choose_one_item("Select family member part",  
                member_data.values, member_data.value_count);  
            intent.fam_intent->attribute_count = 1;  
            intent.fam_intent->attributes = &family_data.attributes[0];  
            intent.fam_intent->match_criteria = &member_data.values[index];  
            UF_CALL(UF_ASSEM_set_instance_intent(instance, &intent));  
            UF_CALL(UF_ASSEM_eval_instance_intent(&instance, TRUE, &status));  
            report_instance_status(&status);  
            UF_FAM_free_member_data(&member_data);  
            UF_CALL(UF_FAM_free_family_data(&family_data));  
            UF_free(families);  
        }  
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

> 这段代码是一个NX Open API的二次开发示例，其主要功能是向装配体中添加一个组件，并将其设置为族成员。
>
> 主要步骤包括：
>
> 1. 获取当前显示的组件作为装配体的根组件。
> 2. 通过对话框提示用户输入模板组件的文件路径。
> 3. 获取当前坐标系信息。
> 4. 向装配体中添加模板组件，并返回添加的组件实例。
> 5. 检查模板组件是否为族模板。
> 6. 获取模板组件所属的族。
> 7. 获取族的数据，包括族成员。
> 8. 让用户选择一个族成员作为实例的意图。
> 9. 设置实例的意图，并评估意图。
> 10. 输出评估结果。
> 11. 释放资源。
>
> 代码中使用了UF_PART、UF_ASSEM、UF_FAM等模块的函数来实现以上功能，并定义了一些辅助函数来简化代码。通过这个示例，展示了如何使用NX Open API进行二次开发，实现组件的装配和管理。
>
