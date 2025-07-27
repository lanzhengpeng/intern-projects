### 【0508】change uistyler list contents 更改UIStyler列表内容

#### 代码

```cpp
    /*HEAD CHANGE_UISTYLER_LIST_CONTENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_RADIO_0                  ("RADIO_0")  
    #define GTAC_LIST_1                   ("LIST_1")  
    #define GTAC_CB_COUNT ( 2 + 1 ) /* Add 1 for the terminator 译:添加1用于终止符 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是在V18版本中新增的功能。 */  
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
    static int GTAC_change_list_cb (int dialog_id, void *client_data,  
            UF_STYLER_item_value_type_p_t callback_data)  
    {  
        char  
            *list_a[4] = { "A1", "A2", "A3", "A4" };  
        UF_STYLER_item_value_type_t  
            ask_value,  
            set_value;  
        ask_value.item_attr = UF_STYLER_VALUE;  
        ask_value.item_id = GTAC_RADIO_0;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        set_value.item_id = GTAC_LIST_1;  
        switch ( ask_value.value.integer )  
        {  
            case 0:  
                set_value.item_attr = UF_STYLER_SUBITEM_VALUES;  
                set_value.value.strings = list_a;  
                set_value.count = 4;  
                break;  
            default:  
                set_value.item_attr = UF_STYLER_LIST_DELETE;  
                set_value.subitem_index = UF_STYLER_NO_SUB_INDEX;  
                break;  
        }  
        UF_CALL(UF_STYLER_free_value(&ask_value));  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        int  
            response;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[GTAC_CB_COUNT] = {  
                {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0,  
                                                           GTAC_change_list_cb},  
                {GTAC_RADIO_0,UF_STYLER_VALUE_CHANGED_CB,0,GTAC_change_list_cb},  
                {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_STYLER_create_dialog( "gtac.dlg", GTAC_cbs, GTAC_CB_COUNT,  
               NULL, &response));  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX Open UI Styler的二次开发示例代码，用于实现一个对话框，其中包含一个单选按钮和一个列表控件。当单选按钮的值发生变化时，会相应地更新列表控件的内容。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了回调函数GTAC_change_list_cb，用于在单选按钮值变化时更新列表控件的内容。
> 3. 在ufusr函数中，初始化UI Styler，并创建一个包含单选按钮和列表控件的对话框，同时设置回调函数。
> 4. ufusr函数最后调用UF_terminate结束UI Styler。
> 5. ufusr_ask_unload函数允许立即卸载二次开发应用程序。
>
> 通过这段代码，实现了单选按钮和列表控件之间的交互，展示了UI Styler的回调机制，以及错误处理方法。
>
