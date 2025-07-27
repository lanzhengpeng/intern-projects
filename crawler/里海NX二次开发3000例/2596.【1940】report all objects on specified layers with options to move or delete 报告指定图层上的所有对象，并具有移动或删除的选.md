### 【1940】report all objects on specified layers with options to move or delete 报告指定图层上的所有对象，并具有移动或删除的选

#### 代码

```cpp
    /*HEAD REPORT_ALL_OBJECTS_ON_SPECIFIED_LAYERS_WITH_OPTIONS_TO_MOVE_OR_DELETE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_layer.h>  
    #include <uf_modl.h>  
    #include <uf_cfi.h>  
    #include <uf_object_types.h>  
    #include <uf_view.h>  
    #include <uf_group.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 新增的功能，用于打印系统日志。 */  
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
    #ifndef UF_LAYER_MAX_LAYER  
        #define UF_LAYER_MAX_LAYER 256  
    #endif  
    static logical prompt_for_a_layer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            laymsg[100],  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            if ((*number > UF_LAYER_MAX_LAYER) || (*number <= 0))  
            {  
                sprintf(laymsg, "Layers range from 1 to %d", UF_LAYER_MAX_LAYER);  
                uc1601(laymsg, TRUE);  
                return prompt_for_a_layer(prompt, item, number);  
            }  
            return TRUE;  
        }  
        else return FALSE;  
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
    static int ask_all_objects_on_layer(int layer, tag_t **objects)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(layer, &object)) &&  
            (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if ((type != UF_solid_type) || (subtype == UF_solid_body_subtype))  
            {  
                UF_CALL(UF_MODL_put_list_item(list, object));  
            }  
        }  
        return make_an_array(&list, objects);  
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
            case UF_circle_type: /* non-std format in the header file 里海译:非标准格式在头文件中 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:万一找不到的话 */  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical prompt_for_two_integers(char *prompt, char *item1, char *item2,  
        int *number1, int *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        int  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1607(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical prompt_for_three_integers(char *prompt, char *item1, char *item2,  
        char *item3, int *number1, int *number2, int *number3)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[3][16];  
        int  
            da[3];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        strncpy(&menu[2][0], item3, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        menu[2][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        resp = uc1607(prompt, menu, 3, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            layer,  
            n,  
            n_objects,  
            new_layer,  
            resp = 0,  
            start,  
            *statuses,  
            end;  
        tag_t  
            *objects;  
        char  
            err[MAX_LINE_SIZE+1],  
            menu[6][38] = { "Move single items to new layer",  
                           "Move range of items to new layer",  
                           "Move all items to new layer",  
                           "Delete single items",  
                           "Delete range of items",  
                           "Delete all items" },  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_LAYER_ask_work_layer(&layer));  
        while (prompt_for_a_layer("Check layer", "layer", &layer))  
        {  
            UF_CALL(UF_UI_exit_listing_window());  
            UF_CALL(UF_UI_open_listing_window());  
            sprintf(msg, "Layer %d: ", layer);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            n_objects = ask_all_objects_on_layer(layer, &objects);  
            sprintf(msg, "%d objects\n", n_objects);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            if (n_objects > 0)  
            {  
                report_object_array(n_objects, objects);  
                while ((resp = uc1603("Select Move or Delete option",  
                    resp, menu, 6)) > 2)  
                {  
                    resp = resp - 4;  
                    if (resp == 15)  
                    {  
                        uc1601("Cannot post menu", TRUE);  
                        break;  
                    }  
                    if (resp == 1) /* Move single items to new layer 里海译:移动单个项目到新图层 */  
                    {  
                        new_layer = layer;  
                        n = 1;  
                        while (prompt_for_two_integers("Move one item",  
                            "Item No.", "New Layer", &n, &new_layer))  
                            if ((n > 0) && (n <= n_objects) && (new_layer > 0) &&  
                                (new_layer <= UF_LAYER_MAX_LAYER))  
                                UF_CALL(UF_OBJ_set_layer(objects[n-1], new_layer));  
                            else uc1601("Invalid response", TRUE);  
                    }  
                    if (resp == 2) /* Move range of items to new layer 里海译:移动项目范围到新图层 */  
                    {  
                        start = 1;  
                        end = n_objects;  
                        new_layer = layer;  
                        while (prompt_for_three_integers("Move range of items",  
                            "Start No.", "End No.", "New Layer", &start, &end,  
                            &new_layer))  
                            if ((start > 0) && (start < n_objects) && (end > 0)  
                                && (end <= n_objects) && (start <= end) &&  
                                (new_layer > 0) &&  
                                (new_layer <= UF_LAYER_MAX_LAYER))  
                                for (ii = start -1; ii < end; ii++)  
                                    UF_CALL(UF_OBJ_set_layer(objects[ii],  
                                        new_layer));  
                            else uc1601("Invalid response", TRUE);  
                    }  
                    if (resp == 3) /* Move all items to new layer 里海译:移动所有项目到新图层 */  
                    {  
                        new_layer = layer;  
                        if (prompt_for_a_layer("Move all items", "New Layer",  
                            &new_layer))  
                            if ((new_layer > 0) &&  
                                (new_layer <= UF_LAYER_MAX_LAYER))  
                                for (ii = 0; ii < n_objects; ii++)  
                                    UF_CALL(UF_OBJ_set_layer(objects[ii],  
                                        new_layer));  
                            else uc1601("Invalid response", TRUE);  
                        break;  
                    }  
                    if (resp == 4) /* Delete single items 里海译:删除单个项目 */  
                    {  
                        n = 1;  
                        while (prompt_for_an_integer("Delete one item",  
                                    "Item No.", &n))  
                            if ((n > 0) && (n <= n_objects))  
                                UF_CALL(UF_OBJ_delete_object(objects[n-1]));  
                            else uc1601("Invalid response", TRUE);  
                    }  
                    if (resp == 5) /* Delete range of items 里海译:删除项目范围 */  
                    {  
                        start = 1;  
                        end = n_objects;  
                        while (prompt_for_two_integers("Delete range of items",  
                            "Start No.", "End No.", &start, &end))  
                            if ((start > 0) && (start < n_objects) && (end > 0)  
                                && (end <= n_objects) && (start <= end))  
                            {  
                                UF_CALL(UF_OBJ_delete_array_of_objects(  
                                    end - start + 1, &objects[start-1], &statuses));  
                                for (ii = 0; ii < end - start + 1; ii++)  
                                {  
                                    if (statuses[ii] == 0) continue;  
                                    UF_get_fail_message(statuses[ii], err);  
                                    sprintf(msg, "Item No. %d: error %d - %s\n",  
                                        start+ii, statuses[ii], err);  
                                    UF_UI_write_listing_window(msg);  
                                }  
                                UF_free(statuses);  
                            }  
                            else uc1601("Invalid response", TRUE);  
                    }  
                    if (resp == 6) /* Delete all items 里海译:删除所有项目 */  
                    {  
                        UF_CALL(UF_OBJ_delete_array_of_objects(n_objects,  
                            objects, &statuses));  
                        for (ii = 0; ii < n_objects; ii++)  
                        {  
                            if (statuses[ii] == 0) continue;  
                            UF_get_fail_message(statuses[ii], err);  
                            sprintf(msg, "Item No. %d: error %d - %s\n",  
                                ii+1, statuses[ii], err);  
                            UF_UI_write_listing_window(msg);  
                        }  
                        UF_free(statuses);  
                        break;  
                    }  
                }  
                UF_free(objects);  
            }  
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

> 这段NX二次开发代码的功能是报告指定图层上的所有对象，并提供移动或删除这些对象的选项。具体包括以下几个关键部分：
>
> 1. 错误报告：使用宏UF_CALL封装了NX API调用，并在出错时报告错误信息。
> 2. 图层和对象管理：提供了查询指定图层上的所有对象，并将对象列表转换为对象数组的功能。
> 3. 对象显示：可以显示临时文本，用于为对象编号。
> 4. 对象报告：能够报告对象数组，包括类型、图层、颜色、字体、是否隐藏等信息。
> 5. 交互提示：提供了一系列交互提示函数，用于提示输入整数值。
> 6. 主要功能：在主函数do_it中，循环提示用户输入要检查的图层，报告图层上的对象，并根据用户选择执行移动或删除对象的操作。
> 7. 初始化和结束：在ufusr中初始化和终止NX API。
> 8. 卸载处理：提供了询问立即卸载的函数。
>
> 总体来说，这段代码实现了NX环境下对指定图层上的对象进行查询、编号、报告以及提供交互式操作的功能，为用户提供了便捷的对象管理功能。
>
