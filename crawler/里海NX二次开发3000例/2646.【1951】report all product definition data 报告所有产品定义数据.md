### 【1951】report all product definition data 报告所有产品定义数据

#### 代码

```cpp
    /*HEAD REPORT_ALL_PRODUCT_DEFINITION_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_pd.h>  
    #include <uf_obj.h>  
    #include <uf_group.h>  
    #include <uf_object_types.h>  
    #include <uf_view.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 新增的功能。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_product_definition(tag_t part, tag_t pd)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part,  
            UF_smsp_product_definition_type, &pd)) && (pd != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(pd, &type, &subtype));  
            if (subtype == UF_smsp_product_definition_subtype) return pd;  
        }  
        return NULL_TAG;  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
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
            case UF_circle_type: /* non-std format in the header file 里海译:头文件中的非标准格式翻译为：头文件中的非标准格式。 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话，请这样回答：如果找不到。 */  
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
    static void ask_object_font_descriptor(int font, char *font_name)  
    {  
        switch (font)  
        {  
            case UF_OBJ_FONT_INVISIBLE:  
                strcpy(font_name, "invisible");  
                break;  
            case UF_OBJ_FONT_SOLID:  
                strcpy(font_name, "solid");  
                break;  
            case UF_OBJ_FONT_DASHED:  
                strcpy(font_name, "dashed");  
                break;  
            case UF_OBJ_FONT_PHANTOM:  
                strcpy(font_name, "phantom");  
                break;  
            case UF_OBJ_FONT_CENTERLINE:  
                strcpy(font_name, "centerline");  
                break;  
            case UF_OBJ_FONT_DOTTED:  
                strcpy(font_name, "dotted");  
                break;  
            case UF_OBJ_FONT_LONG_DASHED:  
                strcpy(font_name, "long dashed");  
                break;  
            case UF_OBJ_FONT_DOTTED_DASHED:  
                strcpy(font_name, "dotted dashed");  
                break;  
            default:  
                strcpy(font_name, "unknown font");  
                break;  
        }  
    }  
    /*  Prior to NX 4.0, the returned full color name looked like this:  
        full_color_name = "2 (Green)"  
        So the method shown here cleans it up and returns just the name.  
    */  
    static void ask_color_name(int color_num, char *color_name)  
    {  
        double  
            clr_values[3];  
        char  
            *full_color_name;  
        UF_CALL(UF_DISP_ask_color(color_num, UF_DISP_rgb_model, &full_color_name,  
            clr_values));  
        if (strchr(full_color_name, '('))  
        {  
            strcpy(color_name, strchr(full_color_name, '(')+1);  
            strcpy(strchr(color_name, ')'), "");  
        }  
        else  
            strcpy(color_name, full_color_name);  
        UF_free(full_color_name);  
    }  
    #define WRITEnT(n,X) (report_object_array(n, X))  
    static void report_object_array(int n_objects, tag_t *objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii,  
            subtype,  
            status,  
            type;  
        tag_t  
            group;  
        char  
            blanked_flag[4],  
            color_name[21],  
            font_name[15],  
            group_name[MAX_ENTITY_NAME_SIZE+1],  
            layer_name[4],  
            msg[UF_UI_MAX_STRING_LEN+1],  
            object_name[MAX_ENTITY_NAME_SIZE+1],  
            type_desc[MAX_LINE_SIZE+1],  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        double  
            clr_values[3];  
        UF_OBJ_disp_props_t  
            disp_props;  
        if (n_objects < 1) return;  
        number_objects(objects, n_objects);  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%5s  %-36s  %-7s  %-20s  %-15s  %-7s  %-30s  %-30s  %-30s\n",  
            "No.", "type", "layer", "color", "font", "blanked", "view dependency",  
            "name", "member of group");  
        UF_CALL(UF_UI_write_listing_window(msg));  
        sprintf(msg, "%5s  %-36s  %-7s  %-20s  %-15s  %-7s  %-30s  %-30s  %-30s\n",  
            "----", "------------------------------------", "-----",  
            "--------------------", "---------------", "-------",  
            "------------------------------", "------------------------------",  
            "------------------------------" );  
        UF_CALL(UF_UI_write_listing_window(msg));  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            ask_object_type_descriptor(type, subtype, type_desc);  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (is_displayable)  
            {  
                UF_CALL(UF_OBJ_ask_display_properties(objects[ii], &disp_props));  
                sprintf(layer_name, "%d", disp_props.layer);  
                ask_color_name(disp_props.color, color_name);  
                ask_object_font_descriptor(disp_props.font, font_name);  
                if (disp_props.blank_status == UF_OBJ_BLANKED)  
                    strcpy(blanked_flag, "Yes");  
                else  
                    strcpy(blanked_flag, "No");  
                uc6409(objects[ii], &status, view_name);  
                if (status == 0) strcpy(view_name, "n/a");  
                UF_CALL(UF_GROUP_ask_group_of_tag(objects[ii], &group));  
                if (group == NULL_TAG)  
                    strcpy(group_name, "no");  
                else  
                    if (UF_OBJ_ask_name(group, group_name))  
                        strcpy(group_name, "yes (unnamed)");  
            }  
            else  
            {  
                strcpy(layer_name, "N/A");  
                strcpy(color_name, "N/A");  
                strcpy(font_name, "N/A");  
                strcpy(blanked_flag, "N/A");  
                strcpy(view_name, "N/A");  
                strcpy(group_name, "N/A");  
            }  
            strcpy(object_name, "(unnamed)");  
            UF_OBJ_ask_name(objects[ii], object_name);  
            sprintf(msg,  
                "%4d.  %-36s  %-7s  %-20s  %-15s  %-7s  %-30s  %-30s  %-30s\n",  
                ii+1, type_desc, layer_name, color_name, font_name, blanked_flag,  
                view_name, object_name, group_name);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    static void report_product_definition_data(tag_t pd)  
    {  
        int  
            ii, jj, kk;  
        UF_PD_product_definition_p_t  
            pds;  
        if (UF_CALL(UF_PD_ask_product_definition_data(pd, &pds))) return;  
        WRITE_S(pds->name);  
        WRITE_D(pds->num_geometry_references);  
        report_object_array(pds->num_geometry_references, pds->geometry_references);  
        WRITE_L(pds->retained);  
        WRITE_D(pds->num_product_attributes);  
        for (ii = 0; ii < pds->num_product_attributes; ii++)  
        {  
            WRITE("pds->product_attributes[ii].attribute_type = ");  
            switch (pds->product_attributes[ii].attribute_type)  
            {  
                case UF_PD_NULL_ATTRIBUTE_TYPE:  
                    WRITE_D(UF_PD_NULL_ATTRIBUTE_TYPE);  
                    break;  
                case UF_PD_UG_USER_DEFINED:  
                    WRITE_D(UF_PD_UG_USER_DEFINED);  
                    break;  
                case UF_PD_STRING_TYPE:  
                    WRITE_D(UF_PD_STRING_TYPE);  
                    break;  
                case UF_PD_INTEGER_TYPE:  
                    WRITE_D(UF_PD_INTEGER_TYPE);  
                    break;  
                case UF_PD_NUMBER_TYPE:  
                    WRITE_D(UF_PD_NUMBER_TYPE);  
                    break;  
                case UF_PD_ENTERPRISE_IDENTIFIER_TYPE:  
                    WRITE_D(UF_PD_ENTERPRISE_IDENTIFIER_TYPE);  
                    break;  
                case UF_PD_PART_IDENTIFIER_TYPE:  
                    WRITE_D(UF_PD_PART_IDENTIFIER_TYPE);  
                    break;  
                case UF_PD_MATERIAL_SPECIFICATION_TYPE:  
                    WRITE_D(UF_PD_MATERIAL_SPECIFICATION_TYPE);  
                    break;  
                case UF_PD_PROCESS_SPECIFICATION_TYPE:  
                    WRITE_D(UF_PD_PROCESS_SPECIFICATION_TYPE);  
                    break;  
                case UF_PD_SURFACE_FINISH_TYPE:  
                    WRITE_D(UF_PD_SURFACE_FINISH_TYPE);  
                    break;  
                case UF_PD_GENERIC_NOTE_TYPE:  
                    WRITE_D(UF_PD_GENERIC_NOTE_TYPE);  
                    break;  
                case UF_PD_SPECIFIC_NOTE_TYPE:  
                    WRITE_D(UF_PD_SPECIFIC_NOTE_TYPE);  
                    break;  
                case UF_PD_BALLOON_NOTE_TYPE:  
                    WRITE_D(UF_PD_BALLOON_NOTE_TYPE);  
                    break;  
                case UF_PD_LOCATOR_DESIGNATOR_TYPE:  
                    WRITE_D(UF_PD_LOCATOR_DESIGNATOR_TYPE);  
                    break;  
                default:  
                    WRITE("Unknown type, ");  
                    WRITE_D(pds->product_attributes[ii].attribute_type);  
                    break;  
            }  
            WRITE_S(pds->product_attributes[ii].attribute_group);  
            WRITE_S(pds->product_attributes[ii].attribute_type_name);  
            WRITE_S(pds->product_attributes[ii].attribute_name);  
            WRITE_S(pds->product_attributes[ii].symbolic_display);  
            WRITE_S(pds->product_attributes[ii].kf_name);  
            WRITE_D(pds->product_attributes[ii].number_busmods);  
            for (jj = 0; jj < pds->product_attributes[ii].number_busmods; jj++)  
            {  
                WRITE_S(pds->product_attributes[ii].busmods[jj].title);  
                WRITE_S(pds->product_attributes[ii].busmods[jj].value);  
            }  
            WRITE_D(pds->product_attributes[ii].num_attribute_values);  
            for (jj = 0;  
                 jj < pds->product_attributes[ii].num_attribute_values;  
                 jj++)  
            {  
                WRITE("pds->product_attributes[ii].values[jj].value_type = ");  
                switch (pds->product_attributes[ii].values[jj].value_type)  
                {  
                    case UF_PD_INTEGER_VALUE:  
                        WRITE_D(UF_PD_INTEGER_VALUE);  
                        break;  
                    case UF_PD_NUMBER_VALUE:  
                        WRITE_D(UF_PD_NUMBER_VALUE);  
                        break;  
                    case UF_PD_STRING_VALUE:  
                        WRITE_D(UF_PD_STRING_VALUE);  
                        break;  
                    case UF_PD_LIST_VALUE:  
                        WRITE_D(UF_PD_LIST_VALUE);  
                        break;  
                    case UF_PD_URL_VALUE:  
                        WRITE_D(UF_PD_URL_VALUE);  
                        break;  
                    default:  
                        WRITE("Unknown type, ");  
                        WRITE_D(pds->product_attributes[ii].values[jj].value_type);  
                        break;  
                }  
                WRITE_S(pds->product_attributes[ii].values[jj].value_title);  
                WRITE_D(pds->product_attributes[ii].values[jj].integer_value);  
                WRITE_F(pds->product_attributes[ii].values[jj].number_value);  
                WRITE_S(pds->product_attributes[ii].values[jj].string_value);  
            }  
            WRITE_D(pds->product_attributes[ii].num_attribute_value_dats);  
            for (jj = 0;  
                 jj < pds->product_attributes[ii].num_attribute_value_dats;  
                 jj++)  
            {  
              WRITE_S(pds->product_attributes[ii].value_dats[jj].title);  
              WRITE_D(pds->product_attributes[ii].value_dats[jj].dat.num_strings);  
              for (kk = 0;  
                  kk < pds->product_attributes[ii].value_dats[jj].dat.num_strings;  
                  kk++)  
              {  
                WRITE_S(pds->product_attributes[ii].value_dats[jj].dat.strings[kk]);  
              }  
            }  
        }  
        UF_CALL(UF_PD_free_product_definition_data(&pds));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            a_pd = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while (( a_pd = ask_next_product_definition(part, a_pd)) != NULL_TAG)  
            report_product_definition_data(a_pd);  
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

> 这段NX Open C++代码主要用于报告当前部件中的所有产品定义(Product Definition)的相关数据。具体来说，该代码实现了以下功能：
>
> 1. 定义了UF_CALL宏，用于执行UF函数并报告错误。
> 2. 定义了各种WRITE_XXX宏，用于向日志窗口写入不同类型的数据。
> 3. 实现了ask_next_product_definition函数，用于遍历部件中的所有产品定义。
> 4. 实现了display_temporary_text函数，用于在对象上显示临时文字。
> 5. 实现了number_objects函数，用于给对象编号。
> 6. 实现了ask_object_type_descriptor函数，用于获取对象类型描述。
> 7. 实现了ask_object_font_descriptor函数，用于获取对象字体描述。
> 8. 实现了ask_color_name函数，用于获取颜色名称。
> 9. 实现了report_object_array函数，用于报告对象数组信息。
> 10. 实现了report_product_definition_data函数，用于报告产品定义数据。
> 11. 实现了do_it函数，用于遍历所有产品定义并报告其数据。
> 12. 实现了ufusr函数，作为二次开发程序的入口。
> 13. 实现了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 总体来说，这段代码通过遍历当前部件中的所有产品定义，并获取每个产品定义的详细信息，实现了对产品定义数据的报告功能。
>
