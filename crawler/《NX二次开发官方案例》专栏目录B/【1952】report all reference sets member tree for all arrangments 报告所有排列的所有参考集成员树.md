### 【1952】report all reference sets member tree for all arrangments 报告所有排列的所有参考集成员树

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
    static tag_t create_new_unique_part(void)  
    {  
        int  
            units;  
        tag_t  
            new_part,  
            old_part = UF_PART_ask_display_part();  
        char  
            unique[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_units(old_part, &units));  
        build_unique_temp_part_name(unique);  
        UF_CALL(UF_PART_new(unique, units, &new_part));  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:In case its not found，意为“以防万一找不到”。 */  
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
        into a higher level assembly when each reference set is used with  
        each arrangement.  
    */  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_arrangements,  
            n_refsets;  
        tag_t  
            default_arrangement,  
            *arrangements,  
            part = UF_PART_ask_display_part(),  
            *refsets,  
            temp_comp,  
            temp_inst,  
            temp_part;  
        double  
            abs[6] = { 1,0,0, 0,1,0 };  
        char  
            arrangement[MAX_ENTITY_NAME_SIZE+1],  
            reference_set[MAX_ENTITY_NAME_SIZE+1],  
            this_part[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        n_refsets = ask_all_reference_sets(part, &refsets);  
        UF_CALL(UF_ASSEM_ask_arrangements_in_part(part, &n_arrangements,  
            &arrangements));  
        UF_CALL(UF_ASSEM_ask_default_arrangement(part, &default_arrangement));  
        UF_CALL(UF_DISP_set_display(FALSE));  
        temp_part = create_new_unique_part();  
        UF_CALL(UF_PART_ask_part_name(part, this_part));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(temp_part, this_part,  
            "", NULL, abs, abs, 0, &temp_inst, &error_status));  
        report_load_status(&error_status);  
        temp_comp = UF_ASSEM_ask_part_occ_of_inst(  
            UF_ASSEM_ask_root_part_occ(temp_part), temp_inst);  
        for (ii = 0; ii < n_arrangements; ++ii)  
        {  
            UF_CALL(UF_ASSEM_set_used_arrangement(temp_comp, arrangements[ii]));  
            UF_CALL(UF_OBJ_ask_name(arrangements[ii], arrangement));  
            if (arrangement[ii] == default_arrangement)  
                ECHO("Using arrangement = %s (Default):\n", arrangement);  
            else  
                ECHO("Using arrangement = %s:\n", arrangement);  
            for (jj = 0; jj < n_refsets; ++jj)  
            {  
                UF_CALL(UF_OBJ_ask_name(refsets[jj], reference_set));  
                if (!strcmp(reference_set, "Empty")) continue;  
                UF_CALL(UF_ASSEM_replace_refset(1, &temp_comp, reference_set));  
                ECHO("  %s contents:\n", reference_set);  
                report_component_member_tree(temp_comp, 4);  
                ECHO("\n");  
            }  
            ECHO("\n");  
        }  
        UF_CALL(UF_PART_set_display_part(part));  
        UF_CALL(UF_DISP_set_display(TRUE));  
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

> 这段代码是一个NX二次开发示例，主要用于遍历和分析NX装配中的引用集成员。以下是代码的主要功能：
>
> 1. 遍历当前显示的零件，获取所有引用集，并将它们存储在一个数组中。
> 2. 获取零件中的所有排列，并将它们存储在一个数组中。同时获取默认排列。
> 3. 创建一个临时零件，将当前显示的零件添加到临时零件的装配中。
> 4. 遍历每个排列，然后遍历每个引用集，使用当前排列替换临时零件中的引用集，并报告每个引用集的成员。
> 5. 输出每个引用集的成员树，包括组件和实例名称。
> 6. 最后，删除临时零件并恢复原始显示的零件。
> 7. 使用宏UF_CALL进行错误检查，并在出错时输出错误信息。
> 8. 定义了一些辅助函数，如allocate_memory、make_an_array、ask_next_ref_set等。
> 9. 使用ECHO函数输出信息到日志窗口和系统日志。
>
> 总体来说，该代码通过二次开发实现了对NX装配中引用集成员的遍历和分析，输出每个引用集的成员树，展示了NX二次开发的相关技巧。
>
