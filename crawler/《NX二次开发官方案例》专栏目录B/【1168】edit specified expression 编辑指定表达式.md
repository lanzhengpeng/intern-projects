### 【1168】edit specified expression 编辑指定表达式

#### 代码

```cpp
    /*HEAD EDIT_SPECIFIED_EXPRESSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
注意：UF_print_syslog在V18中是新增的，请只回答翻译，不要添加其他内容。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void report_feature_name_and_type(char *title, tag_t feat)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &name)))  
        {  
            if (!UF_CALL(UF_MODL_ask_feat_type(feat, &type)))  
            {  
                UF_CALL(UF_UI_open_listing_window());  
                sprintf(msg, "%s = %d %s (%s)\n", title, feat, name, type);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_free(type);  
            }  
            UF_free(name);  
        }  
    }  
    static int ask_number_of_suppressed_features(void)  
    {  
        int  
            count;  
        uf_list_p_t  
            feature_list;  
        UF_CALL(UF_MODL_ask_suppress_list(&feature_list));  
        UF_CALL(UF_MODL_ask_list_count(feature_list, &count));  
        UF_CALL(UF_MODL_delete_list(&feature_list));  
        return (count);  
    }  
    static tag_t ask_next_solid_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SOLID_BODY) return body;  
            }  
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
    static int ask_all_solids(tag_t part, tag_t **solids)  
    {  
        tag_t  
            solid = NULL_TAG;  
        uf_list_p_t  
            solid_list;  
        UF_CALL(UF_MODL_create_list(&solid_list));  
        while ((solid = ask_next_solid_body(part, solid)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(solid_list, solid));  
        return (make_an_array(&solid_list, solids));  
    }  
    static double ask_total_solid_volume(void)  
    {  
        int  
            n_solids,  
            units;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *solids;  
        double  
            acc[11] = { 0.99, 0,0,0,0,0,0,0,0,0,0 },  
            mass_props[47],  
            stats[13];  
        UF_CALL(UF_PART_ask_units(part, &units));  
        if (units == UF_PART_METRIC)  
            units = UF_MODL_grams_centimeters;  
        else  
            units = UF_MODL_pounds_inches;  
        if (n_solids = ask_all_solids(part, &solids))  
        {  
            UF_CALL(UF_MODL_ask_mass_props_3d(solids, n_solids, 1, units, 1, 1,  
                acc, mass_props, stats));  
            UF_free(solids);  
            return mass_props[1];  
        }  
        else  
            return 0;  
    }  
    static void do_it(void)  
    {  
        int  
            ifail,  
            n_feat;  
        tag_t  
            feat,  
            *feats;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1] = { "" };  
        double  
            old_volume,  
            new_volume;  
        while (prompt_for_text("Enter new expression (e.g. p1=2)", exp_str))  
        {  
            old_volume = ask_total_solid_volume();  
            UF_CALL(UF_MODL_edit_exp(exp_str));  
            UF_CALL(UF_MODL_set_update_fail_option(UF_MODL_UPDATE_ACCEPT_ALL));  
            UF_CALL(UF_MODL_update());  
            UF_CALL(UF_MODL_ask_out_of_date_features(&n_feat, &feats));  
            WRITENZ(n_feat);  
            if (n_feat > 0) UF_free(feats);  
            UF_CALL(UF_MODL_edit_exp(exp_str));  
            UF_CALL(UF_MODL_set_update_fail_option(UF_MODL_UPDATE_UNDO));  
            UF_CALL(UF_MODL_update());  
            UF_CALL(UF_MODL_ask_update_undo_feat(&feat, &ifail));  
            if (feat != NULL_TAG)  
            {  
                report_feature_name_and_type("failure feature", feat);  
                UF_CALL(ifail);  
            }  
            UF_CALL(UF_MODL_edit_exp(exp_str));  
            UF_CALL(UF_MODL_set_update_fail_option(UF_MODL_UPDATE_SUPPRESS_ALL));  
            UF_CALL(UF_MODL_update());  
            n_feat = ask_number_of_suppressed_features();  
            WRITENZ(n_feat);  
            new_volume = ask_total_solid_volume();  
            if (old_volume == new_volume) uc1601("Model did not change", TRUE);  
        }  
        UF_CALL(UF_MODL_set_update_fail_option(UF_MODL_UPDATE_NO_OPTION));  
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

> 这段代码是一个NX的二次开发示例，主要功能是编辑指定的表达式，并更新模型。以下是代码的主要功能和流程：
>
> 1. 定义了错误报告函数report_error，用于输出函数执行错误信息。
> 2. 定义了整数写入列表窗口函数write_integer_to_listing_window，用于输出整数值。
> 3. 定义了提示输入文本函数prompt_for_text，用于提示用户输入表达式。
> 4. 定义了报告特征名称和类型函数report_feature_name_and_type，用于输出特征的名称和类型。
> 5. 定义了查询被抑制特征数量函数ask_number_of_suppressed_features，用于查询被抑制的特征数量。
> 6. 定义了查询下一个实体函数ask_next_solid_body，用于查询模型中的下一个实体。
> 7. 定义了内存分配函数allocate_memory，用于分配内存。
> 8. 定义了列表转换为数组函数make_an_array，用于将列表转换为数组。
> 9. 定义了查询所有实体函数ask_all_solids，用于查询模型中的所有实体。
> 10. 定义了查询实体体积函数ask_total_solid_volume，用于查询模型中所有实体的总体积。
> 11. 定义了主要执行函数do_it，用于编辑表达式、更新模型，并输出更新后模型的体积变化。
> 12. 定义了ufusr函数，用于初始化NX、执行do_it函数、并终止NX。
> 13. 定义了卸载函数ufusr_ask_unload，用于立即卸载用户程序。
>
> 整体而言，该代码通过交互式提示用户输入表达式，然后更新模型，并输出更新后的模型体积变化，从而实现编辑表达式并更新模型的功能。
>
