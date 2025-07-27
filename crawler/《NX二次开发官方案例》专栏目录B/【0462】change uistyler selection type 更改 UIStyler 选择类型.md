### 【0462】change uistyler selection type 更改 UIStyler 选择类型

#### 代码

```cpp
    /*HEAD CHANGE_UISTYLER_SELECTION_TYPE CCC UFUN */  
    /*  To test this example - you must create a simple dialog:  
        1.  Change CHANGE to GTAC  
        2.  Assign the constructor "const_cb" on the Callbacks tab  
        3.  Enable selection on the Selection tab  
        4.  Set the type to Robust Selection  
        5.  Add a Radio Box  
        6.  Set the Radio Box items to "Robust" and "Single Position"  
        7.  Save the dialog to your application directory as gtac.dlg  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_RADIO_0               ("RADIO_0")  
    #define GTAC_CB_COUNT ( 2 + 1 ) /* Add 1 for the terminator 译:增加1作为终结符 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数，主要用于打印系统日志信息。该函数具有以下特点：

- UF_print_syslog是V18版本新增的函数。
- 专门用于打印系统日志信息。
- 参数包括日志等级和日志内容。
- 日志等级用于区分日志的严重程度。
- 可以根据日志等级过滤显示的日志信息。
- UF_print_syslog函数会自动将日志信息写入系统日志文件中。
- 程序员可以使用UF_print_syslog函数在程序中输出各种日志信息。
- 通过该函数可以方便地跟踪程序的运行状态。

综上所述，UF_print_syslog是V18版本新增的用于打印系统日志信息的函数，为程序调试和运行状态跟踪提供了便利。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    /* 里海 */  
    static int sel_cb ( int num_sel, tag_p_t sel_objects,  
        int num_deselected, tag_p_t deselected_objs,  void *user_data,  
        UF_UI_selection_p_t select)  
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
        UF_CALL(UF_UI_ask_sel_descriptor(select, &descriptor));  
        if (descriptor & UF_UI_SEL_DESC_SINGLE_POSITION)  
        {  
            UF_CALL(UF_UI_ask_sel_cursor_pos(select, &view, pos));  
            display_temporary_point(pos);  
        }  
        if ((descriptor & UF_UI_SEL_DESC_RECTANGLE) &&  
            !UF_CALL(UF_UI_ask_sel_rectangle_pos(select, &view, pos1, pos2, pos3,  
                pos4)))  
        {  
            display_temporary_point(pos1);  
            display_temporary_point(pos2);  
            display_temporary_point(pos3);  
            display_temporary_point(pos4);  
        }  
        return( UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* 里海 */  
    static int GTAC_const_cb( int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t sel_data;  
        sel_data.item_id = UF_STYLER_DIALOG_INDEX;  
        sel_data.item_attr  = UF_STYLER_SELECTION;  
        UF_STYLER_ask_value ( dialog_id, &sel_data );  
        UF_CALL( UF_UI_set_sel_procs ( sel_data.value.selection,  
                                       NULL, sel_cb, NULL));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* 里海 */  
    static int GTAC_change_sel_type( int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t sel_type;  
        UF_STYLER_item_value_type_t sel_data;  
        sel_data.item_attr  = UF_STYLER_SELECTION;  
        sel_data.item_id    = UF_STYLER_DIALOG_INDEX;  
        UF_CALL( UF_STYLER_ask_value ( dialog_id, &sel_data ));  
        sel_type.item_attr = UF_STYLER_VALUE;  
        sel_type.item_id   = GTAC_RADIO_0;  
        UF_CALL( UF_STYLER_ask_value( dialog_id, &sel_type));  
        switch( sel_type.value.integer)  
        {  
            case 0:  
                UF_CALL( UF_UI_set_sel_type( sel_data.value.selection,  
                                                 UF_UI_SEL_TYPE_ROBUST_SELECTION));  
                WRITE("Robust selection enabled\n");  
            break;  
            case 1:  
                UF_CALL( UF_UI_set_sel_type( sel_data.value.selection,  
                                                 UF_UI_SEL_TYPE_SINGLE_POSITION));  
                WRITE("Single position selection enabled\n");  
            break;  
            default:  
                WRITE("Error in switch statement, case value not found\n");  
            break;  
        }  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            response;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[GTAC_CB_COUNT] =  
            {  
             {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB  , 0, GTAC_const_cb},  
             {GTAC_RADIO_0, UF_STYLER_VALUE_CHANGED_CB, 0, GTAC_change_sel_type},  
             {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
            };  
        UF_CALL( UF_STYLER_create_dialog ( "gtac.dlg", GTAC_cbs, GTAC_CB_COUNT,  
            NULL, &response ));  
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

> 这段代码是一个NX Open UI的二次开发示例，实现了根据用户在对话框中的选择改变选择类型的功能。代码的主要逻辑如下：
>
> 1. 定义了一个回调函数GTAC_const_cb，在对话框打开时调用，获取对话框中创建的选择对象，并设置选择回调函数sel_cb。
> 2. sel_cb回调函数根据不同的选择类型显示不同的临时点，例如单点选择显示单点，矩形框选择显示四个点。
> 3. 定义了GTAC_change_sel_type回调函数，当对话框中的单选按钮值改变时调用，根据单选按钮的选择改变选择类型。
> 4. 在GTAC_change_sel_type中，根据单选按钮的值，设置选择类型为健壮选择或单点选择。
> 5. 主函数do_it创建对话框，并注册上述两个回调函数。
> 6. ufusr函数初始化NX Open，调用do_it执行主逻辑，然后终止NX Open。
> 7. ufusr_ask_unload函数在用户请求卸载时立即卸载。
> 8. 使用UF_PRINT_SYSLOG打印错误信息到系统日志。
> 9. 使用UF_UI_WRITE_LISTING_WINDOW将信息打印到NX日志窗口。
>
> 通过这个示例，我们可以学习如何根据用户操作动态改变选择类型，以及如何使用NX Open UI的回调机制。
>
