### 【0139】add assembly and change name prefix on all components 向装配体中添加组件，并更改所有组件名称的前缀

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_ugmgr.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static logical add_proto_to_list(tag_t comp, void *user_data)  
    {  
        tag_t  
            proto = UF_ASSEM_ask_prototype_of_occ(comp);  
        uf_list_p_t  
            *list = (uf_list_p_t *)user_data;  
        UF_CALL(UF_MODL_put_list_item(*list, proto));  
        return TRUE;  
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
    static int ask_all_assembly_parts(tag_t part, tag_t **parts)  
    {  
        tag_t  
            cset;  
        uf_list_p_t  
            part_list;  
        UF_PART_load_status_t  
            status;  
        UF_CALL(UF_MODL_create_list(&part_list));  
        UF_CALL(UF_MODL_put_list_item(part_list, part));  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &cset));  
        if (cset != NULL_TAG)  
        {  
            UF_CALL(UF_PART_open_cset(cset, &status));  
            report_load_status(&status);  
            UF_CALL(UF_ASSEM_apply_to_cset(cset, add_proto_to_list, &part_list));  
        }  
        return make_an_array(&part_list, parts);  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void do_it(void)  
    {  
        logical  
            in_nxmanager;  
        int  
            ii,  
            n_chars = 3,  
            n_parts;  
        tag_t  
            added_part,  
            instance,  
            *parts,  
            top_part = UF_ASSEM_ask_work_part();  
        double  
            identity[6] = { 1,0,0, 0,1,0 },  
            zero[3] = { 0,0,0 };  
        char  
            assem_spec[MAX_FSPEC_SIZE+1] = { "" },  
            dspec[MAX_FSPEC_SIZE+1],  
            newpart_number[UF_UGMGR_PARTNO_SIZE+1],  
            new_spec[MAX_FSPEC_SIZE+1],  
            old_spec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1],  
            newpart_name[MAX_FSPEC_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            prefix[MAX_FSPEC_SIZE+1] = { "GTAC" },  
            renamed[MAX_FSPEC_SIZE+1],  
            to_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        UF_ASSEM_options_t  
            old_options,  
            new_options;  
        if ((top_part == NULL_TAG) ||  
            !prompt_for_existing_part_name("Add Assembly", assem_spec) ||  
            !prompt_for_an_integer("Replace prefix", "No. Chars", &n_chars) ||  
            !prompt_for_text("New prefix", prefix)) return;  
        UF_CALL(UF_ASSEM_ask_assem_options(&old_options));  
        new_options = old_options;  
        new_options.load_fully = UF_ASSEM_load_components_fully;  
        UF_CALL(UF_ASSEM_set_assem_options(&new_options));  
        if (UF_CALL(UF_ASSEM_add_part_to_assembly(top_part, assem_spec, NULL,  
            NULL, zero, identity, -1, &instance, &status))) return;  
        report_load_status(&status);  
        added_part = UF_ASSEM_ask_prototype_of_occ(UF_ASSEM_ask_part_occ_of_inst(  
            UF_ASSEM_ask_root_part_occ(top_part), instance));  
        UF_CALL(UF_is_ugmanager_active(&in_nxmanager));  
        n_parts = ask_all_assembly_parts(added_part, &parts);  
        for (ii = 0; ii< n_parts; ii++)  
        {  
            UF_CALL(UF_PART_ask_part_name(parts[ii], old_spec));  
            if (in_nxmanager)  
            {  
                UF_CALL(UF_UGMGR_decode_part_filename(old_spec, part_number,  
                        part_revision, part_file_type, part_file_name));  
                strcpy(newpart_number, prefix);  
                if (strlen(part_number) > n_chars)  
                    strcat(newpart_number, &part_number[n_chars]);  
                else  
                    continue;  
                UF_CALL(UF_UGMGR_encode_part_filename(newpart_number, part_revision,  
                    part_file_type, part_file_name, new_spec));  
            }  
            else  
            {  
                uc4576(old_spec, 2, dspec, part_name);  
                strcpy(newpart_name, prefix);  
                if (strlen(part_name) > n_chars)  
                    strcat(newpart_name, &part_name[n_chars]);  
                else  
                    continue;  
                uc4575(dspec, 2, newpart_name, new_spec);  
            }  
            if (!UF_CALL(UF_PART_rename(parts[ii], new_spec)))  
            {  
                UF_CALL(UF_PART_name_for_display(old_spec, renamed));  
                UF_CALL(UF_PART_name_for_display(new_spec, to_name));  
                WRITE_S(renamed);  
                WRITE_S(to_name);  
            }  
        }  
        UF_CALL(UF_ASSEM_set_assem_options(&old_options));  
        UF_free(parts);  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 添加装配：用户可以指定一个装配文件，将其添加到当前的工作部件中。
> 2. 替换前缀：用户可以指定一个新前缀，然后代码会遍历新添加的装配中的所有部件，将部件名称中的前缀替换为新前缀。
> 3. 报告加载状态：在添加装配的过程中，会报告每个部件的加载状态。
> 4. NX Manager模式：代码会检测是否在NX Manager模式下运行，并据此采用不同的方法来处理部件名称。
> 5. 重命名部件：代码会遍历新添加的装配中的所有部件，将它们的名称替换为新前缀加上原名称的剩余部分，并报告重命名结果。
> 6. 恢复装配选项：代码会在执行前后保存和恢复装配的加载选项。
> 7. 内存释放：代码会释放分配的内存。
>
> 总体来说，这段代码通过用户交互，实现了向工作部件中添加新装配，并对新装配中的部件进行批量重命名操作。
>
