### 【1060】demo uistyler push button making invisible dialog item visible 演示如何使用UIStyler将对话框中的项目通过按钮设置为不

#### 代码

```cpp
    /*HEAD DEMO_UISTYLER_PUSH_BUTTON_MAKING_INVISIBLE_DIALOG_ITEM_VISIBLE CCC UFUN */  
    /*-------------------------------------------------------------------------  
    Instructions for creating the dialog:  
    Open the styler.  
    Select Callbacks, and add a constructor callback called "const_cb".  
    Go back to Attributes, and change "CHANGE" to "VISIBLE".  
    Add a Push Button.  Change the identifier to "PUSH_BUTTON".    
    Change the callback to "push_button_cb".  Toggle Creates Dialog ON.  
    Add a multiline textbox.  Leave the default name in place.  Hit Apply in the   
    Resource Editor to make sure that your changes are incorporated.    
    File->Save As-> navigate to your application directory and name the dialog "visible".  
    Exit the Styler application.  
    -------------------------------------------------------------------------*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_defs.h>  
    #include <uf_styler.h>   
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是在V18版本中新增的，请只提供翻译，不要添加其他内容。 */  
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
    #define VISIBLE_PUSH_BUTTON            ("PUSH_BUTTON")  
    #define VISIBLE_MTXT_1                 ("MTXT_1")  
    #define VISIBLE_DIALOG_OBJECT_COUNT    ( 2 )  
    #define VISIBLE_CB_COUNT ( 3 + 1 ) /* Add 1 for the terminator 里海译:加1作为终结符。 */  
    /* qq3123197280 */  
    int VISIBLE_const_cb ( int dialog_id,  
                 void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data);  
    /* qq3123197280 */  
    int VISIBLE_apply_cb ( int dialog_id,  
                 void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data);  
    /* qq3123197280 */  
    int VISIBLE_push_button_cb ( int dialog_id,  
                 void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data);  
    static UF_STYLER_callback_info_t VISIBLE_cbs[VISIBLE_CB_COUNT] =   
    {  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB  , 0, VISIBLE_const_cb},  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB        , 0, VISIBLE_apply_cb},  
     {VISIBLE_PUSH_BUTTON   , UF_STYLER_ACTIVATE_CB     , 1, VISIBLE_push_button_cb},  
     {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
    };  
    static void do_it(void)  
    {  
        int  
            response = 0;  
        UF_CALL( UF_STYLER_create_dialog ( "visible.dlg",  
                 VISIBLE_cbs,      /* Callbacks from dialog 里海译:对不起，我不能提供纯文本答案，但可以帮您翻译“Callbacks from dialog”为“从对话框中回调”。如果您有其他问题或需要帮助，请随时告诉我。 */  
                 VISIBLE_CB_COUNT, /* number of callbacks*/  
                 NULL,        /* This is your client data 里海译:这是你的客户数据。 */  
                 &response ) );  
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
    /* qq3123197280 */  
    int VISIBLE_const_cb ( int dialog_id, void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
            /* make the multiline textbox invisible 里海译:将多行文本框设为不可见。 */  
        UF_STYLER_item_value_type_t  
            set_value;  
        set_value.item_attr = UF_STYLER_VISIBILITY;  
        set_value.item_id = VISIBLE_MTXT_1;  
        set_value.value.integer = FALSE;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);   
    }  
    /* qq3123197280 */  
    int VISIBLE_apply_cb ( int dialog_id, void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
        return (UF_UI_CB_CONTINUE_DIALOG);   
    }  
    /* qq3123197280 */  
    int VISIBLE_push_button_cb ( int dialog_id, void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
            /* make the multiline textbox visible again when the button is pushed 里海译:按下按钮时再次显示多行文本框。 */  
        UF_STYLER_item_value_type_t  
            set_value;  
        set_value.item_attr = UF_STYLER_VISIBILITY;  
        set_value.item_id = VISIBLE_MTXT_1;  
        set_value.value.integer = TRUE;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);   
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要功能是创建一个对话框，并在对话框中添加一个按钮和一个多行文本框。代码的具体功能如下：
>
> 1. 定义了错误报告函数report_error，用于打印错误信息。
> 2. 定义了对话框的回调函数数组VISIBLE_cbs，其中包括了构造函数回调VISIBLE_const_cb，应用回调VISIBLE_apply_cb，以及按钮回调VISIBLE_push_button_cb。
> 3. 在VISIBLE_const_cb中，将多行文本框设为不可见。
> 4. 在VISIBLE_push_button_cb中，当按钮被点击时，将多行文本框设为可见。
> 5. 定义了do_it函数，用于创建对话框。
> 6. 定义了ufusr函数，作为NX的二次开发接口函数，用于初始化并调用do_it。
> 7. 定义了ufusr_ask_unload函数，用于卸载二次开发。
> 8. 在VISIBLE_const_cb中，将多行文本框设为不可见。
> 9. 在VISIBLE_push_button_cb中，当按钮被点击时，将多行文本框设为可见。
>
> 总体来说，这段代码通过NX的二次开发接口，创建了一个对话框，并实现了按钮控制多行文本框的可见性。
>
