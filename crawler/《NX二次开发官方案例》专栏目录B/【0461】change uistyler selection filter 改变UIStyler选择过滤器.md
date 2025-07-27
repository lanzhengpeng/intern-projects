### 【0461】change uistyler selection filter 改变UIStyler选择过滤器

#### 代码

```cpp
    /*HEAD CHANGE_UISTYLER_SELECTION_FILTER CCC UFUN */  
    /*  To test this example - you must create a simple dialog:  
        1.  Change CHANGE to GTAC  
        2.  Assign the constructor "change_sel_cb" on the Callbacks tab  
        3.  Enable selection on the Selection tab  
        4.  Set the type to Single Selection  
        5.  Add a Radio Box  
        6.  Set the Radio Box items to "Solids" and "Sheets"  
        7.  Save the dialog to your application directory as gtac.dlg  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #define GTAC_RADIO_0              ("RADIO_0")  
    #define GTAC_CB_COUNT ( 2 + 1 ) /* Add 1 for the terminator 译:加1代表终结者 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，可以翻译为：

注意：UF_print_syslog是V18版本新增的功能，请只回答有关该功能的译文，不要添加其他无关内容。 */  
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
    /* 里海 */  
    static int sel_cb (int num_sel, tag_p_t sel_objects, int num_deselected,  
        tag_p_t deselected_objs,  void *user_data, UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(sel_objects[0], &body_type));  
        WRITE_D(body_type);  
        UF_CALL(UF_DISP_set_highlight(sel_objects[0], FALSE));  
        return( UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* 里海 */  
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type,  
            *right_type = (int *)data;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *right_type)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* 里海 */  
    static int GTAC_change_sel_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            *sol_types = (int *)client_data;  
        UF_STYLER_item_value_type_t  
            sel_data,  
            sel_type;  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        sel_data.item_id = UF_STYLER_DIALOG_INDEX;  
        sel_data.item_attr  = UF_STYLER_SELECTION;  
        UF_STYLER_ask_value(dialog_id, &sel_data);  
        sel_type.item_attr = UF_STYLER_VALUE;  
        sel_type.item_id   = GTAC_RADIO_0;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &sel_type));  
        UF_CALL(UF_UI_set_sel_mask(sel_data.value.selection,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask));  
        UF_CALL(UF_UI_set_sel_procs(sel_data.value.selection, filter_body_type,  
            sel_cb, &sol_types[sel_type.value.integer]));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            response,  
            sol_types[2] = { UF_MODL_SOLID_BODY, UF_MODL_SHEET_BODY };  
        UF_STYLER_callback_info_t  
            GTAC_cbs[GTAC_CB_COUNT] =  
            {  
                {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0,  
                    GTAC_change_sel_cb },  
                {GTAC_RADIO_0, UF_STYLER_VALUE_CHANGED_CB, 0,  
                    GTAC_change_sel_cb },  
                {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
            };  
        UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, GTAC_CB_COUNT,  
            sol_types, &response));  
    }  
    /* 里海 */  
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

> 这段代码是用于NX二次开发的示例，主要实现以下功能：
>
> 1. 定义了一个GTAC_change_sel_cb函数，它作为构造函数回调，用于在对话框创建时初始化选择过滤器。
> 2. GTAC_change_sel_cb函数会根据单选按钮的值，设置选择过滤器只接受实体类型为solids或sheets的体。
> 3. 定义了一个sel_cb函数，作为选择回调，用于在用户选择一个体后，输出该体的类型并取消高亮。
> 4. 定义了一个filter_body_type函数，作为选择过滤器，用于判断用户选择的体是否符合过滤器的要求。
> 5. 在do_it函数中，创建了一个对话框，并设置了构造函数回调GTAC_change_sel_cb和单选按钮值改变回调GTAC_change_sel_cb。
> 6. ufusr函数是NX二次开发的入口函数，在其中调用do_it函数创建对话框。
> 7. ufusr_ask_unload函数用于卸载应用程序。
>
> 总的来说，这段代码通过NX Styler创建了一个对话框，用户可以选择solids或sheets，并设置选择过滤器只接受对应的实体类型。在用户选择一个体后，会输出该体的类型并取消高亮。
>
