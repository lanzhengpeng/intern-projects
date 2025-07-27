### 【2737】set body density 设置实体密度

#### 代码

```cpp
    /*HEAD SET_BODY_DENSITY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中内容，UF_print_syslog 是 V18 中的新增功能。 */  
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
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select solid", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
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
    static void do_it(void)  
    {  
        logical  
            is_occ;  
        int  
            units;  
        tag_t  
            body,  
            owning_part,  
            part = UF_PART_ask_display_part();  
        double  
            density;  
        while ((body = select_a_solid("Edit solid density")) != NULL_TAG)  
        {  
            is_occ = (UF_ASSEM_is_occurrence(body) &&  
                        !UF_OBJ_is_object_a_promotion(body));  
            if (is_occ)  
            {  
                body = UF_ASSEM_ask_prototype_of_occ(body);  
                UF_CALL(UF_OBJ_ask_owning_part(body, &owning_part));  
                ensure_part_fully_loaded(owning_part);  
                UF_CALL(UF_ASSEM_set_work_part_quietly(owning_part, &part));  
                UF_CALL(UF_PART_ask_units(owning_part, &units));  
            }  
            else  
                UF_CALL(UF_PART_ask_units(part, &units));  
            if (units == UF_PART_METRIC)  
                units = UF_MODL_grams_centimeters;  
            else  
                units = UF_MODL_pounds_inches;  
            UF_CALL(UF_MODL_ask_body_density(body, units, &density));  
            if (prompt_for_a_number("Enter new density", "density", &density))  
                UF_CALL(UF_MODL_set_body_density(body, units, density));  
            if (is_occ)  
                UF_CALL(UF_ASSEM_set_work_part_quietly(part, &owning_part));  
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

> 这段代码是用于修改NX模型中实体的密度的二次开发代码。主要功能包括：
>
> 1. 定义了错误报告函数 report_error，用于打印错误代码和消息。
> 2. 定义了选择过滤函数 filter_body_type 和 mask_body_type，用于选择实体类型。
> 3. 定义了选择实体的函数 select_a_solid，用于选择一个实体。
> 4. 定义了提示输入数值的函数 prompt_for_a_number。
> 5. 定义了报告加载状态的函数 report_load_status。
> 6. 定义了确保零件完全加载的函数 ensure_part_fully_loaded。
> 7. 主函数 do_it 中实现了修改密度的逻辑：选择一个实体，如果是装配体中的实例，则切换到原型零件。检查并确保原型零件完全加载。读取并修改实体的密度。如果是实例，切换回原装配体。
> 8. 选择一个实体，如果是装配体中的实例，则切换到原型零件。
> 9. 检查并确保原型零件完全加载。
> 10. 读取并修改实体的密度。
> 11. 如果是实例，切换回原装配体。
> 12. ufusr 函数是NX二次开发的入口函数，初始化并调用 do_it 函数。
> 13. ufusr_ask_unload 函数用于卸载二次开发程序。
>
> 这段代码实现了在NX环境中选择实体并修改其密度的功能，适用于NX的二次开发。
>
