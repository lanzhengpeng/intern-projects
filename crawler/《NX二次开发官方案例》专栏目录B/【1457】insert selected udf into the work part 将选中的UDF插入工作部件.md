### 【1457】insert selected udf into the work part 将选中的UDF插入工作部件

#### 代码

```cpp
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
            *choice = resp - 4;  /* returns 1 or 2 里海译:returns 1 or 2的翻译为“返回1或2”。 */  
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
    /*  NOTE:  This will free the input curves array automatically 里海译:释放输入曲线数组。 */  
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
    /*  NOTE:  This will free the input edges array automatically 里海译:This will free the input edges array automatically。 */  
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
            *lhs,  
            **parents_prompt = NULL,  
            *rhs,  
            *string,  
            udf_part[MAX_FSPEC_SIZE+1] = { "" };  
        UF_PART_load_status_t  
            error_status;  
        if (!prompt_for_any_existing_part_name("UDF part", udf_part)) return;  
        set_undo_mark("Insert UDF");  
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
            (void ***)&exp_rhs));  
        for (ii = 0; ii < num_expression; ii++)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_string(expression[ii], &string));  
            UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs, &exp));  
            strcpy(exp_rhs[ii], rhs);  
            UF_free(lhs);  
            UF_free(rhs);  
            UF_free(string);  
            prompt_for_text(expression_prompt[ii], exp_rhs[ii]);  
        }  
        UF_CALL(UF_MODL_register_rpo_routine(UF_MODL_udf_rpo_menu));  
        UF_CALL(UF_MODL_create_instantiated_udf(feature, NULL, parents,  
            new_parents, num_parents, expression, exp_rhs, num_expression,  
            &new_udf_feature));  
        UF_CALL(UF_MODL_unregister_rpo_routine());  
        UF_free(parents);  
        UF_free(new_parents);  
        UF_free_string_array(num_parents, parents_prompt) ;  
        UF_free(expression);  
        UF_free_string_array(num_expression, expression_prompt);  
        UF_free_string_array(num_expression, exp_rhs);  
        UF_CALL(UF_PART_close(part, 0, 1));  
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
    /*  Note:  can NOT be unloaded - calls UF_MODL_register_rpo_routine. 里海译:根据上下文，这句话的意思是：

不能卸载 - 调用了UF_MODL_register_rpo_routine函数。 */  
        return (UF_UNLOAD_UG_TERMINATE);  
    }

```

#### 代码解析

> 这是一段用于NX二次开发的代码，其主要功能是创建一个新的UDF(用户定义特征)特征。具体来说：
>
> 1. 功能介绍：代码首先会提示用户选择一个现有的NX部件文件，该文件中包含一个UDF定义特征。
> 2. 打开部件：代码会打开用户选择的NX部件，并找出该部件中的UDF定义特征。
> 3. 获取参数：代码会获取UDF定义中的父特征、表达式等参数信息。
> 4. 用户交互：用户需要选择或输入新的父特征，以满足UDF定义中的要求。
> 5. 创建UDF：最后，代码会使用用户提供的参数，在当前工作部件中创建一个新的UDF实例特征。
> 6. 错误处理：代码中包含错误处理，能够输出错误信息，并设置undo标记。
> 7. 函数说明：主要函数包括do_it()用于执行创建UDF的流程，以及ufusr()作为NX二次开发的入口函数。
> 8. 注释：代码中的注释详细介绍了每个步骤的实现逻辑。
> 9. 调用库函数：代码调用了NX提供的UF库函数，如UF_MODL、UF_UI、UF_OBJ等。
> 10. 注意事项：代码中使用了register_rpo_routine来注册一个回调函数，这意味着代码不能被卸载。
>
> 总体来说，这段代码实现了UDF特征的创建，并提供了良好的用户交互体验。
>
