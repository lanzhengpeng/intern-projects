### 【1269】find and show selected object at each assembly level 在各个装配级别查找并显示所选对象

#### 代码

```cpp
    /*HEAD FIND_AND_SHOW_SELECTED_OBJECT_AT_EACH_ASSEMBLY_LEVEL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static int get_component_instance_map(tag_t part_occ, tag_t **instances)  
    {  
        tag_t  
            instance;  
        uf_list_p_t  
            instance_list;  
        UF_CALL(UF_MODL_create_list(&instance_list));  
    /*  Walk up the assy tree and put each parent instance in list for map 里海译:向上遍历装配树，并将每个父实例放入列表中映射 */  
        while (part_occ != NULL_TAG)  
        {  
            instance = UF_ASSEM_ask_inst_of_part_occ(part_occ);  
            if (instance != NULL_TAG)  
                UF_CALL(UF_MODL_put_list_item(instance_list, instance));  
            UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ));  
        }  
        return make_an_array(&instance_list, instances);  
    }  
    static int get_component_occs_at_all_assy_levels(tag_t part_occ, tag_t **occs)  
    {  
        int  
            ii, jj,  
            n_instances;  
        tag_t  
            *instances;  
        uf_list_p_t  
            occ_list;  
        *occs = NULL;  
        if (!UF_ASSEM_is_part_occurrence(part_occ)) return 0;  
        UF_CALL(UF_MODL_create_list(&occ_list));  
        UF_CALL(UF_MODL_put_list_item(occ_list, part_occ));  
        n_instances = get_component_instance_map(part_occ, &instances);  
    /*  Walk to the bottom of the same instance branch at each assy level 里海译:在每个装配级别走到同一个实例分支的底部。 */  
        for (ii = n_instances - 1; ii > 0; ii--)  
        {  
            part_occ = UF_ASSEM_ask_root_part_occ(  
                UF_ASSEM_ask_child_of_instance(instances[ii]));  
            for (jj = ii - 1; jj >= 0; jj--)  
                part_occ = UF_ASSEM_ask_part_occ_of_inst(part_occ, instances[jj]);  
            UF_CALL(UF_MODL_put_list_item(occ_list, part_occ));  
        }  
        if (n_instances > 0) UF_free(instances);  
        return make_an_array(&occ_list, occs);  
    }  
    static int get_object_instance_map(tag_t object_occ, tag_t **instances)  
    {  
        tag_t  
            instance,  
            part_occ;  
        uf_list_p_t  
            instance_list;  
        UF_CALL(UF_MODL_create_list(&instance_list));  
    /*  Walk up the assy tree and put each parent instance in list for map 里海译:向上遍历装配树，并将每个父实例放入列表以供映射。 */  
        UF_CALL(UF_ASSEM_ask_parent_component(object_occ, &part_occ));  
        while (part_occ != NULL_TAG)  
        {  
            instance = UF_ASSEM_ask_inst_of_part_occ(part_occ);  
            if (instance != NULL_TAG)  
                UF_CALL(UF_MODL_put_list_item(instance_list, instance));  
            UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ));  
        }  
        return make_an_array(&instance_list, instances);  
    }  
    static int get_object_occs_at_all_assy_levels(tag_t object_occ, tag_t **occs)  
    {  
        int  
            ii, jj,  
            n_instances;  
        tag_t  
            *instances,  
            object_proto,  
            part_occ;  
        uf_list_p_t  
            occ_list;  
        *occs = NULL;  
        if (!UF_ASSEM_is_occurrence(object_occ)  
            || UF_ASSEM_is_part_occurrence(object_occ)) return 0;  
        UF_CALL(UF_MODL_create_list(&occ_list));  
        UF_CALL(UF_MODL_put_list_item(occ_list, object_occ));  
        n_instances = get_object_instance_map(object_occ, &instances);  
        object_proto = UF_ASSEM_ask_prototype_of_occ(object_occ);  
    /*  Walk to the bottom of the same instance branch at each assy level 里海译:在每个装配级别沿着相同的实例分支向下走。 */  
        for (ii = n_instances - 1; ii > 0; ii--)  
        {  
            part_occ = UF_ASSEM_ask_root_part_occ(  
                UF_ASSEM_ask_child_of_instance(instances[ii]));  
            for (jj = ii - 1; jj >= 0; jj--)  
                part_occ = UF_ASSEM_ask_part_occ_of_inst(part_occ, instances[jj]);  
            object_occ = UF_ASSEM_find_occurrence(part_occ, object_proto);  
            UF_CALL(UF_MODL_put_list_item(occ_list, object_occ));  
        }  
        UF_CALL(UF_MODL_put_list_item(occ_list, object_proto));  
        if (n_instances > 0) UF_free(instances);  
        return make_an_array(&occ_list, occs);  
    }  
    static int get_occs_at_all_assy_levels(tag_t object, tag_t **occs)  
    {  
        *occs = NULL;  
        if (UF_ASSEM_is_part_occurrence(object))  
            return get_component_occs_at_all_assy_levels(object, occs);  
        if (UF_ASSEM_is_occurrence(object))  
            return get_object_occs_at_all_assy_levels(object, occs);  
        return 0;  
    }  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_occs;  
        tag_t  
            object,  
            *occs,  
            parent,  
            part = UF_PART_ask_display_part();  
        UF_ASSEM_options_t  
            options;  
        char  
            pn[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_ASSEM_ask_assem_options(&options));  
        if (options.maintain_work_part != UF_ASSEM_dont_maintain_work_part)  
        {  
            options.maintain_work_part = UF_ASSEM_dont_maintain_work_part;  
            UF_CALL(UF_ASSEM_set_assem_options(&options));  
            options.maintain_work_part = UF_ASSEM_do_maintain_work_part;  
        }  
        while ((object = select_anything("Show at all assembly levels"))  
            != NULL_TAG)  
        {  
            if ((n_occs = get_occs_at_all_assy_levels(object, &occs)) == 0)  
                continue;  
            for (ii = 0; ii < n_occs; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_owning_part(occs[ii], &parent));  
                UF_CALL(UF_PART_set_display_part(parent));  
                UF_CALL(UF_DISP_set_highlight(occs[ii], TRUE));  
                ask_part_shortname(parent, pn);  
                uc1601(pn, TRUE);  
                UF_CALL(UF_DISP_set_highlight(occs[ii], FALSE));  
            }  
            UF_free(occs);  
            UF_CALL(UF_PART_set_display_part(part));  
        }  
        if (options.maintain_work_part != UF_ASSEM_dont_maintain_work_part)  
            UF_CALL(UF_ASSEM_set_assem_options(&options));  
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

> 这段代码是用于NX软件的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告：包含了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 选择对象：定义了函数select_anything，用于让用户在对话框中选择一个对象，并返回该对象的tag。
> 3. 获取对象在所有装配层级的实例：定义了函数get_occs_at_all_assy_levels，用于获取指定对象在所有装配层级的实例，并返回实例的tag数组。
> 4. 内存分配和释放：包含了一些用于内存分配和释放的函数。
> 5. 遍历装配树：定义了函数get_component_instance_map和get_object_instance_map，用于遍历装配树，获取指定对象实例的所有父实例。
> 6. 设置高亮显示：定义了函数do_it，用于遍历指定对象的所有实例，在每一级装配中设置高亮显示。
> 7. 获取零件简称：定义了函数ask_part_shortname，用于获取指定零件的简称。
> 8. 主函数：定义了函数ufusr，作为程序的入口点，调用函数do_it实现功能。
> 9. 卸载请求：定义了函数ufusr_ask_unload，用于在程序卸载时请求立即卸载。
>
> 总的来说，这段代码主要用于在NX装配模型中，高亮显示用户选择的对象在所有装配层级的实例，以方便查看和检查。
>
