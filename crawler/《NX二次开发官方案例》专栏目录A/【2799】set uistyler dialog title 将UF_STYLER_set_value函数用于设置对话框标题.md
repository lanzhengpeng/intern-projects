### 【2799】set uistyler dialog title 将UF_STYLER_set_value函数用于设置对话框标题

#### 代码

```cpp
    /*HEAD SET_UISTYLER_DIALOG_TITLE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文意，UF_print_syslog是V18版本新增的功能。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            set_value;  
        set_value.item_attr = UF_STYLER_LABEL;  
        set_value.item_id = UF_STYLER_DIALOG_INDEX;  
        set_value.value.string = (char *)client_data;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        char  
            title_text[133];  
        UF_STYLER_callback_info_t GTAC_cbs[2] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }};  
        while (prompt_for_text("Enter text for dialog title", title_text) &&  
            !UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, 2, title_text,  
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

> 这段代码是一个NX的二次开发示例，主要功能是在NX中创建一个标题为用户输入文本的对话框。
>
> 代码的主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印出NX API调用错误信息。
> 2. 定义了一个获取用户输入文本的函数prompt_for_text。
> 3. 定义了一个设置对话框标题的回调函数GTAC_const_cb。
> 4. 主函数do_it中，循环获取用户输入的标题文本，然后创建一个对话框，并设置标题文本。
> 5. ufusr是NX二次开发的入口函数，在其中调用do_it函数来实现功能。
> 6. ufusr_ask_unload函数用于卸载二次开发模块。
>
> 通过以上代码，可以在NX中创建一个简单的对话框，并可以设置用户指定的标题。这为NX二次开发提供了一个基本示例。
>
