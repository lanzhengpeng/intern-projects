### 【1645】move all displayable objects from layer to layer 将所有可显示对象从层移动到层

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_layer.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_cfi.h>  
    #include <uf_object_types.h>  
    #include <uf_view.h>  
    #include <uf_group.h>  
    #include <uf_trns.h>  
    #include <uf_undo.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #ifndef UF_LAYER_MAX_LAYER_LAYER  
    #define UF_LAYER_MAX_LAYER_LAYER 512  
    #endif  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
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
    static tag_t ask_displayable_objects_on_layer(int layer, tag_t **objects)  
    {  
        int  
            ii = 0,  
            subtype,  
            type;  
        logical  
            is_transferable;  
        UF_OBJ_disp_props_t  
            disp_props;  
        uf_list_p_t  
            list;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            object = NULL_TAG;  
        UF_CALL(UF_MODL_create_list(&list));  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            UF_CALL(UF_OBJ_is_transferable(object, &is_transferable));  
            if ((UF_OBJ_is_displayable(object))&& is_transferable &&  
                ((type != UF_solid_type) || (subtype == UF_solid_body_subtype)) )  
            {  
                UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
                if ( layer == disp_props.layer)  
                {  
                    ii++;  
                    UF_CALL(UF_MODL_put_list_item(list, object));  
                }  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话。 */  
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
    static void layer_move(tag_t *objects, int n_objects, int to_layer)  
    {  
        int  
            mov = 1,  
            off = 2,  
            resp;  
        double  
            delta_zero[3] = { 0.0, 0.0, 0.0 },  
            mx[16];  
        tag_t  
            *copies;  
        copies = (tag_t *)UF_allocate_memory(n_objects * sizeof (tag_t), &resp);  
        UF_CALL(resp);  
        FTN(uf5943)(delta_zero, mx);  
        FTN(uf5947)(mx, objects, &n_objects, &mov, &to_layer, &off, copies, NULL,  
            &resp);  
        WRITENZ(resp);  
        UF_free(copies);  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        int  
            layer = 258,  
            n_objects,  
            to_layer = 13;  
        tag_t  
            *objects;  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        logical  
            confirm = FALSE;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_LAYER_ask_work_layer(&layer));  
        if ((prompt_for_a_layer("Changing Object Layer", "Original Layer", &layer)) &&  
                (prompt_for_a_layer("Changing Object Layer", "Destination Layer", &to_layer)))  
        {  
            n_objects = ask_displayable_objects_on_layer(layer, &objects);  
            if (n_objects > 0)  
            {  
                UF_CALL(UF_UI_open_listing_window());  
                WRITEnT(n_objects, objects);  
                sprintf(msg, "Move %d object(s) from \nLayer %d to Layer %d",  
                    n_objects, layer, to_layer);  
                confirm = ask_yes_or_no("Confirm Layer Move", msg );  
                if (confirm)  
                {  
                    UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Layer Move", &mark_id));  
                    layer_move(objects, n_objects, to_layer);  
                }  
                UF_free(objects);  
            }  
            else  
            {  
                UF_CALL(UF_UI_exit_listing_window());  
                UF_CALL(UF_UI_open_listing_window());  
                sprintf(msg, "No displayable objects found on Layer %d: ", layer);  
                UF_CALL(UF_UI_write_listing_window(msg));  
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

> 这段NX二次开发代码的主要功能是允许用户选择一个图层，然后将该图层上的所有可显示对象移动到另一个指定的图层。代码的主要步骤如下：
>
> 1. 获取当前工作图层，并提示用户输入原始图层号和目标图层号。
> 2. 遍历原始图层上的所有对象，筛选出可显示的对象，并将它们的标签放入一个数组。
> 3. 打开一个列表窗口，显示这些可显示对象的详细信息，包括类型、图层号、颜色、字体、是否隐藏、视图依赖、名称和所属组等。
> 4. 提示用户确认是否将这些对象移动到目标图层。如果确认，则创建一个撤销标记，并调用函数移动对象到目标图层。
> 5. 释放分配的内存。
>
> 代码中使用了NX提供的各种API函数，如UF_MODL、UF_OBJ、UF_DISP、UF_LAYER、UF_GROUP、UF_UNDO等，实现了对象的遍历、属性查询、显示控制、图层操作等功能。整体流程清晰，通过交互式提示使用户能够选择需要操作的图层和对象，实现了层移动的功能。
>
