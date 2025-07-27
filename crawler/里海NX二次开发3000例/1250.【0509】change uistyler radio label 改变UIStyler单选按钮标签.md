### 【0509】change uistyler radio label 改变UIStyler单选按钮标签

#### 代码

```cpp
    /*HEAD CHANGE_UISTYLER_RADIO_LABEL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_RADIO_0                  ("RADIO_0")  
    #define GTAC_RADIO_1                   ("RADIO_1")  
    #define GTAC_CB_COUNT ( 2 + 1 ) /* Add 1 for the terminator 译:加1以终止 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 中新增的函数。 */  
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
    /* 里海 */  
    static int GTAC_change_list_cb (int dialog_id, void *client_data,  
            UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            ask_value,  
            set_value;  
        ask_value.item_attr = UF_STYLER_VALUE;  
        ask_value.item_id = GTAC_RADIO_0;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        set_value.item_id = GTAC_RADIO_1;  
        switch ( ask_value.value.integer )  
        {  
            case 0:  
                set_value.item_attr = UF_STYLER_LABEL;  
                set_value.value.string = "Select Number";  
                break;  
            default:  
                set_value.item_attr = UF_STYLER_LABEL;  
                set_value.value.string = "Choisissez le Nombre";  
                break;  
        }  
        UF_CALL(UF_STYLER_free_value(&ask_value));  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* 里海 */  
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

> 这段代码是一个NX Open UIStyler的二次开发示例，主要功能如下：
>
> 1. 定义了两个单选按钮RADIO_0和RADIO_1，以及一个回调函数GTAC_change_list_cb，用于根据RADIO_0的选择改变RADIO_1的标签。
> 2. 在GTAC_change_list_cb函数中，首先获取RADIO_0的值，然后根据这个值来设置RADIO_1的标签。如果RADIO_0的值是0，则将RADIO_1的标签设置为"Select Number"，否则设置为"Choisissez le Nombre"。
> 3. 在ufusr函数中，初始化UIStyler，并创建一个对话框，对话框的布局文件为"gtac.dlg"。同时，为对话框指定了两个回调函数，一个是构造函数回调GTAC_change_list_cb，一个是RADIO_0值变化时的回调GTAC_change_list_cb。
> 4. 在ufusr函数的最后，调用UF_terminate终止UIStyler。
> 5. ufusr_ask_unload函数用于立即卸载二次开发程序。
>
> 总的来说，这段代码实现了根据一个单选按钮的选择来动态改变另一个单选按钮标签的功能，通过UIStyler的回调机制来实现交互。
>
