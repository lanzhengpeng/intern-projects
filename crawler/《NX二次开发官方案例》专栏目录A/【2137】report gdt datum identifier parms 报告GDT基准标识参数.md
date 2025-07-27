### 【2137】report gdt datum identifier parms 报告GDT基准标识参数

#### 代码

```cpp
    /*HEAD REPORT_GDT_DATUM_IDENTIFIER_PARMS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_gdt.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
注意：UF_print_syslog是V18版本中新增的，请只提供翻译，不要添加任何废话。 */  
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
    /* qq3123197280 */  
    static int mask_gdt(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            triples = { UF_tol_feature_instance_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &triples)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_gdt(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a GDT", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_gdt, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_P(X) (write_pointer_to_listing_window(#X, X))  
    static void write_pointer_to_listing_window(char *title, void *pointer)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (pointer == NULL)  
            sprintf(msg, "%s = NULL\n", title);  
        else  
            sprintf(msg, "%s != NULL\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_gdt_modifier_data(UF_GDT_modifier_data_p_t modifier_data)  
    {  
        logical  
            is_user_keyword;  
        int  
            ii;  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        char  
            *keyword_name,  
            *keyword_text;  
        if (modifier_data == NULL) return;  
        WRITE_D(modifier_data->num_modifiers);  
        WRITEnD(modifier_data->num_modifiers,  
            (int*)modifier_data->modifier_types);  
        WRITE_P(modifier_data->proj_zone);  
        if (modifier_data->proj_zone != NULL)  
        {  
            WRITE_D(modifier_data->proj_zone->expression);  
            WRITE_F(modifier_data->proj_zone->value);  
            WRITE_D(modifier_data->proj_zone->decimal_format);  
            WRITE_D(modifier_data->proj_zone->direction);  
        }  
        WRITE_P(modifier_data->unit_length);  
        if (modifier_data->unit_length != NULL)  
        {  
            WRITE_D(modifier_data->unit_length->tolerance_expression);  
            WRITE_D(modifier_data->unit_length->length_expression);  
            WRITE_D(modifier_data->unit_length->width_expression);  
            WRITE_D(modifier_data->unit_length->decimal_format);  
            WRITE_D(modifier_data->unit_length->ub_decimal_format);  
        }  
        WRITE_P(modifier_data->unit_area);  
        if (modifier_data->unit_area != NULL)  
        {  
            WRITE_D(modifier_data->unit_area->tolerance_expression);  
            WRITE_D(modifier_data->unit_area->length_expression);  
            WRITE_D(modifier_data->unit_area->width_expression);  
            WRITE_D(modifier_data->unit_area->decimal_format);  
            WRITE_D(modifier_data->unit_area->ub_decimal_format);  
        }  
        WRITE_P(modifier_data->bonus_tol);  
        if (modifier_data->bonus_tol != NULL)  
        {  
            WRITE_D(modifier_data->bonus_tol->expression);  
            WRITE_F(modifier_data->bonus_tol->value);  
            WRITE_D(modifier_data->bonus_tol->decimal_format);  
        }  
        WRITE_D(modifier_data->num_keywords);  
        for (ii = 0; ii < modifier_data->num_keywords; ii++)  
        {  
            WRITE_D(modifier_data->keywords[ii].id);  
            UF_CALL(UF_GDT_is_user_defined_keyword(part,  
                modifier_data->keywords[ii].id, &is_user_keyword));  
            WRITE_L(is_user_keyword);  
            UF_CALL(UF_GDT_ask_keyword_name(part, modifier_data->keywords[ii].id,  
                &keyword_name));  
            WRITE_S(keyword_name);  
            UF_CALL(UF_GDT_ask_keyword_text(part, keyword_name, &keyword_text));  
            WRITE_S(keyword_text);  
            UF_free(keyword_text);  
            UF_free(keyword_name);  
        }  
    }  
    static void report_gdt_feature_parms(tag_t tolerance_feature)  
    {  
        int  
            ii;  
        UF_GDT_feature_parms_p_t  
            feat_parms;  
        if (UF_CALL(UF_GDT_ask_feature_parms(tolerance_feature, &feat_parms)))  
            return;  
        WRITE_D(feat_parms->num_faces);  
        WRITEnD(feat_parms->num_faces, (int *)feat_parms->face_list);  
        WRITE_S(feat_parms->descript->name);  
        WRITE_S(feat_parms->descript->description);  
        WRITE_D(feat_parms->feature);  
        WRITE_D(feat_parms->pattern);  
        WRITE_P(feat_parms->modifier_data);  
        report_gdt_modifier_data(feat_parms->modifier_data);  
        WRITE_D(feat_parms->num_sets);  
        for (ii = 0; ii < feat_parms->num_sets; ii++)  
        {  
            WRITE_D(feat_parms->sets[ii]->num_faces);  
            WRITEnD(feat_parms->sets[ii]->num_faces,  
                (int *)feat_parms->sets[ii]->faces);  
        }  
        WRITE_D(feat_parms->def.type);  
        WRITE_D(feat_parms->def.so_tag);  
        WRITE_F(feat_parms->def.radius);  
        WRITE_D(feat_parms->def.trimmed_sheet);  
        UF_CALL(UF_GDT_free(UF_GDT_FEAT_PARMS_TYPE, feat_parms));  
    }  
    static void report_gdt_datum_identifier_parms(tag_t datum_feat)  
    {  
        UF_GDT_datum_identifier_p_t  
            datum_data;  
        if (UF_CALL(UF_GDT_ask_datum_ident_parms(datum_feat, &datum_data)))  
            return;  
        WRITE_S(datum_data->datum_label);  
        WRITE_L(datum_data->is_individual);  
        UF_CALL(UF_GDT_free(UF_GDT_DATUM_IDENT_TYPE, datum_data));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            datum_feat,  
            gdt;  
        while ((gdt = select_a_gdt("Report Datum Indentifier Parms")) != NULL_TAG)  
        {  
            UF_CALL(UF_GDT_ask_tol_feat_of_instance(gdt, &datum_feat));  
            WRITE_D(datum_feat);  
            report_gdt_feature_parms(datum_feat);  
            report_gdt_datum_identifier_parms(datum_feat);  
        }  
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

> 这段NX Open API代码用于报告NX模型中公差特征实例的基准标识参数。
>
> 主要功能包括：
>
> 1. 选择公差特征实例：使用UF_UI_select_with_single_dialog函数，通过对话框让用户选择一个公差特征实例。
> 2. 查询基准标识参数：使用UF_GDT_ask_tol_feat_of_instance函数获取公差特征实例关联的基准特征，然后使用UF_GDT_ask_datum_ident_parms函数查询基准标识参数。
> 3. 报告基准标识参数：将基准标识参数输出到日志窗口，包括基准标签、是否为单独标识等信息。
> 4. 报告公差特征参数：使用UF_GDT_ask_feature_parms函数查询公差特征参数，包括面列表、描述信息、公差值等信息，并将其输出到日志窗口。
> 5. 错误处理：使用宏定义UF_CALL来包裹NX API调用，通过错误处理函数report_error来报告错误。
> 6. 辅助函数：定义了多个辅助函数，用于输出不同类型的数据到日志窗口。
> 7. 主函数：通过调用do_it函数来执行选择公差特征实例、查询和报告参数的逻辑。
> 8. 启动和卸载：定义了ufusr和ufusr_ask_unload函数来启动和卸载用户程序。
>
> 总体来说，这段代码实现了查询和报告NX模型中公差特征实例的基准标识参数的功能。
>
