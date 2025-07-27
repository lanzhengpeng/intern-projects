### 【2789】set radio box initial value 设置单选框初始值

#### 代码

```cpp
    /*HEAD SET_RADIO_BOX_INITIAL_VALUE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_RADIO_0                   ("RADIO_0")  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，可以翻译为：

注：UF_print_syslog是V18版本新增的。 */  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    /* qq3123197280 */  
    static int GTAC_const_cb ( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            *number = (int *)client_data;  
        UF_STYLER_item_value_type_t  
            set_value;  
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.item_id = GTAC_RADIO_0;  
        set_value.subitem_index = *number - 1;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_radio_cb ( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            *number = (int *)client_data;  
        *number = callback_data->value.integer + 1;  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            number = 2,  
            resp;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[3] = {  
              {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
              {GTAC_RADIO_0, UF_STYLER_VALUE_CHANGED_CB, 0, GTAC_radio_cb},  
              {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        while (prompt_for_an_integer("Radio box default", "option", &number))  
            UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, 3,  
                (void *)&number, &resp));  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义了提示用户输入整数的函数prompt_for_an_integer。
> 3. 定义了两个回调函数GTAC_const_cb和GTAC_radio_cb。GTAC_const_cb用于在对话框创建时设置单选按钮的默认选中项，GTAC_radio_cb用于在单选按钮值改变时获取新的选中项。
> 4. 主函数do_it中，使用prompt_for_an_integer函数提示用户输入单选按钮的默认选中项，并创建一个对话框，使用GTAC_cbs数组中的回调函数。
> 5. ufusr函数是NX二次开发的入口函数，其中调用了do_it函数，实现了上述功能。
> 6. ufusr_ask_unload函数用于卸载二次开发程序。
>
> 总的来说，这段代码通过回调机制实现了单选按钮的默认选中项设置以及选中项值变化的获取，展示了NX二次开发中对话框和回调的使用。
>
