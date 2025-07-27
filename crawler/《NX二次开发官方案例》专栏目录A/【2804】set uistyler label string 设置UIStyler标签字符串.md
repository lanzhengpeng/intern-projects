### 【2804】set uistyler label string 设置UIStyler标签字符串

#### 代码

```cpp
    /*HEAD SET_UISTYLER_LABEL_STRING CCC UFUN */  
    /*  To create your own dialog for use with this program:  
        Application->UIStyler  
        change CHANGE to GTAC  
        enter constructor callback:  GTAC_const_cb  
        add a label object  
        File->Save As->gtac  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_LABEL_0                   ("LABEL_0")  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
注意：UF_print_syslog 是 V18 中新增的，请只提供翻译，不要添加任何无关内容。 */  
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
    /* qq3123197280 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            set_value;  
        set_value.item_attr = UF_STYLER_LABEL;  
        set_value.item_id = GTAC_LABEL_0;  
        set_value.value.string = (char *)client_data;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
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
    static void do_it(void)  
    {  
        int  
            resp;  
        char  
            text[133] = { "New Label Text" };  
        UF_STYLER_callback_info_t  
            GTAC_cbs[2] = {  
                {UF_STYLER_DIALOG_INDEX,UF_STYLER_CONSTRUCTOR_CB,0,GTAC_const_cb},  
                {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        while (prompt_for_text("Enter string for label", text) &&  
            !UF_CALL(UF_STYLER_create_dialog("gtac", GTAC_cbs, 2, text, &resp)) &&  
            (resp == UF_UI_OK));  
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

> 这段代码是NX Open C++的二次开发代码，主要实现以下功能：
>
> 1. 定义了错误报告函数report_error，用于在发生UF函数调用错误时打印错误信息。
> 2. 定义了对话框构造回调函数GTAC_const_cb，用于在创建对话框时设置标签控件的文本。
> 3. 定义了提示输入文本的函数prompt_for_text，用于提示用户输入文本。
> 4. 定义了主函数do_it，用于创建对话框并设置标签文本。
> 5. 定义了ufusr函数，作为NX Open C++的入口函数，用于初始化、调用主函数，然后终止。
> 6. 定义了ufusr_ask_unload函数，用于处理卸载时的情况。
>
> 整体来说，这段代码实现了一个简单的二次开发功能：通过自定义对话框，允许用户输入文本并显示在标签控件上。通过回调函数，实现了对话框的动态创建和设置。
>
