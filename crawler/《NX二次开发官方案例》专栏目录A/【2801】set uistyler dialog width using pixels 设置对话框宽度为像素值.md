### 【2801】set uistyler dialog width using pixels 设置对话框宽度为像素值

#### 代码

```cpp
    /*HEAD SET_UISTYLER_DIALOG_WIDTH_USING_PIXELS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
UF_print_syslog 是 V18 中的新增功能。 */  
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
        set_value.value.integer = *(int *)client_data;  
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
        while (prompt_for_an_integer("Dialog width", "Pixels", &n) &&  
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

> 这段代码是用于NX 18版本中的UIStyler对话框宽度设置的二次开发示例代码。
>
> 主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息到系统日志和对话框列表窗口。
> 2. 定义了提示输入整数的函数prompt_for_an_integer，用于提示用户输入对话框宽度。
> 3. 定义了回调函数GTAC_const_cb，用于设置对话框宽度。
> 4. do_it函数中，使用prompt_for_an_integer获取用户输入的宽度值，然后调用UF_STYLER_create_dialog创建对话框，并通过GTAC_cbs数组传入GTAC_const_cb回调，用于设置对话框宽度。
> 5. ufusr函数中，初始化UF系统，调用do_it函数进行对话框宽度设置，然后终止UF系统。
> 6. ufusr_ask_unload函数用于卸载当前UF库。
>
> 该代码通过UF_STYLER接口，实现了在NX中自定义UIStyler对话框的宽度，并通过交互式提示获取用户输入，实现了一个简单的对话框宽度设置二次开发示例。
>
