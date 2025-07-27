### 【2352】report selected object array info 报告选定对象数组信息

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_OBJECT_ARRAY_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_group.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，翻译如下：

注意：UF_print_syslog是V18版本新增的功能。 */  
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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话，就只回答翻译。 */  
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
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *objects;  
        while ((n = select_objects("Report info", &objects)) > 0)  
        {  
            report_object_array(n, objects);  
            UF_free(objects);  
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

> 这段代码是一个NX的二次开发示例，主要功能是选择并报告所选对象的信息。
>
> 关键功能包括：
>
> 1. 错误报告函数：使用宏定义UF_CALL来包装UF函数调用，并在出错时输出错误信息。
> 2. 选择对象：使用UF_UI_select_with_class_dialog函数弹出选择对话框，用户可以输入提示并选择对象，返回对象的tag数组。
> 3. 报告对象信息：包括对象的类型、图层、颜色、字体、是否隐藏、视图依赖关系、名称、所属组等信息，并使用UF_UI_open_listing_window函数在列表窗口中显示。
> 4. 主函数：do_it函数负责循环调用选择对象，并报告所选对象的信息。
> 5. ufusr函数：二次开发程序的入口函数，初始化后调用主函数，并在结束时终止。
> 6. 卸载函数：ufusr_ask_unload函数返回立即卸载标志。
>
> 总体来说，这段代码展示了如何使用NX的UF接口进行交互式选择、对象属性查询、错误处理、以及窗口输出，为NX二次开发提供了很好的示例。
>
