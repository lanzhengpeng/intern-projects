### 【0079】add selected deformable part to assembly 在装配中添加选定的可变形零件

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <uf_sc.h>  
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
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
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
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
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
    /* 里海 */  
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
    /*  NOTE:  This will free the input curves array automatically 译:这将会自动释放输入曲线数组。 */  
        UF_CALL(UF_MODL_free_section_data(&sec_rule));  
        return section;  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n_curves,  
            type,  
            subtype;  
        tag_t  
            exp,  
            dfeat,  
            dpart,  
            dpart_occ,  
            instance,  
            *new_parents = NULL,  
            part = UF_PART_ask_display_part(),  
            *sc_curves;  
        double  
            origin[3],  
            matrix[9];  
        char  
            fspec[MAX_FSPEC_SIZE+1] = { "" },  
            *lhs,  
            **new_expressions,  
            *rhs,  
            *string;  
        UF_PART_load_status_t  
            error_status;  
        UF_ASSEM_deformed_definition_data_t  
            dfdata;  
        UF_ASSEM_deform_part_data_t  
            dpdata;  
        ask_wcs_info(origin, matrix);  
        while (prompt_for_existing_part_name("Deformable Component", fspec))  
            while (specify_point("Indicate origin", origin))  
            {  
                set_undo_mark("Add Deformable Component");  
                UF_CALL(UF_ASSEM_add_part_to_assembly(part, fspec, NULL, NULL,  
                    origin, matrix, 0, &instance, &error_status));  
                if (error_status.n_parts > 0) report_load_status(&error_status);  
                if (error_status.failed) break;  
                dpart_occ = UF_ASSEM_ask_part_occ_of_inst(NULL_TAG, instance);  
                dpart = UF_ASSEM_ask_prototype_of_occ(dpart_occ);  
                if (!UF_ASSEM_is_part_deformable(dpart)) break;  
            //  If the part was already partially loaded in the session, adding a  
            //  new occurrence will not change that, so make sure its fully loaded.  
            //  See PR 6626157.  
                ensure_part_fully_loaded(dpart);  
                UF_CALL(UF_ASSEM_init_deform_part_data(&dpdata));  
                dpdata.part_occ_to_deform = dpart_occ;  
                UF_CALL(UF_ASSEM_ask_deformable_definition(dpart, &dfeat));  
                UF_CALL(UF_ASSEM_ask_deformed_definition_data(dfeat, &dfdata));  
                dpdata.num_parents = dfdata.num_parents;  
                if (dfdata.num_parents > 0)  
                {  
                    dpdata.old_parents = dfdata.parents;  
                    UF_CALL(allocate_memory(dpdata.num_parents * sizeof(tag_t),  
                        (void **)&new_parents));  
                    dpdata.new_parents = new_parents;  
                    for (ii = 0; ii < dpdata.num_parents; ii++)  
                    {  
                        UF_CALL(UF_OBJ_ask_type_and_subtype(dpdata.old_parents[ii],  
                            &type, &subtype));  
                        if (type == UF_smart_container_type)  
                        {  
                            n_curves = select_curves(dfdata.parent_prompts[ii],  
                                &sc_curves);  
                            dpdata.new_parents[ii] =  
                                create_dumb_curve_section(n_curves, sc_curves);  
                        }  
                        else  
                            dpdata.new_parents[ii] =  
                                select_an_object(dfdata.parent_prompts[ii]);  
                    }  
                }  
                dpdata.num_expressions = dfdata.num_expressions;  
                if (dfdata.num_expressions > 0)  
                {  
                    dpdata.old_expressions = dfdata.expressions;  
                    UF_CALL(allocate_string_array(dpdata.num_expressions,  
                        UF_MAX_EXP_LENGTH, (char ***)&new_expressions));  
                    dpdata.new_expression_values = (const char **)new_expressions;  
                    for (ii = 0; ii < dfdata.num_expressions; ii++)  
                    {  
                        UF_CALL(UF_MODL_ask_exp_tag_string(dfdata.expressions[ii],  
                            &string));  
                        UF_CALL(UF_MODL_dissect_exp_string(string,&lhs,&rhs,&exp));  
                        strcpy(new_expressions[ii], rhs);  
                        prompt_for_text(dfdata.expression_prompts[ii],  
                            new_expressions[ii]);  
                        UF_free(lhs);  
                        UF_free(rhs);  
                        UF_free(string);  
                    }  
                }  
                UF_CALL(UF_ASSEM_deform_part(&dpdata, NULL));  
                if (dfdata.num_parents > 0) UF_free(dpdata.new_parents);  
                if (dfdata.num_expressions > 0)  
                    UF_free_string_array(dpdata.num_expressions, new_expressions);  
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

> 根据代码内容，这是一段基于NX的二次开发代码，主要功能是向装配中添加可变形部件。以下是对代码的详细介绍：
>
> 1. 头文件包含：代码包含了NX的二次开发库头文件，提供了UF模块的接口。
> 2. 错误报告函数：定义了ECHO和report_error函数，用于打印错误信息。
> 3. 用户输入函数：定义了多个用于提示用户输入的函数，如prompt_for_existing_part_name、specify_point等。
> 4. 获取信息函数：定义了ask_wcs_info函数，用于获取当前工作坐标系信息。
> 5. 选择对象函数：定义了select_an_object和select_curves函数，用于选择曲线等对象。
> 6. 创建截面函数：定义了create_dumb_curve_section函数，用于根据选择的曲线创建截面。
> 7. 设置撤销标记函数：定义了set_undo_mark函数，用于设置撤销标记点。
> 8. 确保部件完全加载函数：定义了ensure_part_fully_loaded函数，用于确保部件完全加载。
> 9. 主体函数：定义了do_it函数，实现了向装配添加可变形部件的主要逻辑。
> 10. ufusr入口函数：定义了ufusr函数，作为二次开发的入口点，调用do_it函数执行主体逻辑。
> 11. 卸载函数：定义了ufusr_ask_unload函数，用于卸载二次开发应用程序。
>
> 综上所述，这段代码实现了向NX装配中添加可变形部件的功能，包括用户交互、信息获取、对象选择、撤销支持等功能。
>
