### 【2346】report selected item of radio box 报告单选按钮选择的项

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_ITEM_OF_RADIO_BOX CCC UFUN */  
    /*  To create a dialog to use with this program:  
        Application->UIStyler  
        Add a Radio Box  
        enter ok callback "apply_cb"  
        File->Save As->"radio"  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，UF_print_syslog是V18版本新增的功能。 */  
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
    #define CHANGE_CB_COUNT ( 2 + 1 )  
    #define CHANGE_RADIO_0                 ("RADIO_0")  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static CHANGE_apply_cb ( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            ask_value;  
        ask_value.item_id = CHANGE_RADIO_0;  
        ask_value.item_attr = UF_STYLER_VALUE;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        WRITE("Radio item selected ");  
        WRITE_D(ask_value.value.integer);  
        if (callback_data->subitem_index == UF_STYLER_OK_INDEX)  
            return (UF_UI_CB_EXIT_DIALOG);  
        else  
            return ( UF_UI_CB_CONTINUE_DIALOG );  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t  
            CHANGE_cbs[CHANGE_CB_COUNT] =  
        {  
          {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB, 0, CHANGE_apply_cb},  
          {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB, 0, CHANGE_apply_cb},  
          {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
        };  
        UF_CALL(UF_STYLER_create_dialog("radio.dlg", CHANGE_cbs, CHANGE_CB_COUNT,  
            NULL, &resp));  
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

> 这段NX二次开发代码的功能如下：
>
> 1. 错误报告：定义了一个错误报告函数report_error，用于在调用UF函数出错时打印错误信息。
> 2. 回调函数：定义了CHANGE_apply_cb函数，作为单选按钮对话框的回调函数。该函数在单选按钮被选中时触发，打印被选中的单选按钮的值。
> 3. 创建对话框：在do_it函数中，使用UF_STYLER_create_dialog创建了一个单选按钮对话框，并指定了CHANGE_apply_cb作为回调函数。
> 4. NX启动和关闭：ufusr函数在NX启动时被调用，用于执行do_it函数，并在完成后关闭NX。
> 5. 卸载函数：ufusr_ask_unload函数被调用来卸载用户二次开发程序。
>
> 整体而言，该代码实现了在NX中创建一个单选按钮对话框，并打印用户所选中的单选按钮值的功能。
>
