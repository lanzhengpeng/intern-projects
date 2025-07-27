### 【1931】report all members of selected reference sets 报告所选参考集的所有成员

#### 代码

```cpp
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <uf_ugmgr.h>  
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
    static tag_t ask_next_ref_set(tag_t part, tag_t refset)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
            if ((subtype == UF_reference_design_subtype) ||  
                (subtype == UF_reference_design_model_subtype) ||  
                (subtype == UF_reference_design_lightweight_subtype))  
                return refset;  
        }  
        return NULL_TAG;  
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
    static int ask_all_reference_sets(tag_t part, tag_t **refsets)  
    {  
        tag_t  
            refset = NULL_TAG;  
        uf_list_p_t  
            refset_list;  
        UF_CALL(UF_MODL_create_list(&refset_list));  
        while ((refset = ask_next_ref_set(part, refset)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(refset_list, refset));  
        return (make_an_array(&refset_list, refsets));  
    }  
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
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
                case 1:                     /* Back 里海译:翻译Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:More...的翻译为：更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:挑选了一个 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_a_reference_set(char *prompt)  
    {  
        int  
            ii,  
            n_refsets,  
            which;  
        tag_t  
            part = UF_ASSEM_ask_work_part(),  
            the_refset,  
            *refsets;  
        char  
            **refset_names;  
        n_refsets = ask_all_reference_sets(part, &refsets);  
        UF_CALL(allocate_string_array(n_refsets, MAX_ENTITY_NAME_SIZE+1,  
            &refset_names));  
        for (ii = 0; ii < n_refsets; ii++)  
            UF_CALL(UF_OBJ_ask_name(refsets[ii], refset_names[ii]));  
        if ((which = choose_one_item(prompt, refset_names, n_refsets)) == -1)  
            the_refset = NULL_TAG;  
        else  
            the_refset = refsets[which];  
        UF_free(refsets);  
        UF_free_string_array(n_refsets, refset_names);  
        return the_refset;  
    }  
    static void build_unique_temp_part_name(char *fspec)  
    {  
        logical  
            tceng;  
        char  
            *tmp_dir,  
            unique[MAX_FSPEC_SIZE+1];  
        UF_CALL(uc4577(unique));  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (tceng)  
            UF_CALL(UF_UGMGR_encode_part_filename(unique, "A", "", "", fspec));  
        else  
        {  
            UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
            UF_CALL(uc4575(tmp_dir, 2, unique, fspec));  
        }  
    }  
    static tag_t create_new_unique_part_quietly(void)  
    {  
        int  
            units;  
        tag_t  
            new_part,  
            old_part = UF_PART_ask_display_part();  
        char  
            unique[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_PART_ask_units(old_part, &units));  
        build_unique_temp_part_name(unique);  
        UF_CALL(UF_PART_new(unique, units, &new_part));  
        UF_CALL(UF_PART_set_display_part(old_part));  
        UF_CALL(UF_DISP_set_display(TRUE));  
        return new_part;  
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
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void ask_object_type_descriptor(int type, int subtype, char *descriptor)  
    {  
        int  
            ii,  
            a_type;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            *ptr,  
            uoth_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        switch (type)  
        {  
            case UF_circle_type: /* non-std format in the header file 里海译:在头文件中使用非标准格式 */  
                strcpy(descriptor, "arc");  
                return;  
            case UF_OBJ_EXPR_TYPE:  
                strcpy(descriptor, "expression");  
                return;  
            case UF_OBJ_PART_TYPE:  
                strcpy(descriptor, "part");  
                return;  
            default:  
                break;  
        }  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf_object_types.h", uoth_file));  
        if ((in_file = fopen(uoth_file, "r")) == NULL)  
        {  
            strcpy(descriptor, "(uf_object_types.h not found)");  
            return;  
        }  
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    if (strcmp(define, "gfem_load")) break;  
                }  
            }  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if (ptr = strstr(define, "_type")) break;  
                if (a_type == subtype)  
                {  
                    if (ptr = strstr(define, "_subtype")) *ptr = '\0';  
                    if (strstr(define, descriptor) == define)  
                        strcpy(descriptor, define);  
                    else  
                    {  
                        strcat(descriptor, "/");  
                        strcat(descriptor, define);  
                    }  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
        while (ptr = strchr(descriptor, '_')) *ptr = ' ';  
    }  
    static void report_component_member_tree(tag_t comp, int indent)  
    {  
        logical  
            parent_suppressed,  
            suppressed;  
        int  
            ii,  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            abs[6] = { 1,0,0, 0,1,0 },  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            desc[MAX_LINE_SIZE+1],  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            reference_set[MAX_ENTITY_NAME_SIZE+1];  
        while ((object = UF_ASSEM_cycle_ents_in_part_occ(comp, object)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if ((subtype == UF_solid_face_subtype) ||  
                (subtype == UF_solid_edge_subtype)) continue;  
            ask_object_type_descriptor(type, subtype, desc);  
            if (type == UF_component_type)  
            {  
                UF_CALL(UF_ASSEM_ask_component_data(object, part_name,  
                    reference_set, instance_name, origin, csys_matrix, transform));  
                for (ii = 0; ii < indent; ++ii) ECHO(" ");  
                ECHO("%s - %s uses reference set: %s",  
                    desc, instance_name, reference_set);  
                UF_CALL(UF_ASSEM_ask_part_occ_suppress_state(object,  
                    &suppressed, &parent_suppressed));  
                if (suppressed) ECHO(" (Suppressed)\n");  
                else  
                {  
                    ECHO("\n");  
                    report_component_member_tree(object, indent+2);  
                }  
            }  
            else  
            {  
                for (ii = 0; ii < indent; ++ii) ECHO(" ");  
                ECHO("%s\n", desc);  
            }  
        }  
    }  
    /*  
        UF_ASSEM_ask_ref_set_members only returns the immediate members of the  
        reference set.  This reports every object that will be brought  
        into a higher level assembly when the reference set is used.  
    */  
    static void report_all_reference_set_members(tag_t temp_part, tag_t refset)  
    {  
        tag_t  
            arr,  
            new_inst,  
            part = UF_PART_ask_display_part();  
        double  
            abs[6] = { 1,0,0, 0,1,0 };  
        char  
            default_arrangement[MAX_ENTITY_NAME_SIZE+1],  
            reference_set[MAX_ENTITY_NAME_SIZE+1],  
            this_part[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_PART_ask_part_name(part, this_part));  
        UF_CALL(UF_OBJ_ask_name(refset, reference_set));  
        UF_CALL(UF_ASSEM_ask_default_arrangement(part, &arr));  
        UF_CALL(UF_OBJ_ask_name(arr, default_arrangement));  
        ECHO("%s contents in Arrangement %s (Default):\n", reference_set,  
            default_arrangement);  
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_PART_set_display_part(temp_part));  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(temp_part, this_part,  
            reference_set, NULL, abs, abs, 0, &new_inst, &error_status));  
        report_load_status(&error_status);  
        report_component_member_tree(UF_ASSEM_ask_part_occ_of_inst(  
                       UF_ASSEM_ask_root_part_occ(temp_part), new_inst), 2);  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
        UF_CALL(UF_PART_set_display_part(part));  
        UF_CALL(UF_DISP_set_display(TRUE));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            temp_part = NULL_TAG,  
            refset;  
        while ((refset = choose_a_reference_set("Report ALL members"))  
            != NULL_TAG)  
        {  
            if (temp_part == NULL_TAG)  
                temp_part = create_new_unique_part_quietly();  
            report_all_reference_set_members(temp_part, refset);  
        }  
        if (temp_part != NULL_TAG)  
            UF_CALL(UF_PART_close(temp_part, 0, 1));  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一些辅助函数，用于错误处理、内存分配、创建列表等。
> 2. 实现了遍历组件成员树的功能，递归地打印出组件及其所有子组件的信息。
> 3. 实现了查询参考集所有成员的功能，递归地打印出参考集及其所有子参考集的成员信息。
> 4. 主函数do_it()会循环提示用户选择一个参考集，并打印出该参考集的所有成员信息。
> 5. ufusr()是NX调用的入口函数，初始化NX环境后调用do_it()函数，执行完毕后关闭NX环境。
> 6. ufusr_ask_unload()函数用于处理NX卸载时的相关操作。
>
> 总体来说，该代码实现了在NX中查询并打印出参考集及其所有成员的功能，可用于参考集的查看和管理。代码结构清晰，功能完整，是一个典型的NX二次开发示例。
>
