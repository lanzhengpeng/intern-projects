### 【1154】edit part family template column data 编辑零件族模板列数据

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_fam.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_tabnot.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
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
                case 1:                     /* Back 里海译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:Cancel的翻译为“取消”。 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:More... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:根据提供的信息，"Picked one" 翻译为“挑选一个”。 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
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
    static char *allocate_and_duplicate_string(char *old_string)  
    {  
        int  
            n,  
            resp;  
        char  
            *new_string = NULL;  
        n = (int)strlen(old_string);  
        if (n > 0)  
        {  
            new_string = (char *)UF_allocate_memory((n+1) * sizeof(char), &resp);  
            strcpy(new_string, old_string);  
        }  
        return new_string;  
    }  
    static void update_all_tabnotes(void)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            tnote = NULL_TAG;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type,  
            &tnote)) && (tnote != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(tnote, &type, &subtype));  
            if (subtype == UF_tabular_note_subtype)  
                UF_CALL(UF_TABNOT_update(tnote));  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            is_template;  
        int  
            col_index,  
            family_count,  
            ii;  
        tag_t  
            *families,  
            part = UF_PART_ask_display_part();  
        char  
            new_value[133],  
            **column_names;  
        UF_FAM_family_data_t  
            family_data;  
        UF_FAM_attribute_data_t  
            attribute_data;  
        UF_FAM_member_data_t  
            prompt,  
            member_data;  
        UF_CALL(UF_PART_is_family_template(part, &is_template));  
        if (!is_template) return;  
        UF_CALL(UF_PART_ask_families(part, &family_count, &families));  
        UF_CALL(UF_FAM_ask_family_data(families[0], &family_data));  
        UF_CALL(allocate_memory(family_data.attribute_count * sizeof(char *),  
            (void **)&column_names));  
        for (ii = 0; ii < family_data.attribute_count; ii++)  
        {  
            UF_CALL(UF_FAM_ask_attribute_data(family_data.attributes[ii],  
                &attribute_data));  
            UF_CALL(allocate_memory(((int)strlen(attribute_data.name)+1) * sizeof(char),  
                (void **)&column_names[ii]));  
            strcpy(column_names[ii], attribute_data.name);  
            UF_CALL(UF_FAM_free_attribute_data(&attribute_data));  
        }  
        while ((col_index = choose_one_item("Select column to edit", column_names,  
            family_data.attribute_count)) >= 0)  
        {  
            UF_CALL(UF_FAM_ask_member_column_data(families[0], 0, &prompt));  
            for (ii = 0; ii < family_data.member_count; ii++)  
            {  
                UF_CALL(UF_FAM_ask_member_row_data(families[0], ii, &member_data));  
                strncpy(new_value, member_data.values[col_index], 132);  
                new_value[132] = '\0';  
                if (prompt_for_text(prompt.values[ii], new_value))  
                {  
                    UF_free(member_data.values[col_index]);  
                    member_data.values[col_index] =  
                        allocate_and_duplicate_string(new_value);  
                    UF_CALL(UF_FAM_edit_member(families[0], ii, &member_data));  
                }  
                else break;  
                UF_CALL(UF_FAM_free_member_data(&member_data));  
            }  
            UF_CALL(UF_FAM_free_member_data(&prompt));  
        }  
        UF_free_string_array(family_data.attribute_count, column_names);  
        UF_CALL(UF_FAM_free_family_data(&family_data));  
    //  This does not update tabular notes associated to the spreadsheet  
    //  See PR 6595644  
        UF_CALL(UF_MODL_update());  
    //  This does not help either  
        update_all_tabnotes();  
        ECHO("To update any tabular notes associated to the Part Family Spreadsheet\n");  
        ECHO("use Tools-> Part Families-> Edit, then PartFamily-> Save\n");  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发示例代码，主要功能如下：
>
> 1. 初始化和终止NX Open API。
> 2. 定义了一些辅助函数，如ECHO()用于打印信息，report_error()用于报告错误，allocate_memory()用于分配内存，choose_one_item()用于选择项目，prompt_for_text()用于提示输入文本，allocate_and_duplicate_string()用于复制字符串等。
> 3. update_all_tabnotes()函数用于更新所有与表格注释关联的表格注释。
> 4. do_it()函数是主要逻辑，首先判断当前显示的部件是否为模板，如果不是则直接返回。然后获取部件的族信息，并循环遍历每个族成员，提示用户编辑族成员的各个列属性，并更新到NX中。
> 5. ufusr()函数是NX二次开发的入口函数，调用do_it()执行主要逻辑。
> 6. ufusr_ask_unload()函数用于卸载二次开发代码。
>
> 该代码主要用于编辑NX部件族模板中的族成员属性，并更新到NX中。通过对话框提示用户选择列，然后逐个编辑族成员的该列属性值。但需要注意，代码中的注释指出，更新表格注释需要通过NX的“工具”->“部件族”->“编辑”->“保存”菜单来实现，直接调用API函数无法更新表格注释。
>
