### 【2890】simulate edit leader menu 模拟编辑引线菜单

#### 代码

```cpp
    /*HEAD SIMULATE_EDIT_LEADER_MENU CCC UFUN */  
    /*  
        1.  Change CHANGE to GTAC  
        2.  On the Callbacks tab - assign the callbacks:  
               apply        -> "apply_cb"  
               cancel       -> "cancel_cb"  
               constructor  -> "const_cb"  
               ok           -> "ok_cb"  
        3.  Enable selection on the Selection tab  
        4.  Set the type to Single Selection  
        5.  Add a Radio Box  
        6.  Set the Radio Box items "Add" , "Remove", and "Edit"  
        7.  Set the Radio Box's Value Changed callback to "radio_cb"  
        8.  Save the dialog to your application directory as gtac.dlg  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中的新功能，用于在用户函数中打印系统日志。具体来说，UF_print_syslog 接受一个字符串参数，该字符串会被发送到系统日志。此外，这个函数在 UF 处理流程结束后才会执行，因此它不会影响 UF 的执行。总的来说，UF_print_syslog 是一个在用户函数中打印系统日志的新功能。 */  
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
    #define GTAC_RADIO_0                   ("RADIO_0")  
    #define GTAC_CB_COUNT ( 5 + 1 ) /* Add 1 for the terminator 里海译:在终结符后加1。 */  
    #define ADD 0  
    #define REMOVE 1  
    #define EDIT 2  
    struct leader_data_s  
    {  
        tag_t  
            object,  
            leader_object,  
            leader_view;  
        double  
            leader_pos[3];  
        int  
            action;  
    };  
    typedef struct leader_data_s leader_data_t, *leader_data_p_t;  
    static void init_leader_data(leader_data_p_t some_data)  
    {  
        double  
            zero[3] = { 0, 0, 0 };  
        some_data->object = NULL_TAG;  
        some_data->leader_object = NULL_TAG;  
        some_data->leader_view = NULL_TAG;  
        UF_VEC3_copy(zero, some_data->leader_pos);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void reset_selection(int dialog_id)  
    {  
        int  
            n;  
        tag_t  
            *objs;  
        UF_STYLER_item_value_type_t  
            value;  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_id_symbol_subtype, 0 };  
        value.item_id = UF_STYLER_DIALOG_INDEX;  
        value.item_attr = UF_STYLER_SELECTION;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &value));  
        UF_CALL(UF_UI_ask_sel_object_list(value.value.selection, &n, &objs));  
        if (n > 0)  
        {  
            UF_CALL(UF_UI_remove_from_sel_list(value.value.selection,n,objs,TRUE));  
            UF_free(objs);  
        }  
        UF_CALL(UF_UI_set_sel_mask(value.value.selection,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask));  
        UF_CALL(UF_UI_set_sel_type(value.value.selection,  
            UF_UI_SEL_TYPE_SINGLE_SELECTION));  
    }  
    /* qq3123197280 */  
    static int sel_cb ( int num_sel, tag_p_t sel_objects,  
        int num_deselected, tag_p_t deselected_objs,  void *user_data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            descriptor;  
        leader_data_p_t  
            sel_data = (leader_data_p_t) user_data;  
        UF_UI_mask_t  
            mask[7] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, UF_all_subtype },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_solid_silhouette_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        UF_OBJ_disp_props_t  
            attrib = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        switch (sel_data->action)  
        {  
            case ADD:  
                if (sel_data->object == NULL_TAG)  
                {  
                    sel_data->object = sel_objects[0];  
                    UF_CALL(UF_UI_remove_from_sel_list(select,1,sel_objects,FALSE));  
                    UF_CALL(UF_UI_set_sel_mask(select,  
                        UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 7, mask));  
                    UF_CALL(UF_UI_set_sel_type(select,  
                        UF_UI_SEL_TYPE_SINGLE_SELECTION |  
                        UF_UI_SEL_TYPE_SINGLE_POSITION));  
                    UF_CALL(UF_UI_set_cursor_view(0));  
                    UF_CALL(UF_UI_set_prompt("Select leader end point"));  
                }  
                else  
                {  
                    UF_CALL(UF_UI_ask_sel_cursor_pos(select,  
                        &sel_data->leader_view, sel_data->leader_pos));  
                    UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
                        UF_DISP_USE_ACTIVE_PLUS, sel_data->leader_pos, &attrib,  
                        UF_DISP_ASTERISK));  
                    UF_CALL(UF_UI_ask_sel_descriptor(select, &descriptor));  
                    if (descriptor & UF_UI_SEL_DESC_SINGLE)  
                        sel_data->leader_object = sel_objects[0];  
                    UF_CALL(UF_UI_set_sel_type(select,  
                        UF_UI_SEL_TYPE_INACTIVE_SELECTION));  
                    UF_CALL(UF_UI_set_prompt("Apply to create new leader"));  
                }  
                break;  
            case REMOVE:  
                    sel_data->object = sel_objects[0];  
                    UF_CALL(UF_UI_set_sel_type(select,  
                        UF_UI_SEL_TYPE_INACTIVE_SELECTION));  
                    UF_CALL(UF_UI_set_prompt("Apply to remove leader"));  
                break;  
            case EDIT:  
                    sel_data->object = sel_objects[0];  
                    UF_CALL(UF_UI_set_sel_type(select,  
                        UF_UI_SEL_TYPE_INACTIVE_SELECTION));  
                    UF_CALL(UF_UI_set_prompt("Apply to edit leader"));  
                break;  
            default:  
                WRITE("Huh? - ");  
                WRITE_D(sel_data->action);  
                break;  
        }  
        return( UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static int GTAC_radio_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        leader_data_p_t  
            radio_data = (leader_data_p_t) client_data;  
        radio_data->action = callback_data->value.integer;  
        if (radio_data->object != NULL_TAG)  
            UF_CALL(UF_DISP_set_highlight(radio_data->object, FALSE));  
        init_leader_data(radio_data);  
        reset_selection(dialog_id);  
        switch (radio_data->action)  
        {  
            case ADD:  
                UF_CALL(UF_UI_set_prompt("Select id symbol to add leaders"));  
                break;  
            case REMOVE:  
                UF_CALL(UF_UI_set_prompt("Select id symbol to remove leaders"));  
                break;  
            case EDIT:  
                UF_CALL(UF_UI_set_prompt("Select id symbol to edit leaders"));  
                break;  
            default:  
                WRITE("Huh? - ");  
                WRITE_D(radio_data->action);  
                break;  
        }  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        leader_data_p_t  
            const_data = (leader_data_p_t) client_data;  
        UF_STYLER_item_value_type_t  
            value;  
        value.item_id = UF_STYLER_DIALOG_INDEX;  
        value.item_attr  = UF_STYLER_SELECTION;  
        UF_STYLER_ask_value(dialog_id, &value);  
        UF_CALL(UF_UI_set_sel_procs(value.value.selection, NULL, sel_cb,  
            const_data));  
        value.item_id = GTAC_RADIO_0;  
        value.item_attr = UF_STYLER_VALUE;  
        UF_STYLER_ask_value(dialog_id, &value);  
        return (GTAC_radio_cb(dialog_id, client_data, &value));  
    }  
    static void change_it(leader_data_p_t leader_data)  
    {  
        if (leader_data->object == NULL_TAG) return;  
        WRITE("Modifying leader\n");  
        WRITE_D(leader_data->object);  
        WRITE_D(leader_data->leader_object);  
        WRITE_D(leader_data->leader_view);  
        WRITE3F(leader_data->leader_pos);  
        WRITE_D(leader_data->action);  
        if (leader_data->object != NULL_TAG)  
            UF_CALL(UF_DISP_set_highlight(leader_data->object, FALSE));  
        if (leader_data->leader_object != NULL_TAG)  
            UF_CALL(UF_DISP_set_highlight(leader_data->leader_object, FALSE));  
    }  
    /* qq3123197280 */  
    static int GTAC_ok_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        leader_data_p_t  
            apply_data = (leader_data_p_t) client_data;  
        change_it(apply_data);  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_apply_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        leader_data_p_t  
            apply_data = (leader_data_p_t) client_data;  
        change_it(apply_data);  
        init_leader_data(apply_data);  
        reset_selection(dialog_id);  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_cancel_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        leader_data_p_t  
            cancel_data = (leader_data_p_t) client_data;  
        if (cancel_data->object != NULL_TAG)  
            UF_CALL(UF_DISP_set_highlight(cancel_data->object, FALSE));  
        if (cancel_data->leader_object != NULL_TAG)  
            UF_CALL(UF_DISP_set_highlight(cancel_data->leader_object, FALSE));  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        leader_data_t  
            the_data;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[GTAC_CB_COUNT] = {  
         {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB  , 0, GTAC_const_cb},  
         {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB           , 0, GTAC_ok_cb},  
         {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB        , 0, GTAC_apply_cb},  
         {UF_STYLER_DIALOG_INDEX, UF_STYLER_CANCEL_CB       , 0, GTAC_cancel_cb},  
         {GTAC_RADIO_0          , UF_STYLER_VALUE_CHANGED_CB, 0, GTAC_radio_cb},  
         {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        init_leader_data(&the_data);  
        the_data.action = ADD;  
        UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, GTAC_CB_COUNT,  
            &the_data, &resp));  
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

> 这段代码是用于在NX中创建一个对话框，实现添加、删除和编辑标注线的功能。
>
> 主要功能包括：
>
> 1. 对话框创建：使用UF_STYLER创建一个对话框，文件名为gtac.dlg。
> 2. 单选按钮：在对话框中添加单选按钮，用于选择添加、删除或编辑标注线的操作。
> 3. 选择回调：设置选择回调函数sel_cb，用于处理选择标注线或标注线终点时的逻辑。
> 4. 单选按钮回调：设置单选按钮的回调函数GTAC_radio_cb，用于处理单选按钮值变化时的逻辑。
> 5. 确定/应用/取消回调：设置确定、应用和取消按钮的回调函数，用于处理按钮点击时的逻辑。
> 6. 处理标注线逻辑：在回调函数中，根据用户选择的操作，处理标注线的添加、删除和编辑逻辑。
> 7. 主函数：在主函数ufusr中，初始化NX环境，调用do_it函数创建并显示对话框。
> 8. 清理环境：在ufusr_ask_unload中卸载用户函数。
>
> 总体来说，这段代码实现了在NX中创建一个对话框，通过该对话框，用户可以方便地进行添加、删除和编辑标注线的操作。
>
