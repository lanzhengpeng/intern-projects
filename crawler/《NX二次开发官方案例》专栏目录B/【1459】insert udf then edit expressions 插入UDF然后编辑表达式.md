### 【1459】insert udf then edit expressions 插入UDF然后编辑表达式

#### 代码

```cpp
    /*  
    This example demonstrates bringing the UDF another way which may help to avoid the problem.    
    It brings in the UDF using the newly specified parent objects, but leaves the expressions alone.    
    Then it edits any expressions for which the user has specified new values and performs an update.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_sc.h>  
    #include <uf_undo.h>  
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
    static tag_t ask_next_feature_of_type(tag_t part, char *type, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (!strcmp(this_type, type))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    static void report_load_status(UF_PART_load_status_t *status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 里海译:returns 1 or 2 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_part_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir,  
            *p_ext;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_PART_OPEN_FLTR, &p_ext));  
        combine_directory_and_wildcard(p_dir, p_ext, filter);  
        UF_free(p_dir);  
        UF_free(p_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Part Name", filter, "", fspec,  
            &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static logical prompt_for_any_existing_part_name(char *prompt, char *fspec)  
    {  
        logical  
            nxman,  
            unused;  
        int  
            mode = 1,  
            resp;  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
        {  
            if (pick_one_of_two_choices(prompt, "in Native", "in TcEng",  
                &mode))  
            {  
                if (mode == 1)  
                    return (prompt_for_part_name(prompt, fspec));  
                else  
                {  
                    UF_CALL(UF_UI_set_prompt(prompt));  
                    if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
                        || (resp == UF_UI_CANCEL)) return FALSE;  
                    return TRUE;  
                }  
            }  
            else return FALSE;  
        }  
        else  
        {  
            UF_CALL(UF_UI_set_prompt(prompt));  
            if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
                || (resp == UF_UI_CANCEL)) return FALSE;  
            return TRUE;  
        }  
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
    static int allocate_string_array(int n_elements, int n_chars, void ***where)  
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
    /* qq3123197280 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    static tag_t create_dumb_curve_section(int n_curves, tag_t *curves)  
    {  
        int  
            resp;  
        tag_t  
            section;  
        UF_SC_section_data_t  
            sec_rule;  
        UF_MODL_initialize_section_data(&sec_rule);  
        sec_rule.n_rules = 1;  
        sec_rule.rule_types = UF_allocate_memory(sizeof(int), &resp);  
        sec_rule.rules = UF_allocate_memory(sizeof(UF_SC_input_data_t), &resp);  
        UF_MODL_init_sc_input_data(UF_SC_CURVE_DUMB_CURVES, &sec_rule.rules[0]);  
        sec_rule.rule_types[0] = UF_SC_CURVE_DUMB_CURVES;  
        sec_rule.rules[0].curve_dumb_input.num_curves = n_curves;  
        sec_rule.rules[0].curve_dumb_input.curves = curves;  
        UF_CALL(UF_MODL_create_section(curves[0], 1, &sec_rule, NULL_TAG, NULL,  
            NULL, FALSE, &section));  
    /*  NOTE:  This will free the input curves array automatically 里海译:这个将会自动释放输入曲线数组。 */  
        UF_CALL(UF_MODL_free_section_data(&sec_rule));  
        return section;  
    }  
    /* qq3123197280 */  
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_edges(char *prompt, tag_t **edges)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select edges", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_edges, NULL, &resp, &cnt, edges));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*edges)[ii], FALSE));  
        return cnt;  
    }  
    static tag_t create_dumb_edge_section(int n_edges, tag_t *edges)  
    {  
        int  
            resp;  
        tag_t  
            section;  
        UF_SC_section_data_t  
            sec_rule;  
        UF_MODL_initialize_section_data(&sec_rule);  
        sec_rule.n_rules = 1;  
        sec_rule.rule_types = UF_allocate_memory(sizeof(int), &resp);  
        sec_rule.rules = UF_allocate_memory(sizeof(UF_SC_input_data_t), &resp);  
        UF_MODL_init_sc_input_data(UF_SC_EDGE_DUMB_EDGES, &sec_rule.rules[0]);  
        sec_rule.rule_types[0] = UF_SC_EDGE_DUMB_EDGES;  
        sec_rule.rules[0].edge_dumb_input.num_edges = n_edges;  
        sec_rule.rules[0].edge_dumb_input.edges = edges;  
        UF_CALL(UF_MODL_create_section(edges[0], 1, &sec_rule, NULL_TAG, NULL,  
            NULL, FALSE, &section));  
    /*  NOTE:  This will free the input edges array automatically 里海译:这将会自动释放输入边数组 */  
        UF_CALL(UF_MODL_free_section_data(&sec_rule));  
        return section;  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            error,  
            ii,  
            n_curves,  
            num_parents,  
            num_expression,  
            resp,  
            subtype,  
            type,  
            which = 2;  
        tag_t  
            *expression = NULL,  
            exp,  
            feature = NULL_TAG,  
            *new_parents = NULL,  
            new_udf_feature,  
            *parents = NULL,  
            part,  
            prev_work_part,  
            *sc_curves;  
        char  
            **exp_rhs,  
            **expression_prompt = NULL,  
            **exp_lhs,  
            **old_rhs,  
            new_expression[UF_MAX_EXP_LENGTH+1],  
            **parents_prompt = NULL,  
            *string,  
            udf_part[MAX_FSPEC_SIZE+1] = { "" };  
        UF_PART_load_status_t  
            error_status;  
        if (!prompt_for_any_existing_part_name("UDF part", udf_part)) return;  
        UF_CALL(UF_PART_open_quiet(udf_part, &part, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
        UF_CALL(UF_ASSEM_set_work_part_quietly(part, &prev_work_part));  
        feature = ask_next_feature_of_type(part, "UDF_DEF", feature);  
        UF_CALL(UF_MODL_ask_udf_definition(feature, &parents, &parents_prompt,  
            &num_parents, &expression, &expression_prompt, &num_expression));  
        UF_CALL(UF_ASSEM_set_work_part_quietly(prev_work_part, &part));  
        new_parents = (tag_t *)UF_allocate_memory(sizeof(tag_t)*num_parents,  
            &error);  
        for (ii = 0; ii < num_parents; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(parents[ii], &type, &subtype));  
            if (type == UF_smart_container_type)  
            {  
                pick_one_of_two_choices("Section from", "Curves", "Edges", &which);  
                if (which == 1)  
                {  
                    n_curves = select_curves(parents_prompt[ii], &sc_curves);  
                    new_parents[ii] = create_dumb_curve_section(n_curves, sc_curves);  
                }  
                else  
                {  
                    n_curves = select_edges(parents_prompt[ii], &sc_curves);  
                    new_parents[ii] = create_dumb_edge_section(n_curves, sc_curves);  
                }  
            }  
            else  
                new_parents[ii] = select_anything(parents_prompt[ii]);  
        }  
        UF_CALL(allocate_string_array(num_expression, UF_MAX_EXP_LENGTH,  
            (void ***)&exp_lhs));  
        UF_CALL(allocate_string_array(num_expression, UF_MAX_EXP_LENGTH,  
            (void ***)&exp_rhs));  
        UF_CALL(allocate_string_array(num_expression, UF_MAX_EXP_LENGTH,  
            (void ***)&old_rhs));  
        for (ii = 0; ii < num_expression; ii++)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_string(expression[ii], &string));  
            UF_CALL(UF_MODL_dissect_exp_string(string, &(exp_lhs[ii]), &(old_rhs[ii]), &exp));  
            strcpy(exp_rhs[ii], old_rhs[ii]);  
            UF_free(string);  
            prompt_for_text(expression_prompt[ii], exp_rhs[ii]);  
        }  
        set_undo_mark("Insert UDF");  
        UF_CALL(UF_MODL_create_instantiated_udf(feature, NULL, parents,  
            new_parents, num_parents, NULL, NULL, 0, &new_udf_feature));  
        UF_CALL(UF_MODL_ask_time_stamp_of_feature(new_udf_feature, &resp));  
        for (ii = 0; ii < num_expression; ii++)  
        {  
            if (strcmp(old_rhs[ii], exp_rhs[ii]))  
            {  
                sprintf(new_expression, "%s_%d=%s", exp_lhs[ii], resp, exp_rhs[ii]);  
                UF_CALL(UF_MODL_edit_exp(new_expression));  
            }  
        }  
        UF_CALL(UF_MODL_update());  
        UF_free(parents);  
        UF_free(new_parents);  
        UF_free_string_array(num_parents, parents_prompt) ;  
        UF_free(expression);  
        UF_free_string_array(num_expression, expression_prompt);  
        UF_free_string_array(num_expression, exp_rhs);  
        UF_free_string_array(num_expression, exp_lhs);  
        UF_free_string_array(num_expression, old_rhs);  
    //  if you close the UDF part, you cannot UNDO since closing any part deletes  
    //  all UNDO marks.  
    //  UF_CALL(UF_PART_close(part, 0, 1));  
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

> 这是段NX二次开发代码，主要用于实现用户自定义特征(UDF)的重新实例化。以下是代码的主要功能和流程：
>
> 1. 提示用户选择一个包含UDF定义的部件。
> 2. 打开用户选择的部件，并找到第一个类型为"UDF_DEF"的特征，即UDF定义。
> 3. 获取该UDF定义的父特征列表和表达式列表。
> 4. 重新选择或创建UDF的父特征。
> 5. 提示用户编辑每个表达式，并收集新的表达式值。
> 6. 创建UDF实例，使用新的父特征和表达式。
> 7. 更新模型。
> 8. 提示用户打开部件时，可以选择在本地或Teamcenter Engineering中打开。
> 9. 使用UF_UNDO_set_mark记录UNDO标记点，以便后续撤销操作。
> 10. 调用UF_MODL_create_instantiated_udf创建UDF实例，并调用UF_MODL_update更新模型。
> 11. 最后释放所有分配的内存。
>
> 综上所述，该代码实现了UDF的重新实例化，通过重新选择父特征和编辑表达式，实现了对UDF的修改。
>
