### 【3123】mask selection for a udo class 定义了一个用户自定义对象(UDO)类A，并为该类设置了一个选择过滤掩码，该掩码仅允许选择属于类A的UDO对象在选择对话框中使

#### 代码

```cpp
    /*HEAD MASK_SELECTION_FOR_A_UDO_CLASS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_exit.h>  
    #include <uf_disp.h>  
    #define ECHO(X)   (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是在 V18 中新增的功能，用于打印系统日志信息。 */  
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
    static UF_UDOBJ_class_t A_class_id;  
    static UF_UDOBJ_class_t B_class_id;  
    DllExport extern UF_UDOBJ_class_t get_A_class_id(void)  
    {  
        return A_class_id;  
    }  
    DllExport extern UF_UDOBJ_class_t get_B_class_id(void)  
    {  
        return B_class_id;  
    }  
    DllExport extern void display_pt_cb(tag_t udo_tag, void *display_context)  
    {  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_UDOBJ_all_data_t  
            udo_data;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UDOBJ_ask_udo_data (udo_tag, &udo_data));  
        UF_CALL(UF_DISP_display_points(udo_data.doubles, 1, UF_DISP_POINT,  
            display_context));  
        UF_CALL(UF_OBJ_ask_name(udo_tag, name));  
        UF_CALL(UF_DISP_display_text(name, udo_data.doubles, UF_DISP_MIDDLECENTER,  
            display_context));  
        UF_CALL(UF_UDOBJ_free_udo_data(&udo_data));  
        UF_terminate();  
    }  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UDOBJ_create_class("A_UDO", "A class UDO", &A_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(A_class_id));  
        UF_CALL(UF_UDOBJ_register_attn_pt_cb(A_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_display_cb(A_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_fit_cb(A_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_select_cb(A_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_create_class("B_UDO", "B class UDO", &B_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(B_class_id));  
        UF_CALL(UF_UDOBJ_register_attn_pt_cb(B_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_display_cb(B_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_fit_cb(B_class_id, display_pt_cb));  
        UF_CALL(UF_UDOBJ_register_select_cb(B_class_id, display_pt_cb));  
        UF_terminate();  
    }  
    static int filter_for_udo_a(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        UF_UDOBJ_class_t  
            this_class_id;  
        UF_UDOBJ_all_data_t  
            udo_data;  
        UF_CALL(UF_UDOBJ_ask_udo_data (object, &udo_data));  
        this_class_id = udo_data.class_id;  
        UF_CALL(UF_UDOBJ_free_udo_data(&udo_data));  
        if (this_class_id == get_A_class_id())  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_for_udo_a(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_user_defined_object_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_for_udo_a, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_udo_a(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_udo_a, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            udo;  
        double  
            loc_a[3] = { 0,0,0 },  
            loc_b[3] = { 1,0,0 };  
        UF_CALL(UF_UDOBJ_create_udo(get_A_class_id(), &udo));  
        UF_CALL(UF_UDOBJ_add_doubles(udo, 3, loc_a));  
        UF_CALL(UF_OBJ_set_name(udo, "UDO_A"));  
        UF_CALL(UF_DISP_add_item_to_display(udo));  
        UF_CALL(UF_UDOBJ_create_udo(get_B_class_id(), &udo));  
        UF_CALL(UF_UDOBJ_add_doubles(udo, 3, loc_b));  
        UF_CALL(UF_OBJ_set_name(udo, "UDO_B"));  
        UF_CALL(UF_DISP_add_item_to_display(udo));  
        while ((udo = select_a_udo_a("Select UDO A")) != NULL_TAG)  
            ECHO(udo);  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了一个自定义的用户定义对象(UDO)类的选择功能。代码的主要功能包括：
>
> 1. 定义了两个UDO类A和B，并分别为它们创建了类ID。
> 2. 为这两个类注册了显示、拟合、选择等回调函数，用于在NX中显示UDO。
> 3. 实现了选择过滤函数，用于在NX中选择操作时只允许选择类A的UDO。
> 4. 实现了选择掩码函数，用于在NX中选择操作时启用UDO类型的选择。
> 5. 实现了选择类A的UDO的函数，该函数会循环提示用户选择类A的UDO，直到用户选择取消。
> 6. 在ufusr函数中，创建了两个UDO实例，一个是类A，一个是类B，并添加到显示中。
> 7. 在do_it函数中，循环调用选择类A的UDO的函数，并打印出选择的UDO的标签。
>
> 总的来说，这段代码实现了自定义UDO类在NX中的创建、显示和选择功能，通过回调函数实现了UDO的自定义显示方式，通过选择过滤和掩码实现了对选择范围的限制，从而实现了选择特定类的UDO实例的功能。
>
