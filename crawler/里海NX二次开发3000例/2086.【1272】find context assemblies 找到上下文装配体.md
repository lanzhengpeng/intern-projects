### 【1272】find context assemblies 找到上下文装配体

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_wave.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
    //  See ER 6669518  
    #include <uf_kf.h>  
    static logical is_linked_feature(tag_t feature)  
    {  
        logical  
            is_init = false,  
            is_linked = false;  
        char  
            rule[133],  
            unique[UF_CFI_MAX_FILE_NAME_SIZE],  
            *value;  
        UF_CALL(UF_KF_is_initialized(&is_init));  
        if (!is_init)  
            UF_CALL(UF_KF_init_part(UF_ASSEM_ask_work_part()));  
        UF_CALL(uc4577(unique));  
        sprintf(rule, "mqc_isLinkedFeature(%d)", feature);  
        UF_CALL(UF_KF_create_rule_no_update("root:", unique, "Boolean", rule, ""));  
        sprintf(rule, "root:%s:", unique);  
        UF_CALL(UF_KF_evaluate_rule_to_string(rule, &value));  
        UF_CALL(UF_KF_delete_instance_rule("root:", unique));  
        if (strcmp(value, "TRUE") == 0) is_linked = true;  
        UF_free(value);  
        return is_linked;  
    }  
    static tag_t ask_next_wave_linked_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (is_linked_feature(feat)) return feat;  
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
    static int ask_all_open_parts(tag_t **parts)  
    {  
        int  
            ii,  
            n;  
        n = UF_PART_ask_num_parts();  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)parts));  
        for (ii = 0; ii < n; ii++)  
            (*parts)[ii] = UF_PART_ask_nth_part(ii);  
        return n;  
    }  
    static void load_all_wave_parents(tag_t part)  
    {  
        int  
            ii,  
            n_parts,  
            *statuses;  
        char  
            **file_names,  
            msg[133];  
        UF_CALL(UF_WAVE_load_parents(part, UF_ASSEM_all, &n_parts, &file_names,  
            &statuses));  
        if (n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(statuses);  
            UF_free_string_array(n_parts, file_names);  
        }  
    }  
    static tag_t ask_wave_source_part(tag_t feature)  
    {  
        logical  
            is_broken;  
        tag_t  
            source_object,  
            source_part;  
        if (!UF_CALL(UF_WAVE_is_link_broken(feature, &is_broken)) &&  
            !is_broken &&  
            !UF_CALL(UF_WAVE_ask_link_source(feature, TRUE, &source_object)) &&  
            !UF_CALL(UF_OBJ_ask_owning_part(source_object, &source_part)))  
            return source_part;  
        return NULL_TAG;  
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
    static void ensure_part_fully_loaded(tag_t part)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        if (!UF_CALL(UF_PART_ask_part_name(part, partname))  
            && ((UF_PART_is_loaded(partname)) != 1))  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void report_occ_path(char *name, tag_t part_occ)  
    {  
        tag_t  
            inst,  
            parent,  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        ECHO("%s", name);  
        part = UF_ASSEM_ask_prototype_of_occ(part_occ);  
        ask_part_shortname(part, part_name);  
        ECHO("%s", part_name);  
        if ((inst = UF_ASSEM_ask_inst_of_part_occ(part_occ)) != NULL_TAG)  
            ECHO(" (%d)", inst);  
        UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &parent));  
        if (parent == NULL_TAG)  
        {  
            ECHO("\n");  
        }  
        else  
        {  
            ECHO(" -> ");  
            report_occ_path("", parent);  
        }  
    }  
    static void report_feature_sysname(tag_t feature)  
    {  
        char  
            *sysname;  
        UF_CALL(UF_MODL_ask_feat_sysname(feature, &sysname));  
        ECHO("%s\n", sysname);  
        UF_free(sysname);  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_s_occs,  
            n_t_occs,  
            n_parts;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *parts,  
            *s_occs,  
            *t_occs,  
            source,  
            wave_feat = NULL_TAG;  
        char  
            context_assy[MAX_FSPEC_SIZE+1];  
        n_parts = ask_all_open_parts(&parts);  
    /*  Make sure all WAVE source parts are loaded 里海译:确保所有WAVE源部件都已加载。 */  
        for (ii = 0; ii < n_parts; ii++)  
        {  
            load_all_wave_parents(parts[ii]);  
        }  
        while ((wave_feat = ask_next_wave_linked_feature(part, wave_feat))  
            != NULL_TAG)  
        {  
            report_feature_sysname(wave_feat);  
            if ((source = ask_wave_source_part(wave_feat)) == NULL_TAG) continue;  
            for (ii = 0; ii < n_parts; ii++)  
            {  
                if ((parts[ii] == part) || (parts[ii] == source)) continue;  
                ensure_part_fully_loaded(parts[ii]);  
                n_t_occs = UF_ASSEM_ask_occs_of_part(parts[ii], part, &t_occs);  
                n_s_occs = UF_ASSEM_ask_occs_of_part(parts[ii], source, &s_occs);  
                if ((n_s_occs > 0) && (n_t_occs > 0))  
                {  
                    UF_PART_ask_part_name(parts[ii], context_assy);  
                    ECHO("context_assy = %s\n", context_assy);  
                    for (jj = 0; jj < n_s_occs; jj++)  
                        report_occ_path(" Source: ", s_occs[jj]);  
                    for (jj = 0; jj < n_t_occs; jj++)  
                        report_occ_path(" Target: ",t_occs[jj]);  
                }  
                if (n_s_occs > 0) UF_free(s_occs);  
                if (n_t_occs > 0) UF_free(t_occs);  
            }  
        }  
        if (n_parts > 0) UF_free(parts);  
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

> 这段代码是NX Open C++ API编写的二次开发代码，主要用于处理WAVE链接特征。代码的主要功能包括：
>
> 1. 遍历当前显示部件中的所有特征，并检查哪些特征是WAVE链接特征。
> 2. 对于每个WAVE链接特征，找出它的源部件。
> 3. 遍历当前打开的所有部件，确保源部件和目标部件都被完全加载。
> 4. 对于每个源部件和目标部件，找出它们在当前部件中的装配路径。
> 5. 输出源部件和目标部件的装配路径，以检查WAVE链接是否正确。
> 6. 使用自定义的错误处理函数来报告API调用错误。
> 7. 使用UF_SYSLOG宏输出信息到系统日志窗口。
> 8. 使用UF_UI宏打开和写入列表窗口。
> 9. 定义了自定义内存分配函数，用于调用UF_allocate_memory。
> 10. 定义了自定义字符串截取函数，用于调用UF_string_subst。
> 11. 定义了自定义规则函数，用于调用知识融合API。
> 12. 定义了自定义确保部件完全加载的函数。
> 13. 定义了自定义询问部件短名称的函数。
> 14. 定义了自定义报告装配路径的函数。
> 15. 定义了自定义报告特征系统名称的函数。
> 16. 定义了主函数do_it，用于执行上述步骤。
> 17. 定义了UFusr函数，作为NX二次开发的入口函数。
> 18. 定义了卸载函数，用于卸载二次开发库。
>
> 总的来说，这段代码利用NX Open C++ API，实现了对WAVE链接特征的检查，以确保模型的正确性。
>
