### 【2567】select a drawing using an option menu 选择使用下拉菜单选择一个绘图

#### 代码

```cpp
    /*HEAD SELECT_A_DRAWING_USING_AN_OPTION_MENU CCC UFUN */  
    /*  To test this example - you must create a simple dialog:  
        1.  change CHANGE to GTAC  
        2.  assign the constructor callback "const_cb"  
        3.  add an option menu  
        2.  assign its Activate callback as "select_cb"  
        5.  Save the dialog to your application directory as gtac.dlg  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
    #define GTAC_OPTION_0                  ("OPTION_0")  
    #define GTAC_CB_COUNT ( 2 + 1 ) /* Add 1 for the terminator 里海译:添加1个终止符 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static int GTAC_const_cb ( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            ii,  
            n_drawings;  
        tag_t  
            *drawings;  
        char  
            **opts;  
        UF_STYLER_item_value_type_t  
            value;  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        if (n_drawings == 0) return (UF_UI_CB_CONTINUE_DIALOG);  
        UF_CALL(allocate_string_array(n_drawings, MAX_ENTITY_NAME_SIZE+1,  
            (void ***) &opts));  
        for (ii = 0; ii < n_drawings; ii++)  
            UF_CALL(UF_OBJ_ask_name(drawings[ii], opts[ii]));  
        UF_free(drawings);  
        value.item_attr = UF_STYLER_SUBITEM_VALUES;  
        value.item_id = GTAC_OPTION_0;  
        value.count = n_drawings;  
        value.value.strings = opts;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &value));  
        for (ii = 0; ii < n_drawings; ii++)  
            if (!strcmp(opts[ii], (char *)client_data))  
            {  
                value.item_attr = UF_STYLER_VALUE;  
                value.subitem_index = ii;  
                UF_CALL(UF_STYLER_set_value(dialog_id, &value));  
                break;  
            }  
        if (ii == n_drawings) strcpy((char *)client_data, opts[0]);  
        UF_free_string_array(n_drawings, opts);  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_select_cb ( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            count = 0;  
        UF_STYLER_item_value_type_t  
            values[2];  
        values[0].item_attr = UF_STYLER_SUBITEM_VALUES;  
        values[0].item_id = GTAC_OPTION_0;  
        values[1].item_attr = UF_STYLER_VALUE;  
        values[1].item_id = GTAC_OPTION_0;  
        UF_CALL(UF_STYLER_ask_values(dialog_id, 2, values, &count));  
        strcpy((char *)client_data,  
            values[0].value.strings[values[1].value.integer]);  
        UF_CALL(UF_STYLER_free_value(&values[0]));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        UF_STYLER_callback_info_t  
            GTAC_cbs[GTAC_CB_COUNT] = {  
              {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
              {GTAC_OPTION_0, UF_STYLER_ACTIVATE_CB, 0, GTAC_select_cb},  
              {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        while (!UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs,  
            GTAC_CB_COUNT, dname, &resp)) && (resp != UF_UI_CANCEL))  
        {  
            WRITE_S(dname);  
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

> 根据代码，这是一段用于在NX中创建对话框并选择绘图文件的功能的二次开发代码。
>
> 主要功能包括：
>
> 1. 在对话框中动态获取当前打开的绘图文件，并显示在选项菜单中供用户选择。
> 2. 用户选择某个绘图文件后，将该绘图文件的名字保存起来。
> 3. 提供了错误报告和调试输出功能。
> 4. 使用NX提供的UF_STYLER接口创建对话框，并通过回调函数实现动态获取绘图文件列表和保存用户选择的绘图文件的功能。
> 5. 在主函数中循环调用对话框，直到用户点击确定或取消按钮。
> 6. 使用宏定义简化错误报告和调试输出。
> 7. 提供了内存分配和释放函数。
> 8. 实现了对话框的初始化和选择回调函数。
> 9. 在NX的ufusr函数中调用主函数。
> 10. 提供了卸载函数。
>
> 总体来说，这段代码实现了在NX中创建对话框并选择绘图文件的功能，具有错误处理和内存管理的考虑，以及使用NX提供的二次开发接口实现。
>
