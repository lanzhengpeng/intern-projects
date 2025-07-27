### 【2800】set uistyler dialog width using characters 设置对话框宽度使用字符

#### 代码

```cpp
    /*HEAD SET_UISTYLER_DIALOG_WIDTH_USING_CHARACTERS CCC UFUN */  
    /*  
        Using a Ugraf160 file containing this line on HP-UX resulted in a  
        pixel to character relation of 50/7:  
        *Ugraf160*fontList: -adobe-courier-bold-r-normal--12-120-75-75-m-*  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，UF_print_syslog 是 V18 中新增的函数。 */  
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
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            set_value;  
        set_value.item_attr = UF_STYLER_DIALOG_WIDTH;  
        set_value.item_id = UF_STYLER_DIALOG_INDEX;  
        set_value.value.integer = *(int *)client_data * (50/7);  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            n,  
            resp;  
        UF_STYLER_callback_info_t GTAC_cbs[2] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }};  
        while (prompt_for_an_integer("Dialog width", "Characters", &n) &&  
            !UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, 2, &n, &resp))  
            && (resp == UF_UI_OK));  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个报告错误函数report_error，用于打印错误信息。
> 2. 定义了一个提示输入整数的函数prompt_for_an_integer，用于提示用户输入对话框的宽度字符数。
> 3. 定义了一个回调函数GTAC_const_cb，用于设置对话框的宽度，将用户输入的字符数转换为像素宽度。
> 4. 定义了do_it函数，循环提示用户输入宽度字符数，创建对话框，并设置宽度。
> 5. 定义了ufusr函数，用于初始化NX，调用do_it函数，然后终止NX。
> 6. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 通过这段代码，用户可以输入字符数来设置对话框的宽度，实现了根据字符数来控制对话框宽度的功能。
>
