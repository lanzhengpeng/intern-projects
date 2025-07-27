### 【2738】set button layout sensitivity 设置按钮布局灵敏度

#### 代码

```cpp
    /*HEAD SET_BUTTON_LAYOUT_SENSITIVITY CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_BLO_0                     ("BLO_0")  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能。 */  
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
    static int prompt_for_an_integer(char *prompt, char *item, int number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            return da[0];  
        }  
        else return -1;  
    }  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            set_value;  
        set_value.item_attr = UF_STYLER_SENSITIVITY;  
        set_value.item_id =  GTAC_BLO_0;  
        set_value.value.integer = FALSE;  
        set_value.subitem_index = *(int*)client_data;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            n = 0,  
            resp;  
        UF_STYLER_callback_info_t GTAC_cbs[2] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }};  
        while (((n = prompt_for_an_integer("Grey out?", "item", n)) >= 0) &&  
            !UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, 2, &n,  
                &resp)) && (resp == UF_UI_OK));  
    }  
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

> 这段代码是用于NX二次开发的示例，其主要功能是创建一个对话框，并允许用户通过提示输入来更改对话框中某个控件的可见性。具体来说，代码实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息到系统日志。
> 2. 定义了一个提示输入整数的函数prompt_for_an_integer，用于提示用户输入一个整数值。
> 3. 定义了一个回调函数GTAC_const_cb，用于设置对话框中某个控件的可见性。
> 4. 在do_it函数中，使用循环提示用户输入，根据输入的值创建一个对话框，并使用回调函数设置对话框中对应控件的可见性。
> 5. ufusr函数初始化NX并调用do_it函数。
> 6. ufusr_ask_unload函数用于卸载该代码。
>
> 总体来说，这段代码实现了一个简单的二次开发示例，通过提示输入和回调函数，实现了用户交互式更改对话框控件可见性的功能。
>
