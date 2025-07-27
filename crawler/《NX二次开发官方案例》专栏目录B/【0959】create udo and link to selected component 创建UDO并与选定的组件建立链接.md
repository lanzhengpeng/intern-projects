### 【0959】create udo and link to selected component 创建UDO并与选定的组件建立链接

#### 代码

```cpp
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
    #include <uf_assem.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
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
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
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
                case 1:                     /* Back 里海译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:Cancel */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:More... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:好的，我会翻译“Picked one”。 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static void ask_component_origin(tag_t component, double origin[3])  
    {  
        double  
            csys_matrix[9],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
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
            *link_types[4] = {  
                "Link type 1",  
                "Link type 2",  
                "Link type 3",  
                "Link type 4"},  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_UDOBJ_link_t  
            link_def = { 1, NULL_TAG, FALSE };  
        if (!get_my_class_id()) register_udo_class();  
        if (UF_PART_ask_display_part() == NULL_TAG) return;  
        while (((link_def.assoc_ug_tag =  
            select_a_component("Link to udo")) != NULL_TAG) &&  
            ((lt = choose_one_item("Which link type?", link_types, 4)) > -1))  
        {  
            ask_component_origin(link_def.assoc_ug_tag, pos);  
            UF_CALL(UF_UDOBJ_create_udo(get_my_class_id(), &link_udo));  
            sprintf(name, "UDO_%d", ++ii);  
            UF_CALL(UF_OBJ_set_name(link_udo, name));  
            UF_CALL(UF_OBJ_set_name_origin(link_udo, pos));  
            WRITE_D(link_udo);  
            link_def.link_type = lt+1;  
            ECHO("Component:\n");  
            UF_CALL(UF_UDOBJ_add_links(link_udo, 1, &link_def));  
            WRITE_D(link_def.assoc_ug_tag);  
            WRITE_D(link_def.link_type);  
            ECHO("Instance:\n");  
            link_def.assoc_ug_tag = UF_ASSEM_ask_inst_of_part_occ(link_def.assoc_ug_tag);  
            UF_CALL(UF_UDOBJ_add_links(link_udo, 1, &link_def));  
            WRITE_D(link_def.assoc_ug_tag);  
            WRITE_D(link_def.link_type);  
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

> 这段代码是用于NX的二次开发，其主要功能是创建自定义用户定义对象（UDO）并将其链接到组件。下面是代码的主要功能和流程：
>
> 1. 初始化：首先定义了一些辅助函数，用于输出信息、报告错误和注册UDO类。
> 2. UDO类的注册：通过调用UF_UDOBJ_create_class函数来创建名为"MY_test"的UDO类，并注册了更新、删除和判断是否是出现的回调函数。
> 3. 用户选择组件：定义了一个函数select_a_component，用于让用户选择一个组件，并返回其tag。
> 4. 组件信息获取：定义了一个函数ask_component_origin，用于获取组件的原点信息。
> 5. 创建UDO并链接组件：在函数do_it中，循环让用户选择组件和链接类型，然后创建UDO，设置其名称和原点，并使用UF_UDOBJ_add_links将UDO链接到组件和组件实例。
> 6. 显示UDO：调用UF_DISP_add_item_to_display将UDO添加到显示中。
> 7. API的封装：通过ufsta和ufusr函数封装了NX的API，使代码可以在NX中直接运行。
>
> 总体来说，这段代码实现了在NX中创建UDO并与组件进行链接的功能，具有一定的参考价值。
>
