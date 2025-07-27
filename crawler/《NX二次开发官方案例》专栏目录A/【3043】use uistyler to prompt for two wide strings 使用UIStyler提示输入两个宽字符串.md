### 【3043】use uistyler to prompt for two wide strings 使用UIStyler提示输入两个宽字符串

#### 代码

```cpp
    /*HEAD USE_UISTYLER_TO_PROMPT_FOR_TWO_WIDE_STRINGS CCC UFUN */  
    /*  To create your own dialog for use with this program:  
        Application->UIStyler  
        change CHANGE to GTAC  
        set Button Style Options to OK and Cancel  
        enter ok callback:  GTAC_ok_cb  
        add a wide string  
        add another wide string  
        File->Save As->gtac  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。

该功能主要用于打印系统日志。 */  
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
    struct two_strings_s  
    {  
        char  
            string1[133],  
            string2[133];  
    };  
    typedef struct two_strings_s two_strings_t, *two_strings_p_t;  
    #define GTAC_WIDE_S_0                  ("WIDE_S_0")  
    #define GTAC_WIDE_S_1                  ("WIDE_S_1")  
    /* qq3123197280 */  
    static int GTAC_ok_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            n;  
        two_strings_p_t  
            the_data = (two_strings_p_t) client_data;  
        UF_STYLER_item_value_type_t  
            ask_value[2];  
        ask_value[0].item_attr = UF_STYLER_VALUE;  
        ask_value[0].item_id = GTAC_WIDE_S_0;  
        ask_value[1].item_attr = UF_STYLER_VALUE;  
        ask_value[1].item_id = GTAC_WIDE_S_1;  
        UF_CALL(UF_STYLER_ask_values(dialog_id, 2, ask_value, &n));  
        strcpy(the_data->string1, ask_value[0].value.string);  
        strcpy(the_data->string2, ask_value[1].value.string);  
        UF_CALL(UF_STYLER_free_value(&ask_value[0]));  
        UF_CALL(UF_STYLER_free_value(&ask_value[1]));  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        two_strings_t  
            data;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[2] = {  
                {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB         , 0, GTAC_ok_cb},  
                {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, 2,  
            (void *)&data, &resp));  
        if (resp != UF_UI_OK) return;  
        WRITE_S(data.string1);  
        WRITE_S(data.string2);  
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

> 这段代码是一个NX Open Application的示例，用于创建一个包含两个字符串输入的对话框，并在用户输入后显示这两个字符串。
>
> 代码的主要功能如下：
>
> 1. 使用UIStyler工具创建了一个包含两个宽字符串输入框的对话框，并保存为gtac.dlg。
> 2. 定义了一个结构体two_strings_t，用于存储用户输入的两个字符串。
> 3. 定义了GTAC_ok_cb回调函数，在用户点击OK按钮时调用，从对话框获取两个字符串并存储在two_strings_t结构体中。
> 4. 定义了WRITE_S宏，用于在列表窗口中显示字符串。
> 5. do_it函数创建并显示对话框，获取用户输入，并显示结果。
> 6. ufusr函数初始化NX Open Application，调用do_it，然后终止。
> 7. ufusr_ask_unload函数用于卸载应用程序。
>
> 这段代码展示了如何使用UIStyler创建自定义对话框，并通过回调函数获取用户输入，从而实现与用户的交互。
>
