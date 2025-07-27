### 【1063】demonstrate uistyler toggle 演示UIStyler的切换功能

#### 代码

```cpp
    /*HEAD DEMONSTRATE_UISTYLER_TOGGLE CCC UFUN */  
    /*  
        This shows how you could use a value-changed callback for   
        a Toggle item to control the sensitivity of another   
        dialog item - in this case, a Real entry field.  
        To create the UIStyler dialog that goes with this program,  
        open the UIStyler application, and change "CHANGE" to "TOGGLE".  
        Add a "Toggle" dialog object, and assign a callback name   
        of "toggle_cb".  
        Add a "Real" dialog object and accept all defaults, but toggle the  
        "Sensitive" field so that it is not checked.  
        Save the dialog as "toggle_demo", and be sure to put "toggle_demo.dlg"   
        in a valid "application" directory.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_defs.h>  
    #include <uf_styler.h>   
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define TOGGLE_TOGGLE_0                ("TOGGLE_0")  
    #define TOGGLE_REAL_1                  ("REAL_1")  
    #define TOGGLE_DIALOG_OBJECT_COUNT     ( 2 )  
    #define TOGGLE_CB_COUNT ( 2 + 1 ) /* Add 1 for the terminator 里海译:在终结符处加1。 */  
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
    int TOGGLE_apply_cb ( int dialog_id,  
                 void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data);  
    int TOGGLE_toggle_cb ( int dialog_id,  
                 void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data);  
    static UF_STYLER_callback_info_t TOGGLE_cbs[TOGGLE_CB_COUNT] =   
    {  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB        , 0, TOGGLE_apply_cb},  
     {TOGGLE_TOGGLE_0       , UF_STYLER_VALUE_CHANGED_CB, 0, TOGGLE_toggle_cb},  
     {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
    };  
    static void do_it(void)  
    {  
        int    
            response = 0;  
        UF_CALL( UF_STYLER_create_dialog ( "toggle_demo.dlg",  
                 TOGGLE_cbs,      /* Callbacks from dialog 里海译:对不起，我无法提供翻译服务。 */  
                 TOGGLE_CB_COUNT, /* number of callbacks*/  
                 NULL,        /* This is your client data 里海译:这是您的客户数据。 */  
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
    int TOGGLE_apply_cb ( int dialog_id, void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
        return (UF_UI_CB_CONTINUE_DIALOG);   
    }  
    /* qq3123197280 */  
    int TOGGLE_toggle_cb ( int dialog_id, void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            ask_value,  
            set_value;  
        ask_value.item_attr =  UF_STYLER_VALUE;  
        ask_value.item_id = TOGGLE_TOGGLE_0 ;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        set_value.value.integer = ask_value.value.integer;  
        set_value.item_attr = UF_STYLER_SENSITIVITY;  
        set_value.item_id = TOGGLE_REAL_1;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);   
    }

```

#### 代码解析

> 这是段NX二次开发代码，用于展示如何使用UIStyler创建一个包含Toggle和Real两种控件的对话框，并通过Toggle控件的值变化回调来控制Real控件的敏感度。
>
> 主要功能包括：
>
> 1. 定义了Toggle和Real两种控件的ID。
> 2. 实现了Toggle的值变化回调TOGGLE_toggle_cb，当Toggle的值变化时，会读取Toggle的值，然后设置Real的敏感度。
> 3. 实现了对话框的确定回调TOGGLE_apply_cb，目前仅返回继续对话框。
> 4. 定义了回调数组TOGGLE_cbs，包含了Toggle的值变化回调和一个结束回调。
> 5. 实现了主函数do_it，用于创建对话框并显示。
> 6. 定义了ufusr函数作为NX的入口函数，用于初始化、显示对话框、终止等操作。
> 7. 定义了卸载回调函数ufusr_ask_unload，返回立即卸载。
>
> 通过Toggle控件的值变化回调，实现了控制Real控件敏感度的功能，展示了UIStyler对话框编程的一个典型应用场景。
>
