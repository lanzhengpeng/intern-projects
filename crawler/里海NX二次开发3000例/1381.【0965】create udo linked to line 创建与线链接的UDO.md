### 【0965】create udo linked to line 创建与线链接的UDO

#### 代码

```cpp
    /*HEAD CREATE_UDO_LINKED_TO_LINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_exit.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18 */  
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
    static UF_UDOBJ_class_t MY_class_id;  
    DllExport extern UF_UDOBJ_class_t get_MY_class_id(void)  
    {  
        return MY_class_id;  
    }  
    DllExport extern void display_ends_cb(tag_t udo_tag, void *display_context)  
    {  
        int  
            ii;  
        UF_CURVE_line_t  
            coords;  
        UF_DISP_inquire_t  
            display_info;  
        UF_UDOBJ_all_data_t  
            data;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_DISP_ask_display_context(display_context, &display_info));  
        if (display_info.is_draw_open_disp == TRUE) return;  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo_tag, &data));  
        for (ii = 0; ii < data.num_links; ii++)  
        {  
            UF_CALL(UF_CURVE_ask_line_data(data.link_defs[ii].assoc_ug_tag,  
                &coords));  
            UF_CALL(UF_DISP_display_points(coords.start_point, 1, UF_DISP_SQUARE,  
                display_context));  
            UF_CALL(UF_DISP_display_points(coords.end_point, 1, UF_DISP_SQUARE,  
                display_context));  
        }  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    DllExport extern void update_ends_cb(tag_t udo, UF_UDOBJ_link_p_t update_cause)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_DISP_add_item_to_display(udo));  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UDOBJ_create_class("Line_UDO", "Line UDO", &MY_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(MY_class_id));  
        UF_CALL(UF_UDOBJ_register_attn_pt_cb(MY_class_id, display_ends_cb));  
        UF_CALL(UF_UDOBJ_register_display_cb(MY_class_id, display_ends_cb));  
        UF_CALL(UF_UDOBJ_register_fit_cb(MY_class_id, display_ends_cb));  
        UF_CALL(UF_UDOBJ_register_select_cb(MY_class_id, display_ends_cb));  
        UF_CALL(UF_UDOBJ_register_update_cb(MY_class_id, update_ends_cb));  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
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
            my_udo;  
        UF_CURVE_line_t  
            coords;  
        UF_UDOBJ_link_t  
            link_def = { 3, NULL_TAG, FALSE };  
        while ((link_def.assoc_ug_tag =  
            select_a_line("Select line to link to udo")) != NULL_TAG)  
        {  
            UF_CALL(UF_UDOBJ_create_udo(get_MY_class_id(), &my_udo));  
            UF_CALL(UF_UDOBJ_add_links(my_udo, 1, &link_def));  
            UF_CALL(UF_DISP_add_item_to_display(my_udo));  
            uc1601("Linked UDO to line", TRUE);  
            UF_CALL(UF_CURVE_ask_line_data(link_def.assoc_ug_tag, &coords));  
            coords.start_point[1] = coords.start_point[1] + 0.5;  
            coords.end_point[1] = coords.end_point[1] + 0.5;  
            UF_CALL(UF_CURVE_edit_line_data(link_def.assoc_ug_tag, &coords));  
            UF_CALL(UF_MODL_update());  
            uc1601("Editted line", TRUE);  
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

> 这段代码是一个NX Open的二次开发示例，主要功能是创建一个用户定义对象(UDO)，并将其与NX中的线关联起来。代码的主要部分包括：
>
> 1. 错误处理函数：report_error函数用于处理UF函数调用时的错误，并打印错误信息。
> 2. UDO类信息：MY_class_id用于存储创建的UDO类的ID，get_MY_class_id函数用于获取该ID。
> 3. 显示回调函数：display_ends_cb用于在UDO上显示关联线的端点。
> 4. 更新回调函数：update_ends_cb用于在UDO更新时重新显示。
> 5. 初始化函数：ufsta函数用于在启动NX时初始化UDO类，并注册各种回调函数。
> 6. 选择线的函数：mask_for_lines和select_a_line用于选择要关联到UDO的线。
> 7. UDO操作函数：do_it函数用于创建UDO，关联线，并更新线的位置。
> 8. 主函数：ufusr用于调用do_it函数。
>
> 总体来说，该代码通过UDO与NX模型元素建立关联，实现了对模型元素的控制和操作。
>
