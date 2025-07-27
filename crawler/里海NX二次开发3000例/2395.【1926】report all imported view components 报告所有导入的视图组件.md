### 【1926】report all imported view components 报告所有导入的视图组件

#### 代码

```cpp
    /*HEAD REPORT_ALL_IMPORTED_VIEW_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_so.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的提示，UF_print_syslog 是 V18 新增的功能。因此，翻译内容为：

注意：UF_print_syslog 是 V18 新增的功能，只提供翻译，不要添加其他内容。 */  
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
    /*  There is also an attribute REFERENCE_COMPONENT automatically assigned to  
        these components.  An alternate method would be to use  
        UF_ATTR_find_attribute to see if that string attribute exists on the  
        component.  
        NOTE:  This is obsolete as of NX5.0.  See UF_DRAW_is_drafting_component.  
    */  
    static logical is_imported_view_component(tag_t component)  
    {  
        logical  
            is_imported = FALSE;  
        int  
            ii,  
            n,  
            subtype,  
            type;  
        tag_t  
            *children;  
        UF_CALL(UF_SO_ask_children(component, UF_SO_ASK_ALL_CHILDREN, &n,  
            &children));  
        for (ii = 0; ii < n; ii++)  
        {  
            if (UF_OBJ_ask_status(children[ii]) != UF_OBJ_ALIVE) continue;  
            UF_CALL(UF_OBJ_ask_type_and_subtype(children[ii], &type, &subtype));  
            if ((type == UF_view_type) && (subtype == UF_view_imported_subtype))  
            {  
                is_imported = TRUE;  
                break;  
            }  
        }  
        if (n > 0) UF_free(children);  
        return is_imported;  
    }  
    /*  
        This function is basically the same as UF_ASSEM_ask_part_occ_children  
        except that suppressed components are included.  
    */  
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
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    static int ask_all_immediate_children(tag_t part, tag_t **children)  
    {  
        tag_t  
            comp = NULL_TAG,  
            inst,  
            parent;  
        uf_list_p_t  
            child_list;  
        UF_CALL(UF_MODL_create_list(&child_list));  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            if ((inst = UF_ASSEM_ask_inst_of_part_occ(comp)) != NULL_TAG)  
            {  
                parent = UF_ASSEM_ask_parent_of_instance(inst);  
                if (parent == part)  
                    UF_CALL(UF_MODL_put_list_item(child_list, comp));  
            }  
        }  
        return (make_an_array(&child_list, children));  
    }  
    #define RX(X) report_simple_object_info(#X, X)  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
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
            case UF_circle_type: /* non-std format in the header file 里海译:在头文件中，使用非标准格式进行声明。 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话 */  
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
            UF_CALL(UF_UI_write_listing_window(" (NULL_TAG)\n"));  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *children,  
            part = UF_PART_ask_display_part();  
        n = ask_all_immediate_children(part, &children);  
        for (ii = 0; ii < n; ii++)  
            if (is_imported_view_component(children[ii]))  
                RX(children[ii]);  
        if (n > 0) UF_free(children);  
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

> 根据代码内容，这是一段用于NX的二次开发代码，其主要功能是：
>
> 1. 错误报告函数：定义了一个名为report_error的函数，用于打印函数调用失败时的错误信息，包括文件名、行号、函数名和错误码等。
> 2. 判断组件是否为导入视图组件：定义了一个名为is_imported_view_component的函数，用于判断一个组件是否为导入的视图组件。
> 3. 获取零件的直接子组件：定义了allocate_memory、make_an_array和ask_next_component等辅助函数，用于获取零件的直接子组件，包括被抑制的组件。
> 4. 打印对象信息：定义了ask_part_shortname、ask_object_type_descriptor和report_simple_object_info等函数，用于获取对象的相关信息，如类型、子类型、名称、拥有零件等，并打印出来。
> 5. 遍历组件并打印：在do_it函数中，遍历当前显示零件的所有直接子组件，判断是否为导入视图组件，如果是，则调用report_simple_object_info打印组件信息。
> 6. ufusr函数：作为主入口函数，初始化NX环境后调用do_it函数，然后终止NX环境。
> 7. 卸载函数：定义了ufusr_ask_unload函数，用于在用户退出NX时立即卸载此代码。
>
> 总的来说，这段代码的主要功能是遍历当前显示零件的所有直接子组件，判断是否为导入的视图组件，如果是，则打印组件的详细信息。
>
