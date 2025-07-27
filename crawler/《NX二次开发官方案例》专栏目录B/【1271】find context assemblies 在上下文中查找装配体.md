### 【1271】find context assemblies 在上下文中查找装配体

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

> 这段代码是NX的二次开发代码，其主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息。
> 2. 定义了一个判断特征是否为链接特征的函数is_linked_feature，通过知识融合框架(KF)来实现。
> 3. 定义了一个获取下一个链接特征的函数ask_next_wave_linked_feature。
> 4. 定义了一些内存分配、获取当前打开部件、加载WAVE父部件等辅助函数。
> 5. 定义了一个确保部件完全加载的函数ensure_part_fully_loaded。
> 6. 定义了一个报告装配路径的函数report_occ_path。
> 7. 定义了一个报告特征系统名的函数report_feature_sysname。
> 8. 主函数do_it遍历当前部件的所有链接特征，获取每个特征的源部件，然后遍历所有打开的部件，如果当前部件既包含源部件又包含目标部件，则报告它们的装配路径。
> 9. NX启动函数ufusr调用初始化、do_it、终止化。
> 10. 提供了一个卸载函数ufusr_ask_unload。
>
> 总体来说，该代码用于遍历当前部件的链接特征，获取每个特征的源部件，然后报告所有包含源部件和目标部件的上下文装配路径。
>
