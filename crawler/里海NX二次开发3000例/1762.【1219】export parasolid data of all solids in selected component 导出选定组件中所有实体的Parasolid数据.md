### 【1219】export parasolid data of all solids in selected component 导出选定组件中所有实体的Parasolid数据

#### 代码

```cpp
    /*HEAD EXPORT_PARASOLID_DATA_OF_ALL_SOLIDS_IN_SELECTED_COMPONENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ps.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
注意：UF_print_syslog 是 V18 中新增的，只回答译文，不要赘述。

Note: UF_print_syslog is new in V18, only answer the translation, no chit-chat. */  
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
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static void ask_component_shortname(tag_t component, char *shortname)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    static tag_t ask_next_local_solid(tag_t part, tag_t solid)  
    {  
        int  
            subtype,  
            status,  
            type;  
        while ((solid = UF_OBJ_cycle_all(part, solid)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(solid, &type, &subtype));  
            status = UF_OBJ_ask_status(solid);  
            if ((type == UF_solid_type) &&  
                (subtype == UF_solid_body_subtype) &&  
                ((status == 3) || (UF_OBJ_is_object_a_promotion(solid))) &&  
                (!UF_ASSEM_is_occurrence(solid))) return solid;  
        }  
        return NULL_TAG;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            comp,  
            part,  
            solid = NULL_TAG;  
        char  
            msg[UF_UI_MAX_STRING_LEN],  
            comp_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            ps_spec[UF_CFI_MAX_PATH_NAME_SIZE];  
        uf_list_p_t  
            bodies;  
        while ((comp = select_a_component("Export ps data")) != NULL_TAG)  
        {  
            part = UF_ASSEM_ask_prototype_of_occ(comp);  
            ensure_part_fully_loaded(part);  
            ask_component_shortname(comp, comp_spec);  
            build_similar_temp_filespec(comp_spec, 0, ps_spec);  
            strcat(ps_spec, ".x_t");  
            UF_CALL(UF_MODL_create_list(&bodies));  
            while ((solid = ask_next_local_solid(part, solid)) != NULL_TAG)  
                UF_CALL(UF_MODL_put_list_item(bodies, solid));  
            UF_CALL(UF_MODL_ask_list_count(bodies, &n));  
            if (n > 0)  
            {  
                uc4561(ps_spec, 0);  
                sprintf(msg, "Exporting %d solids to %s\n", n, ps_spec);  
                WRITE(msg);  
                UF_CALL(UF_PS_export_data(bodies, ps_spec));  
            }  
            else  
            {  
                sprintf(msg, "No solids found in %s\n", comp_spec);  
                WRITE(msg);  
            }  
            UF_CALL(UF_MODL_delete_list(&bodies));  
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

> 根据代码的内容，这是一段用于在NX中导出选定组件中所有实体的Parasolid数据的二次开发代码。
>
> 主要步骤包括：
>
> 1. 初始化NX环境。
> 2. 循环选择组件，并获取其原型零件。
> 3. 确保原型零件完全加载。
> 4. 获取组件的简短名称，并构建临时的Parasolid文件路径。
> 5. 遍历零件中的所有实体，将实体添加到列表中。
> 6. 如果实体数量大于0，则调用UF_PS_export_data函数将实体列表导出到Parasolid文件。
> 7. 如果没有实体，则提示没有找到实体。
> 8. 循环选择，直到用户取消选择。
> 9. 清理实体列表。
> 10. 退出NX环境。
>
> 该代码通过选择组件，然后导出该组件中的所有实体，实现了Parasolid数据的批量导出功能。
>
