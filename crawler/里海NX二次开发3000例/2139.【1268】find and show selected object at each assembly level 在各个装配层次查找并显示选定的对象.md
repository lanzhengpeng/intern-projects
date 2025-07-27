### 【1268】find and show selected object at each assembly level 在各个装配层次查找并显示选定的对象

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的函数。 */  
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
    /*  Walk up the assy tree and put each parent instance in list for map 里海译:爬上装配树，将每个父实例放入列表以映射。 */  
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
    /*  Walk to the bottom of the same instance branch at each assy level 里海译:在每个装配级别沿着相同的实例分支走到底部。 */  
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
    /*  Walk up the assy tree and put each parent instance in list for map 里海译:爬上组装树，并将每个父实例放入列表以供映射 */  
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
    /*  Walk to the bottom of the same instance branch at each assy level 里海译:在每个装配级别沿同一实例分支走到底。 */  
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

> 这段NX二次开发代码的功能是显示用户在装配树中选择的任何对象在所有装配级别上的实例。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数出错时输出错误信息。
> 2. 实现了mask_add_faces_and_edges函数，用于设置选择掩码，允许用户选择面和边。
> 3. 实现了select_anything函数，使用单选对话框让用户选择一个对象，并高亮显示。
> 4. 实现了allocate_memory函数，用于申请内存。
> 5. 实现了make_an_array函数，用于将列表转换为数组。
> 6. 实现了get_component_instance_map和get_component_occs_at_all_assy_levels函数，用于获取组件在所有装配级别上的实例。
> 7. 实现了get_object_instance_map和get_object_occs_at_all_assy_levels函数，用于获取非组件对象在所有装配级别上的实例。
> 8. 实现了get_occs_at_all_assy_levels函数，用于获取对象在所有装配级别上的实例。
> 9. 实现了ask_part_shortname函数，用于获取部件的短名称。
> 10. 实现了do_it函数，用于主逻辑，循环获取用户选择的对象，获取其在所有装配级别的实例，并高亮显示。
> 11. 实现了ufusr函数，作为NX二次开发程序的入口。
> 12. 实现了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 通过以上功能的组合，用户可以方便地在NX装配树中选择对象，并查看其在所有装配级别上的实例，有助于进行装配设计检查。
>
