### 【2291】report reference set 报告引用集

#### 代码

```cpp
    #define RX(X) report_object_info(#X, X)  
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
            case UF_circle_type: /* non-std format in the header file 译:在头文件中使用非标准格式 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:如果找不到的话， */  
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
    static void report_object_info(char *what, tag_t object)  
    {  
        logical  
            is_displayable,  
            is_so;  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner,  
            part = UF_PART_ask_display_part();  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            owning_part[UF_CFI_MAX_FILE_NAME_SIZE],  
            msg[133];  
        UF_SO_visibility_option_t  
            vis_opt;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d", what, object);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (object == NULL_TAG)  
        {  
            UF_CALL(UF_UI_write_listing_window(" (NULL_TAG)\n\n"));  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        sprintf(msg, ", status: %d", status);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (status == UF_OBJ_DELETED)  
        {  
            UF_CALL(UF_UI_write_listing_window(" (Deleted)\n\n"));  
            return;  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        sprintf(msg, ", type: %d, subtype: %d", type, subtype);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        ask_object_type_descriptor(type, subtype, msg);  
        UF_CALL(UF_UI_write_listing_window(" ("));  
        UF_CALL(UF_UI_write_listing_window(msg));  
        UF_CALL(UF_UI_write_listing_window(")"));  
        if (UF_OBJ_is_object_a_promotion(object))  
            UF_CALL(UF_UI_write_listing_window(", promotion"));  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            UF_CALL(UF_UI_write_listing_window(", occurrence"));  
            owner = UF_ASSEM_ask_part_occurrence(object);  
            if (owner != UF_ASSEM_ask_root_part_occ(part))  
            {  
                UF_CALL(UF_OBJ_ask_name(owner, msg));  
                UF_CALL(UF_UI_write_listing_window(" (from "));  
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_CALL(UF_UI_write_listing_window(")"));  
            }  
        }  
        if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)) &&  
            (owner != part))  
        {  
            ask_part_shortname(owner, owning_part);  
            sprintf(msg, ", owned by: %s", owning_part);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        if (!uc6409(object, &status, name) && (status == 1))  
        {  
            sprintf(msg, ", view dependent: %s", name);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            sprintf(msg, ", name: %s", name);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        UF_CALL(UF_SO_is_subclass(object, &is_so));  
        if (is_so)  
        {  
            UF_CALL(UF_SO_ask_visibility_option(object, &vis_opt));  
            if (vis_opt == UF_SO_invisible)  
            {  
                UF_CALL(UF_UI_write_listing_window(", invisible SO"));  
            }  
            else  
            {  
                UF_CALL(UF_UI_write_listing_window(", visible SO"));  
            }  
        }  
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayble));  
        if (is_displayable)  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
            sprintf(msg, "\n  layer: %d, color: %d, blank: %d, width: %d, "  
                "font: %d, highlight: %d\n\n", disp_props.layer,  
                disp_props.color, disp_props.blank_status,  
                disp_props.line_width, disp_props.font,  
                disp_props.highlight_status);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        else  
            UF_CALL(UF_UI_write_listing_window("\n\n"));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_reference_set(tag_t refset)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *members;  
        RX(refset);  
        UF_CALL(UF_ASSEM_ask_ref_set_members(refset, &n, &members));  
        for (ii = 0; ii < n; ii++)  
        {  
            WRITE_D(ii);  
            RX(members[ii]);  
        }  
        if (n > 0) UF_free(members);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于获取和报告NX模型中对象的信息。主要功能包括：
>
> 1. 获取对象类型和子类型描述：ask_object_type_descriptor函数从头文件中获取对象类型和子类型的描述字符串。
> 2. 报告对象信息：report_object_info函数获取并报告对象的名称、状态、类型、子类型、拥有者、显示属性等信息。
> 3. 报告引用集信息：report_reference_set函数获取并报告引用集中的成员对象。
> 4. 辅助函数：包括write_integer_to_listing_window用于打印整数到列表窗口，以及宏RX和WRITE_D用于简化调用。
>
> 通过这些函数，用户可以快速获取模型中对象的各种信息，以便进行调试或其他二次开发需求。这段代码封装了NX API，提供了较为方便的接口，具有一定的实用价值。
>
