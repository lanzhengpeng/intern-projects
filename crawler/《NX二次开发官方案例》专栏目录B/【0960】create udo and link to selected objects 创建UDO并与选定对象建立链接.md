### 【0960】create udo and link to selected objects 创建UDO并与选定对象建立链接

#### 代码

```cpp
    /*  
        Note:  In this example, only a UDO with an owning link will be  
        selectable without Preferences-> Visualization-> Names/Borders->  
        Object Name Settings-> Work View turn ON.  Selection works from the  
        display list.  To delete the UDOs with other link types, turn on  
        the name display.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_exit.h>  
    #include <uf_part.h>  
    #include <stdarg.h>  
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
    static UF_UDOBJ_class_t MY_class_id;  
    DllExport extern UF_UDOBJ_class_t get_my_class_id(void)  
    {  
        return MY_class_id;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    DllExport extern void update_func(tag_t udo,  
        UF_UDOBJ_link_p_t update_cause)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        ECHO("UDO update function:\n");  
        WRITE_D(udo);  
        WRITE_D(update_cause->assoc_ug_tag);  
        WRITE_D(update_cause->link_type);  
        UF_terminate();  
    }  
    DllExport extern void delete_func(tag_t udo,  
        UF_UDOBJ_link_p_t delete_cause)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        ECHO("UDO delete function:\n");  
        WRITE_D(udo);  
        WRITE_D(delete_cause->assoc_ug_tag);  
        WRITE_D(delete_cause->link_type);  
        UF_terminate();  
    }  
    static void is_occurrenceable_func(tag_t udo_tag, logical *is_occurrenceable)  
    {  
        *is_occurrenceable = TRUE;  
    }  
    static void register_udo_class()  
    {  
        ECHO("registering UDO class...\n");  
        UF_CALL(UF_UDOBJ_create_class("MY_test", "my UDOs", &MY_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(MY_class_id));  
        UF_CALL(UF_UDOBJ_register_update_cb(MY_class_id, update_func));  
        UF_CALL(UF_UDOBJ_register_delete_cb(MY_class_id, delete_func));  
        UF_CALL(UF_UDOBJ_register_is_occurrenceable_cb(MY_class_id,  
            is_occurrenceable_func));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        register_udo_class();  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_user_defined_object_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_object_and_location(char *prompt, double cp[3])  
    {  
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
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 里海译:Back的中文翻译是“背部”。 */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:More... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:好的，以下是 "Picked one" 的翻译：

选择一个 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0,  
            lt;  
        tag_t  
            link_udo;  
        double  
            pos[3];  
        char  
            *link_types[5] = {  
                "Link type 1",  
                "Link type 2",  
                "Link type 3",  
                "Link type 4",  
                "Owning link" },  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_UDOBJ_link_t  
            link_def = { 1, NULL_TAG, FALSE };  
        if (!get_my_class_id()) register_udo_class();  
        if (UF_PART_ask_display_part() == NULL_TAG) return;  
        while (((link_def.assoc_ug_tag =  
            select_an_object_and_location("Link to udo", pos)) != NULL_TAG) &&  
            ((lt = choose_one_item("Which link type?", link_types, 5)) > -1))  
        {  
            UF_CALL(UF_UDOBJ_create_udo(get_my_class_id(), &link_udo));  
            sprintf(name, "UDO_%d", ++ii);  
            UF_CALL(UF_OBJ_set_name(link_udo, name));  
            UF_CALL(UF_OBJ_set_name_origin(link_udo, pos));  
            WRITE_D(link_udo);  
            if (lt < 4)  
            {  
                link_def.link_type = lt+1;  
                UF_CALL(UF_UDOBJ_add_links(link_udo, 1, &link_def));  
                WRITE_D(link_def.assoc_ug_tag);  
                WRITE_D(link_def.link_type);  
            }  
            else  
            {  
                UF_CALL(UF_UDOBJ_add_owning_links(link_udo, 1,  
                    &link_def.assoc_ug_tag));  
                ECHO("owning link to %d\n", link_def.assoc_ug_tag);  
            }  
            UF_CALL(UF_DISP_add_item_to_display(link_udo));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是NX的二次开发示例，实现了以下功能：
>
> 1. 用户自定义对象(UDO)类注册：在NX中注册了一个名为MY_test的UDO类，用于创建自定义的UDO对象。
> 2. 更新和删除回调：为该UDO类注册了更新和删除回调函数，当UDO对象被更新或删除时，会调用这些函数并输出相关信息。
> 3. 选择对象和位置：实现了选择一个对象并获取其位置的功能，可以选择任何实体或UDO对象。
> 4. 创建UDO对象：通过选择一个关联对象和位置，可以创建一个UDO对象，并将其显示在NX中。
> 5. 添加链接：为UDO对象添加不同类型的链接，包括拥有链接，并输出相关信息。
> 6. 交互式选择：通过交互式对话框，用户可以选择链接类型和对象，实现了灵活的UDO对象创建和链接添加功能。
>
> 总的来说，这段代码通过NX的二次开发接口，实现了自定义UDO类的注册、创建、更新、删除等功能，并通过交互式操作提供了便利的UDO对象管理功能。
>
