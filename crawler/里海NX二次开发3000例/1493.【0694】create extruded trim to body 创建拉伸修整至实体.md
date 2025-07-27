### 【0694】create extruded trim to body 创建拉伸修整至实体

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_vec.h>  
    #include <uf_sc.h>  
    #include <uf_so.h>  
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
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
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
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        objs = *curves;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    /* 里海 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void ask_zc_dir(double zc_dir[3])  
    {  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        double  
            origin[3],  
            wcs[9];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, wcs));  
        UF_VEC3_copy(&wcs[6], zc_dir);  
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
        sec_rule.rule_types = (int *)UF_allocate_memory(sizeof(int), &resp);  
        sec_rule.rules =  
            (UF_SC_input_data_t *)UF_allocate_memory(sizeof(UF_SC_input_data_t), &resp);  
        UF_MODL_init_sc_input_data(UF_SC_CURVE_DUMB_CURVES, &sec_rule.rules[0]);  
        sec_rule.rule_types[0] = UF_SC_CURVE_DUMB_CURVES;  
        sec_rule.rules[0].curve_dumb_input.num_curves = n_curves;  
        sec_rule.rules[0].curve_dumb_input.curves = curves;  
        UF_CALL(UF_MODL_create_section(curves[0], 1, &sec_rule, NULL_TAG, NULL,  
            NULL, FALSE, &section));  
    /*  NOTE:  This will free the input curves array automatically 译:释放输入曲线数组。 */  
        UF_CALL(UF_MODL_free_section_data(&sec_rule));  
        return section;  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves,  
            n_feats;  
        tag_t  
            *curves,  
            body,  
            *feats;  
        double  
            dir[3];  
        UF_MODL_mswp_extrude_t  
            extrude;  
        if (((n_curves = select_curves("Select section curves", &curves)) > 0) &&  
            ((body = select_a_body("Select trimming body")) != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_mswp_init_extrude_data(&extrude));  
        /*  NOTE:  This UF_free's the curves array 译:翻译：此处的UF_free是释放曲线数组内存的函数。

答案：此处的UF_free函数用于释放曲线数组占用的内存。 */  
            extrude.section = create_dumb_curve_section(n_curves, curves);  
            ask_zc_dir(dir);  
            UF_CALL(UF_SO_create_dirr_doubles(body, UF_SO_update_within_modeling,  
                dir, &extrude.direction));  
            extrude.limits.end_limit.limit_type = UF_MODL_MSWP_LIMIT_UNTIL_SELECTED;  
            extrude.limits.end_limit.limit_data.until_selected_data.selected_object  
                = body;  
            extrude.sign = UF_POSITIVE;  
            extrude.target_body = body;  
            if (!UF_CALL(UF_MODL_mswp_create_extrude(&extrude, &n_feats, &feats))  
                && (n_feats > 0)) UF_free(feats);  
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

> 根据代码，这是一段NX二次开发代码，主要实现了以下功能：
>
> 1. 选择截面曲线和修剪体，并创建截面。
> 2. 设置拉伸方向和拉伸直至选择的面。
> 3. 创建拉伸特征，并释放相关内存。
> 4. 提供了错误报告机制。
> 5. 实现了UFusr接口，完成初始化、执行主体功能、终止等流程。
> 6. 实现了ufusr_ask_unload接口，以便在卸载时可以立即卸载。
>
> 总体来说，这段代码实现了NX拉伸特征的创建，并提供了友好的用户交互。
>
