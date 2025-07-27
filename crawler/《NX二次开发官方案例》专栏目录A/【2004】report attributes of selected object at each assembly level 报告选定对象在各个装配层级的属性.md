### 【2004】report attributes of selected object at each assembly level 报告选定对象在各个装配层级的属性

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_attr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 版本新增的功能。 */  
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
    /*  Walk up the assy tree and put each parent instance in list for map 里海译:翻译：沿着装配树向上走，并将每个父实例放入列表以映射。 */  
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
    /*  Walk to the bottom of the same instance branch at each assy level 里海译:在每个装配层级走到底部相同的实例分支。 */  
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
    /*  Walk up the assy tree and put each parent instance in list for map 里海译:向上爬取assembly树，并将每个父实例放入列表以进行映射。 */  
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
    /*  Walk to the bottom of the same instance branch at each assy level 里海译:在每个装配级别，走到底部相同的实例分支。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define RX(X) report_simple_object_info(#X, X)  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void ask_object_type_descriptor(int type, int subtype, char *descriptor)  
    {  
        int  
            ii,  
            a_type;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            *ptr,  
            uoth_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        switch (type)  
        {  
            case UF_circle_type: /* non-std format in the header file 里海译:在头文件中使用非标准格式 */  
                strcpy(descriptor, "arc");  
                return;  
            case UF_OBJ_EXPR_TYPE:  
                strcpy(descriptor, "expression");  
                return;  
            case UF_OBJ_PART_TYPE:  
                strcpy(descriptor, "part");  
                return;  
            default:  
                break;  
        }  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf_object_types.h", uoth_file));  
        if ((in_file = fopen(uoth_file, "r")) == NULL)  
        {  
            strcpy(descriptor, "(uf_object_types.h not found)");  
            return;  
        }  
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:In case its not found，如果找不到的话。 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    break;  
                }  
            }  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if (ptr = strstr(define, "_type")) break;  
                if (a_type == subtype)  
                {  
                    if (ptr = strstr(define, "_subtype")) *ptr = '\0';  
                    if (strstr(define, descriptor) == define)  
                        strcpy(descriptor, define);  
                    else  
                    {  
                        strcat(descriptor, "/");  
                        strcat(descriptor, define);  
                    }  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
        while (ptr = strchr(descriptor, '_')) *ptr = ' ';  
    }  
    static void report_simple_object_info(char *what, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner;  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            owning_part[UF_CFI_MAX_FILE_NAME_SIZE],  
            msg[133];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d", what, object);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (object == NULL_TAG)  
        {  
            UF_CALL(UF_UI_write_listing_window("\n"));  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        if (status == UF_OBJ_DELETED)  
        {  
            sprintf(msg, ", status: %d\n", status);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            return;  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            UF_UI_write_listing_window(", named: ");  
            UF_UI_write_listing_window(name);  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ask_object_type_descriptor(type, subtype, msg);  
        UF_UI_write_listing_window(", type: ");  
        UF_UI_write_listing_window(msg);  
        if (UF_OBJ_is_object_a_promotion(object))  
            UF_UI_write_listing_window(", promotion");  
        if (UF_ASSEM_is_occurrence(object))  
            UF_UI_write_listing_window(", occurrence");  
        if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)))  
        {  
            ask_part_shortname(owner, owning_part);  
            UF_UI_write_listing_window(", in: ");  
            UF_UI_write_listing_window(owning_part);  
        }  
        UF_UI_write_listing_window("\n");  
    }  
    #define RA(X) report_object_attributes(#X, X)  
    static void report_object_attributes(char *name, tag_t object)  
    {  
        int  
            indx = 0,  
            n;  
        char  
            dstr[21],  
            mstr[UF_UI_MAX_STRING_LEN+1],  
            title[UF_ATTR_MAX_TITLE_LEN+1],  
            tstr[21];  
        UF_ATTR_value_t  
            att;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_ATTR_count_attributes(object, UF_ATTR_any, &n));  
        sprintf(mstr, "%s (%d) has %d attributes\n", name, object, n);  
        UF_CALL(UF_UI_write_listing_window(mstr));  
        while (!UF_CALL(UF_ATTR_cycle(object, &indx, UF_ATTR_any, title, &att))  
            && (indx != 0))  
        {  
            switch ( att.type )  
            {  
                case UF_ATTR_integer:  
                    sprintf(mstr, "%s = %d (INTEGER)\n", title, att.value.integer);  
                break;  
                case UF_ATTR_real:  
                    sprintf(mstr, "%s = %f (FLOATING POINT)\n", title,  
                        att.value.real);  
                break;  
                case UF_ATTR_time:  
                    UF_CALL(uc4582(att.value.time, 2, dstr, tstr));  
                    sprintf(mstr, "%s = %s %s (DATE & TIME)\n", title, dstr, tstr);  
                break;  
                case UF_ATTR_null:  
                    sprintf(mstr, "%s = (NULL)\n", title);  
                break;  
                case UF_ATTR_string:  
                    sprintf(mstr, "%s = %s (STRING)\n", title, att.value.string);  
                    UF_free(att.value.string);  
                break;  
                case UF_ATTR_reference:  
                    sprintf(mstr, "%s = %s (REFERENCE)\n", title,  
                        att.value.reference);  
                    UF_free(att.value.reference);  
                break;  
                default:  
                break;  
            }  
            UF_CALL(UF_UI_write_listing_window(mstr));  
        }  
        UF_CALL(UF_UI_write_listing_window("\n"));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_occs;  
        tag_t  
            object,  
            *occs;  
        while ((object = select_anything("Report attributes")) != NULL_TAG)  
        {  
            n_occs = get_occs_at_all_assy_levels(object, &occs);  
            for (ii = 0; ii < n_occs; ii++)  
            {  
                WRITE_D(ii);  
                RX(occs[ii]);  
                RA(occs[ii]);  
                WRITE("\n");  
            }  
            if (n_occs > 0) UF_free(occs);  
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

> 这段代码是NX的二次开发代码，其主要功能是查询和报告NX装配体中选定对象的所有属性。
>
> 主要功能包括：
>
> 1. 错误报告：通过自定义的report_error函数进行错误报告，包括文件名、行号、错误码等信息。
> 2. 对象选择：使用select_anything函数选择任意对象，并获取其属性。
> 3. 内存分配：使用allocate_memory函数进行内存分配。
> 4. 创建数组：通过make_an_array函数创建tag数组。
> 5. 获取组件实例映射：get_component_instance_map获取组件在装配树中的实例映射。
> 6. 获取组件位置：get_component_occs_at_all_assy_levels获取组件在所有装配层级的位置。
> 7. 获取对象实例映射：get_object_instance_map获取对象在装配树中的实例映射。
> 8. 获取对象位置：get_object_occs_at_all_assy_levels获取对象在所有装配层级的位置。
> 9. 获取对象位置：get_occs_at_all_assy_levels获取对象在所有装配层级的位置。
> 10. 报告对象信息：report_simple_object_info报告对象的简单信息。
> 11. 报告对象属性：report_object_attributes报告对象的属性。
> 12. 主函数：do_it函数为程序的入口，循环选择对象并报告其属性。
> 13. 初始化和终止：ufusr为程序的入口函数，初始化并调用do_it，然后终止NX。
> 14. 卸载询问：ufusr_ask_unload询问立即卸载用户函数库。
>
> 综上所述，这段代码实现了在NX装配体中查询和报告选定对象属性的功能，具有较好的结构，注释清晰，可供NX二次开发人员参考。
>
