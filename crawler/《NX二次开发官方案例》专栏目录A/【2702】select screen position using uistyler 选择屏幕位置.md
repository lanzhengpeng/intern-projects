### 【2702】select screen position using uistyler 选择屏幕位置

#### 代码

```cpp
    /*HEAD SELECT_SCREEN_POSITION_USING_UISTYLER CCC UFUN */  
    /*  To create a dialog to go with this sample code:  
        Application-> UIStyler  
        change CHANGE to GTAC  
        enter constructor callback:  GTAC_const_cb  
        Enable Selection  
        Turn on Single Position selection  
        File->Save As->"screen_pos"  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数，它用于打印系统日志消息。 */  
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
    #define GTAC_CB_COUNT ( 1 + 1 ) /* Add 1 for the terminator 里海译:Add 1 for the terminator的翻译是：为终止符加1。 */  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
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
    /* qq3123197280 */  
    static int sel_cb(int num_sel, tag_t *sel_objects, int num_deselected,  
        tag_t *deselected_objs,  void *user_data, UF_UI_selection_p_t select)  
    {  
        tag_t  
            view;  
        double  
            pos[3];  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_UI_ask_sel_cursor_pos(select, &view, pos));  
        attrib.color = UF_OBJ_WHITE;  
        UF_CALL(UF_DISP_display_temporary_point(view,  
            UF_DISP_USE_VIEW_TAG, pos, &attrib, UF_DISP_ASTERISK));  
        UF_VEC3_copy(pos, (double *)user_data);  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            value;  
        value.item_id = UF_STYLER_DIALOG_INDEX;  
        value.item_attr = UF_STYLER_SELECTION;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &value));  
        UF_CALL(UF_UI_set_sel_procs(value.value.selection, NULL, sel_cb,  
            client_data));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            response;  
        double  
            pos[3];  
        UF_STYLER_callback_info_t GTAC_cbs[GTAC_CB_COUNT] = {  
         {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
         {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL(UF_STYLER_create_dialog("screen_pos.dlg", GTAC_cbs,  
            GTAC_CB_COUNT, pos, &response));  
        WRITE3F(pos);  
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

> 这段代码是NX的二次开发代码，主要用于创建一个对话框，并使用UIStyler来选择屏幕上的位置。以下是代码的主要功能和流程：
>
> 1. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义了函数write_double_array_to_listing_window，用于将双精度数组写入列表窗口。
> 3. 定义了选择回调函数sel_cb，用于在选择屏幕位置时，显示一个临时点，并获取该位置的坐标。
> 4. 定义了构造函数回调GTAC_const_cb，用于在创建对话框时，设置选择回调函数。
> 5. 定义了主函数do_it，用于创建对话框，并获取屏幕位置。
> 6. 定义了ufusr函数，用于初始化NX环境，执行主函数，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，用于卸载应用程序。
>
> 通过以上函数和流程，该代码实现了使用UIStyler选择屏幕位置的功能。用户可以运行该代码，选择一个屏幕位置，然后获取该位置的坐标。
>
