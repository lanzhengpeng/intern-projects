### 【2234】report object data 报告对象数据

#### 代码

```cpp
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void flag_object(tag_t object, char *msg)  
    {  
        double  
            loc[3];  
        if (UF_OBJ_ask_name_origin(object, loc))  
        {  
            UF_CALL(UF_OBJ_set_name(object, "X"));  
            UF_CALL(UF_OBJ_ask_name_origin(object, loc));  
            UF_CALL(UF_OBJ_delete_name(object));  
        }  
        display_temporary_text(msg, loc);  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:如果找不到， */  
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
    static void report_object_data(tag_t object)  
    {  
        logical  
            is_displayable;  
        int  
            subtype,  
            status,  
            type;  
        tag_t  
            group;  
        char  
            blanked_flag[4] = { "N/A" },  
            color_name[21] = { "N/A" },  
            *full_color_name = { "N/A" },  
            font_name[15] = { "N/A" },  
            group_name[MAX_ENTITY_NAME_SIZE+1],  
            msg[UF_UI_MAX_STRING_LEN+1],  
            object_name[MAX_ENTITY_NAME_SIZE+1],  
            type_desc[MAX_LINE_SIZE+1],  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        double  
            clr_values[3];  
        UF_OBJ_disp_props_t  
            disp_props;  
        sprintf(msg, "EID=%d", object);  
        flag_object(object, msg);  
        ECHO("\n%-36s  %-20s  %-15s  %-7s  %-30s  %-30s  %-30s\n",  
            "type", "color", "font", "blanked", "view dependency", "name",  
            "member of group");  
        ECHO("%-36s  %-20s  %-15s  %-7s  %-30s  %-30s  %-30s\n",  
            "------------------------------------",  
            "--------------------", "---------------", "-------",  
            "------------------------------", "------------------------------",  
            "------------------------------" );  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ask_object_type_descriptor(type, subtype, type_desc);  
        UF_CALL(UF_OBJ_is_displayable(object));  
        if (is_displayable)  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
            ask_color_name(disp_props.color, color_name);  
            ask_object_font_descriptor(disp_props.font, font_name);  
            if (disp_props.blank_status == UF_OBJ_BLANKED)  
                strcpy(blanked_flag, "Yes");  
            else  
                strcpy(blanked_flag, "No");  
        }  
        uc6409(object, &status, view_name);  
        if (status == 0) strcpy(view_name, "n/a");  
        strcpy(object_name, "");  
        UF_OBJ_ask_name(object, object_name);  
        UF_CALL(UF_GROUP_ask_group_of_tag(object, &group));  
        if (group == NULL_TAG)  
            strcpy(group_name, "no");  
        else  
            if (UF_OBJ_ask_name(group, group_name))  
                strcpy(group_name, "yes (unnamed)");  
        ECHO("%-36s  %-20s  %-15s  %-7s  %-30s  %-30s  %-30s\n",  
            type_desc, color_name, font_name, blanked_flag, view_name,  
            object_name, group_name);  
    }

```

#### 代码解析

> 这段代码是用于NX CAD系统的二次开发代码，其主要功能是报告选中对象的数据信息。
>
> 主要功能包括：
>
> 1. 标记对象：使用flag_object函数在对象中心位置显示临时文本，标记出选中对象。
> 2. 报告对象数据：通过report_object_data函数，获取选中对象的各种属性信息，如类型、颜色、字体、是否隐藏等，并打印输出。
> 3. 类型描述：使用ask_object_type_descriptor函数，根据对象类型和子类型获取描述字符串。
> 4. 字体描述：通过ask_object_font_descriptor函数，根据字体编码获取字体名称。
> 5. 颜色名称：通过ask_color_name函数，根据颜色编码获取颜色名称。
> 6. 视图依赖关系：通过uc6409函数，获取对象是否依赖于某个视图。
> 7. 所属组：通过UF_GROUP_ask_group_of_tag函数，获取对象所属的组信息。
> 8. 名称：通过UF_OBJ_ask_name函数，获取对象名称。
> 9. 显示属性：通过UF_OBJ_ask_display_properties函数，获取对象的显示属性，如颜色、字体、是否隐藏等。
>
> 这段代码通过获取对象的各种属性信息，并打印输出，以帮助用户了解选中对象的具体情况。
>
