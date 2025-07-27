### 【3103】demo uistyler position selection 位置选择功能

#### 代码

```cpp
    /*HEAD DEMO_UISTYLER_POSITION_SELECTION CCC UFUN */  
    /*  
      To test this example - you must create a simple dialog:  
        1.  Change CHANGE to GTAC  
        2.  Assign the constructor "const_cb" on the Callbacks tab  
        3.  Enable selection on the Selection tab  
        4.  Enable both Single Postion and Rectangle Position  
        5.  Save the dialog to your application directory as gtac.dlg  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，我翻译如下：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
    /* qq3123197280 */  
    static int select_cb(int n_sel, tag_t *sel_objects, int n_des,  
        tag_t *des_objects, void *user_data, UF_UI_selection_p_t select)  
    {  
        int  
            descriptor;  
        tag_t  
            view;  
        double  
            pos[3],  
            pos1[3],  
            pos2[3],  
            pos3[3],  
            pos4[3];  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_UI_ask_sel_descriptor(select, &descriptor));  
        if (descriptor & UF_UI_SEL_DESC_SINGLE_POSITION)  
        {  
            UF_CALL(UF_UI_ask_sel_cursor_pos(select, &view, pos));  
            UF_CALL(UF_DISP_display_temporary_point(view, UF_DISP_USE_VIEW_TAG,  
                pos, &attrib, UF_DISP_POINT));  
        }  
        if (descriptor & UF_UI_SEL_DESC_RECTANGLE_POSITION)  
        {  
            UF_CALL(UF_UI_ask_sel_rectangle_pos(select, &view, pos1, pos2, pos3,  
                pos4));  
            UF_CALL(UF_DISP_display_temporary_line(view, UF_DISP_USE_VIEW_TAG,  
                pos1, pos3, &attrib));  
            UF_CALL(UF_DISP_display_temporary_line(view, UF_DISP_USE_VIEW_TAG,  
                pos3, pos2, &attrib));  
            UF_CALL(UF_DISP_display_temporary_line(view, UF_DISP_USE_VIEW_TAG,  
                pos2, pos4, &attrib));  
            UF_CALL(UF_DISP_display_temporary_line(view, UF_DISP_USE_VIEW_TAG,  
                pos4, pos1, &attrib));  
        }  
        return( UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            value;  
        value.item_id = UF_STYLER_DIALOG_INDEX;  
        value.item_attr  = UF_STYLER_SELECTION;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &value));  
        UF_CALL(UF_UI_set_sel_procs(value.value.selection, NULL, select_cb, NULL));  
        UF_CALL(UF_STYLER_free_value(&value ));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[2] = {  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
     {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL(UF_STYLER_create_dialog("gtac", GTAC_cbs, 2, NULL, &resp));  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了选择回调函数select_cb，用于在用户选择操作时执行。该函数根据选择描述符判断用户选择了单点选择还是矩形选择，并在视图中绘制对应的临时几何体。
> 3. 定义了构造回调函数GTAC_const_cb，用于在对话框构造时执行。该函数获取对话框的选择项，并设置选择回调函数select_cb。
> 4. 定义了主函数do_it，用于创建对话框并显示。
> 5. 定义了ufusr函数，作为NX二次开发的入口函数，用于初始化环境，调用主函数，并终止环境。
> 6. 定义了ufusr_ask_unload函数，用于立即卸载二次开发应用程序。
>
> 总体来说，这段代码实现了在NX中选择操作时绘制临时几何体的功能，通过回调函数的方式在用户选择时获取选择信息并绘制，从而实现了选择操作的交互式效果。
>
