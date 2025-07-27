### 【2652】select drawing member view using uistyler 使用UIStyler选择绘图成员视图的示例代码

#### 代码

```cpp
    /*HEAD SELECT_DRAWING_MEMBER_VIEW_USING_UISTYLER CCC UFUN */  
    /*  To test this example - you must create a simple dialog:  
        1.  Change CHANGE to GTAC  
        2.  Turn off Initially Sensitive for Back and Apply  
        3.  On the Callbacks tab  
                - remove the default apply callback "apply_cb"  
                - assign the constructor callback "const_cb"  
                - assign the OK callback "pick_cb"  
                - assign the cancel callback "cancel_cb"  
        4.  On the Selection tab - turn on Enable and Single Position  
        5.  Add a Single Select List  
        6.  On the Callbacks tab  
                - assign the Activate callback "act_cb"  
                - and the Double Click callback "pick_cb"  
        7.  Save the dialog to your application directory as select_dwg_view.dlg  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，翻译如下：

注意：UF_print_syslog是V18版本新增的，请只提供翻译，不要添加其他内容。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define GTAC_CB_COUNT ( 5 + 1 ) /* Add 1 for the terminator 里海译:为终结者加1 */  
    #define GTAC_LIST_0                    ("LIST_0")  
    static void set_member_view_highlight(tag_t member_view, int status)  
    {  
        double  
            corners[4],  
            end[3],  
            start[3];  
        UF_OBJ_disp_props_t  
            props[2] = {{ 0, 0, 0, 0, 0, 0}, { 0, UF_OBJ_CYAN, 0, 0, 0, 0}};  
        if (UF_DRAW_ask_view_borders(member_view, corners)) return;  
        start[0] = corners[0];  
        start[1] = corners[1];  
        start[2] = 0;  
        end[0] = corners[2];  
        end[1] = corners[1];  
        end[2] = 0;  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props[status]));  
        start[0] = corners[2];  
        start[1] = corners[1];  
        start[2] = 0;  
        end[0] = corners[2];  
        end[1] = corners[3];  
        end[2] = 0;  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props[status]));  
        start[0] = corners[2];  
        start[1] = corners[3];  
        start[2] = 0;  
        end[0] = corners[0];  
        end[1] = corners[3];  
        end[2] = 0;  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props[status]));  
        start[0] = corners[0];  
        start[1] = corners[3];  
        start[2] = 0;  
        end[0] = corners[0];  
        end[1] = corners[1];  
        end[2] = 0;  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props[status]));  
    }  
    /* qq3123197280 */  
    static int sel_cb(int num_sel, tag_p_t sel_objects,  
        int num_deselected, tag_p_t deselected_objs,  void *user_data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        tag_t  
            *member_view = (tag_p_t)user_data,  
            view;  
        double  
            pos[3];  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_STYLER_dialog_id_t  
            dialog_id;  
        UF_STYLER_item_value_type_t  
            value;  
        UF_CALL(UF_UI_ask_sel_cursor_pos(select, &view, pos));  
        UF_CALL(UF_VIEW_ask_type(view, &type, &subtype));  
        if (type == UF_VIEW_DRAWING_MEMBER_TYPE)  
        {  
            if (*member_view != NULL_TAG)  
                set_member_view_highlight(*member_view, FALSE);  
            *member_view = view;  
            set_member_view_highlight(*member_view, TRUE);  
            UF_CALL(UF_STYLER_ask_select_dialog_id(select, &dialog_id));  
            value.item_attr = UF_STYLER_SUBITEM_VALUES;  
            value.item_id = GTAC_LIST_0;  
            UF_STYLER_ask_value(dialog_id, &value);  
            UF_CALL(UF_OBJ_ask_name(view, view_name));  
            for (ii = 0; ii < value.count; ii++)  
            {  
                if (!strcmp(value.value.strings[ii], view_name))  
                {  
                    UF_CALL(UF_STYLER_free_value(&value));  
                    value.item_id = GTAC_LIST_0;  
                    value.item_attr = UF_STYLER_VALUE;  
                    value.subitem_index = ii;  
                    UF_CALL(UF_STYLER_set_value(dialog_id, &value));  
                    break;  
                }  
            }  
        }  
        return( UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static int allocate_string_array(int n_elements, int n_chars, void ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
    /* qq3123197280 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            ii;  
        tag_t  
            dwg,  
            *views;  
        UF_STYLER_item_value_type_t  
            value;  
        value.item_id = UF_STYLER_DIALOG_INDEX;  
        value.item_attr  = UF_STYLER_SELECTION;  
        UF_STYLER_ask_value(dialog_id, &value);  
        if (UF_CALL(UF_DRAW_ask_current_drawing(&dwg)))  
        {  
            UF_CALL(UF_UI_set_sel_type(value.value.selection,  
                UF_UI_SEL_TYPE_INACTIVE_SELECTION));  
            return (UF_UI_CB_CONTINUE_DIALOG);  
        }  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_set_sel_procs(value.value.selection, NULL, sel_cb,  
            client_data));  
        UF_CALL(UF_DRAW_ask_views(dwg, &value.count, &views));  
        if (value.count > 0)  
        {  
            value.item_attr = UF_STYLER_SUBITEM_VALUES;  
            value.item_id = GTAC_LIST_0;  
            UF_CALL(allocate_string_array(value.count, MAX_ENTITY_NAME_SIZE+1,  
                (void ***)&value.value.strings));  
            for (ii = 0; ii < value.count; ii++)  
                UF_CALL(UF_OBJ_ask_name(views[ii], value.value.strings[ii]));  
            UF_free(views);  
            UF_CALL(UF_STYLER_set_value(dialog_id, &value));  
            UF_free_string_array(value.count, value.value.strings);  
        }  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_act_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        tag_t  
            *view = (tag_p_t)client_data;  
        UF_STYLER_item_value_type_t  
            value;  
        if (*view != NULL_TAG) set_member_view_highlight(*view, FALSE);  
        value.item_attr  = UF_STYLER_VALUE;  
        value.item_id = GTAC_LIST_0;  
        value.indicator  = UF_STYLER_STRING_VALUE;  
        UF_STYLER_ask_value(dialog_id, &value);  
        UF_CALL(UF_VIEW_ask_tag_of_view_name(value.value.string, view));  
        set_member_view_highlight(*view, TRUE);  
        UF_CALL(UF_STYLER_free_value(&value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_pick_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        tag_t  
            *view = (tag_p_t)client_data;  
        UF_STYLER_item_value_type_t  
            value;  
        if (*view != NULL_TAG) set_member_view_highlight(*view, FALSE);  
        value.item_attr  = UF_STYLER_VALUE;  
        value.item_id = GTAC_LIST_0;  
        value.indicator  = UF_STYLER_STRING_VALUE;  
        UF_STYLER_ask_value(dialog_id, &value);  
        if (value.count == 0)  
            *view = NULL_TAG;  
        else  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(value.value.string, view));  
        UF_CALL(UF_STYLER_free_value(&value));  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_cancel_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        tag_t  
            *view = (tag_p_t)client_data;  
        if (*view != NULL_TAG) set_member_view_highlight(*view, FALSE);  
        *view = NULL_TAG;  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    static tag_t select_drawing_member_view(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            view = NULL_TAG;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[GTAC_CB_COUNT] =  
            {  
             {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB  , 0, GTAC_const_cb},  
             {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB           , 0, GTAC_pick_cb},  
             {UF_STYLER_DIALOG_INDEX, UF_STYLER_CANCEL_CB      , 0, GTAC_cancel_cb},  
             {GTAC_LIST_0           , UF_STYLER_ACTIVATE_CB     , 0, GTAC_act_cb},  
             {GTAC_LIST_0           , UF_STYLER_DOUBLE_CLICK_CB , 0, GTAC_pick_cb},  
             {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
            };  
        UF_CALL(UF_UI_set_prompt(prompt));  
        UF_CALL(UF_STYLER_create_dialog("select_dwg_view.dlg", GTAC_cbs,  
            GTAC_CB_COUNT, &view, &resp));  
        return view;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            member_view = NULL_TAG;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        while ((member_view = select_drawing_member_view("Pick member view"))  
            != NULL_TAG)  
        {  
            WRITE_D(member_view);  
            UF_CALL(UF_OBJ_ask_name(member_view, view_name));  
            WRITE_S(view_name);  
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

> 这是一段用于在NX中通过UIStyler对话框选择图纸成员视图的二次开发代码。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息。
> 2. 定义了一些辅助函数，如write_integer_to_listing_window和write_string_to_listing_window，用于向列表窗口写入整型和字符串信息。
> 3. 定义了一个高亮显示图纸成员视图边界的函数set_member_view_highlight。
> 4. 定义了回调函数sel_cb，当用户选择一个视图时，取消上一个视图的高亮，并高亮新选择的视图。
> 5. 定义了构造函数回调GTAC_const_cb，获取当前图纸的所有视图并初始化对话框列表。
> 6. 定义了激活回调GTAC_act_cb，根据列表选择高亮对应的视图。
> 7. 定义了确定回调GTAC_pick_cb，获取列表选择的视图，并结束对话框。
> 8. 定义了取消回调GTAC_cancel_cb，取消当前视图的高亮，并结束对话框。
> 9. 定义了选择图纸成员视图的函数select_drawing_member_view，创建对话框并获取用户选择的视图。
> 10. 定义了主函数do_it，循环调用select_drawing_member_view，直到用户选择结束。
> 11. 定义了ufusr函数，初始化NX，调用do_it，然后终止NX。
> 12. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 该代码实现了在NX中通过UIStyler对话框选择和获取图纸成员视图的功能。
>
