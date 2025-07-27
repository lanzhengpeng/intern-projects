### 【1962】report all reference set members 报告所有引用集的成员，包括嵌套的组件和零件

#### 代码

```cpp
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
            case UF_circle_type: /* non-std format in the header file 译:非标准格式在头文件中 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:如果找不到的话 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    if (strcmp(define, "gfem_load")) break;  
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
    static void report_component_member_tree(tag_t comp, int indent)  
    {  
        logical  
            parent_suppressed,  
            suppressed;  
        int  
            ii,  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            abs[6] = { 1,0,0, 0,1,0 },  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            desc[MAX_LINE_SIZE+1],  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            reference_set[MAX_ENTITY_NAME_SIZE+1];  
        while ((object = UF_ASSEM_cycle_ents_in_part_occ(comp, object)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if ((subtype == UF_solid_face_subtype) ||  
                (subtype == UF_solid_edge_subtype)) continue;  
            ask_object_type_descriptor(type, subtype, desc);  
            if (type == UF_component_type)  
            {  
                UF_CALL(UF_ASSEM_ask_component_data(object, part_name,  
                    reference_set, instance_name, origin, csys_matrix, transform));  
                for (ii = 0; ii < indent; ++ii) ECHO(" ");  
                ECHO("%s - %s uses reference set: %s",  
                    desc, instance_name, reference_set);  
                UF_CALL(UF_ASSEM_ask_part_occ_suppress_state(object,  
                    &suppressed, &parent_suppressed));  
                if (suppressed) ECHO(" (Suppressed)\n");  
                else  
                {  
                    ECHO("\n");  
                    report_component_member_tree(object, indent+2);  
                }  
            }  
            else  
            {  
                for (ii = 0; ii < indent; ++ii) ECHO(" ");  
                ECHO("%s\n", desc);  
            }  
        }  
    }  
    /*  
        UF_ASSEM_ask_ref_set_members only returns the immediate members of the  
        reference set.  This reports every object that will be brought  
        into a higher level assembly when the reference set is used.  
    */  
    static void report_all_reference_set_members(tag_t temp_part, tag_t refset)  
    {  
        tag_t  
            arr,  
            new_inst,  
            part = UF_PART_ask_display_part();  
        double  
            abs[6] = { 1,0,0, 0,1,0 };  
        char  
            default_arrangement[MAX_ENTITY_NAME_SIZE+1],  
            reference_set[MAX_ENTITY_NAME_SIZE+1],  
            this_part[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_PART_ask_part_name(part, this_part));  
        UF_CALL(UF_OBJ_ask_name(refset, reference_set));  
        UF_CALL(UF_ASSEM_ask_default_arrangement(part, &arr));  
        UF_CALL(UF_OBJ_ask_name(arr, default_arrangement));  
        ECHO("%s contents in Arrangement %s (Default):\n", reference_set,  
            default_arrangement);  
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_PART_set_display_part(temp_part));  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(temp_part, this_part,  
            reference_set, NULL, abs, abs, 0, &new_inst, &error_status));  
        report_load_status(&error_status);  
        report_component_member_tree(UF_ASSEM_ask_part_occ_of_inst(  
                       UF_ASSEM_ask_root_part_occ(temp_part), new_inst), 2);  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
        UF_CALL(UF_PART_set_display_part(part));  
        UF_CALL(UF_DISP_set_display(TRUE));  
    }

```

#### 代码解析

> 这段代码是NX Open C++的二次开发代码，主要实现以下功能：
>
> 1. 获取对象类型描述：ask_object_type_descriptor函数通过读取uf_object_types.h头文件，获取NX对象类型(type)和子类型(subtype)对应的描述字符串(descriptor)。
> 2. 报告组件成员树：report_component_member_tree函数递归遍历组件成员树，并打印出每个成员的类型描述，如果是组件类型，则递归打印其成员树。
> 3. 报告引用集所有成员：report_all_reference_set_members函数首先创建一个临时组件，将当前组件添加到该临时组件的默认引用集中。然后调用report_component_member_tree函数打印出该临时组件下所有成员，从而实现对引用集所有成员的递归遍历和打印。
> 4. 辅助函数：代码中还包括一些辅助函数，如report_load_status用于打印组件加载状态，report_component_member_tree用于递归打印组件成员树等。
>
> 总体来说，这段代码通过递归遍历组件树，实现了对NX装配模型中组件和引用集成员的查询和打印功能。通过读取NX头文件获取对象类型描述，增强了代码的可读性和可维护性。
>
