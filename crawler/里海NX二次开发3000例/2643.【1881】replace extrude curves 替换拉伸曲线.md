### 【1881】replace extrude curves 替换拉伸曲线

#### 代码

```cpp
    /*HEAD REPLACE_EXTRUDE_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_sc.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您提供的信息，UF_print_syslog是V18版本中新增的功能。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
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
    static void build_dumb_curve_section_rule(int n_curves, tag_t *curves,  
        UF_SC_section_data_t *sec_rule)  
    {  
        int  
            resp;  
        UF_MODL_initialize_section_data(sec_rule);  
        sec_rule->n_rules = 1;  
        sec_rule->rule_types = UF_allocate_memory(sizeof(int), &resp);  
        sec_rule->rules = UF_allocate_memory(sizeof(UF_SC_input_data_t), &resp);  
        UF_MODL_init_sc_input_data(UF_SC_CURVE_DUMB_CURVES, &sec_rule->rules[0]);  
        sec_rule->rule_types[0] = UF_SC_CURVE_DUMB_CURVES;  
        sec_rule->rules[0].curve_dumb_input.num_curves = n_curves;  
        sec_rule->rules[0].curve_dumb_input.curves = curves;  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves,  
            n_rules;  
        tag_t  
            feat,  
            *curves,  
            start;  
        double  
            dir[3],  
            sp[3];  
        UF_MODL_mswp_extrude_t  
            extrude;  
        UF_SC_section_data_t  
            new_rules,  
            *old_rules;  
        if (((feat = select_a_feature("Edit extrude")) == NULL_TAG) ||  
            ((n_curves = select_curves("Select new section curves", &curves)) == 0) ||  
            UF_CALL(UF_MODL_mswp_ask_extrude(feat, &extrude))) return;  
        UF_CALL(UF_MODL_ask_section(extrude.section, &n_rules, &old_rules, &start, sp, dir));  
        build_dumb_curve_section_rule(n_curves, curves, &new_rules);  
        UF_CALL(UF_MODL_add_rules_to_section(1, &new_rules, extrude.section));  
        UF_CALL(UF_MODL_free_section_data(&new_rules));  
        UF_CALL(UF_MODL_remove_rules_from_section(n_rules, old_rules, extrude.section));  
        UF_CALL(UF_MODL_free_section_data(old_rules));  
    /*  This will UNDO if EXTRUDE has children - see PR 1585343 里海译:如果EXTRUDE有子项，这将撤销 - 见PR 1585343。 */  
        UF_CALL(UF_MODL_update());  
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

> 这段代码是用于NX的二次开发，实现了编辑拉伸特征的功能。
>
> 主要功能包括：
>
> 1. 选择一个拉伸特征。
> 2. 选择新的截面曲线。
> 3. 获取当前拉伸的截面规则，并构建新的截面规则。
> 4. 将新规则添加到拉伸特征中。
> 5. 删除旧的截面规则。
> 6. 更新模型以反映修改。
>
> 其中，代码使用了UF库的函数来选择特征、曲线，构建和操作拉伸的截面规则。错误处理采用宏UF_CALL来实现。
>
> 总体来说，这段代码实现了编辑拉伸特征截面的功能，具有一定的参考价值。
>
