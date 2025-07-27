### 【0958】create udo and link to multiple selected objects 创建UDO并与多个选定的对象建立链接

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
    #include <uf_view.h>  
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
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_any_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_add_faces_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
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
                case 1:                     /* Back 里海译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:Cancel的中文翻译为“取消”。 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:Picked one，只回答译文，不要废话 */  
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
            center[3],  
            scale;  
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
        UF_CALL(uc6430("", center, &scale));  
        if (UF_CALL(UF_UDOBJ_create_udo(get_my_class_id(), &link_udo))) return;  
        sprintf(name, "UDO_%d", ++ii);  
        UF_CALL(UF_OBJ_set_name(link_udo, name));  
        UF_CALL(UF_OBJ_set_name_origin(link_udo, center));  
        UF_CALL(UF_DISP_add_item_to_display(link_udo));  
        while (((link_def.assoc_ug_tag =  
            select_any_object("Link to udo")) != NULL_TAG) &&  
            ((lt = choose_one_item("Which link type?", link_types, 5)) > -1))  
        {  
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

> 这段NX二次开发代码的主要功能包括：
>
> 1. 自定义UDO类：通过注册自定义UDO类，为NX增加了一种新的用户定义对象类型。
> 2. 交互式选择和链接：通过交互式选择任意NX对象，并将UDO与这些对象建立链接关系，支持多种链接类型，包括拥有链接。
> 3. 更新和删除回调：通过为UDO类注册更新和删除回调函数，当UDO被更新或删除时可以执行自定义操作。
> 4. 可选出现：通过为UDO类注册可选出现函数，使UDO可以出现在装配结构树中。
> 5. 创建UDO实例：通过创建一个UDO实例，为其指定名称和位置，并将其添加到显示列表。
> 6. 链接操作：通过循环选择对象，为UDO添加不同类型的链接，包括拥有链接，并将链接信息输出到日志窗口。
> 7. NX初始化和终止：在UFusr和UFsta函数中，分别进行NX的初始化和终止操作。
>
> 总的来说，这段代码实现了自定义UDO类及其基本操作，为NX新增了一种可交互链接的UDO对象，为NX的二次开发提供了扩展能力。
>
