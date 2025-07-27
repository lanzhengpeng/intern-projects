### 【1061】demo uistyler tab control 演示UIStyler选项卡控件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define GTAC_CB_COUNT ( 2 + 1 )  
    #define GTAC_TAB_CTRL_BEG_0            ("TAB_CTRL_BEG_0")  
    #define GTAC_PAGE_BEG_2                ("PAGE_BEG_2")  
    #define GTAC_LABEL_2                   ("LABEL_2")  
    #define GTAC_PAGE_END_3                ("PAGE_END_3")  
    #define GTAC_PAGE_BEG_3                ("PAGE_BEG_3")  
    #define GTAC_LABEL_4                   ("LABEL_4")  
    #define GTAC_PAGE_END_4                ("PAGE_END_4")  
    #define GTAC_PAGE_BEG_1                ("PAGE_BEG_1")  
    #define GTAC_LABEL_6                   ("LABEL_6")  
    #define GTAC_PAGE_END_2                ("PAGE_END_2")  
    #define GTAC_TAB_CTRL_END_3            ("TAB_CTRL_END_3")  
    #define GTAC_DIALOG_OBJECT_COUNT       ( 11 )  
    static int current_page = 0;  
    static int GTAC_switch_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        current_page = callback_data->value.notify->page_switch.activated_page;  
    //  These are 0 based page numbers  
        ECHO("Switched from page %d to page %d\n",  
            callback_data->value.notify->page_switch.deactivated_page,  
            callback_data->value.notify->page_switch.activated_page);  
        return ( UF_UI_CB_CONTINUE_DIALOG );  
    }  
    static int GTAC_apply_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            value;  
        value.item_id = GTAC_TAB_CTRL_BEG_0;  
        value.item_attr = UF_STYLER_VALUE;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &value));  
    //  This is a 0 based dialog item number - not a page number  
        ECHO("The active page is dialog item %d\n", value.value.integer);  
        ECHO("The page number is %d\n", current_page);  
        return ( UF_UI_CB_CONTINUE_DIALOG );  
    }  
    static UF_STYLER_callback_info_t GTAC_cbs[GTAC_CB_COUNT] =  
    {  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB        , 0, GTAC_apply_cb},  
     {GTAC_TAB_CTRL_BEG_0   , UF_STYLER_SWITCH_CB       , 0, GTAC_switch_cb},  
     {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
    };  
    static void do_it(void)  
    {  
        int  
            response;  
        UF_CALL(UF_STYLER_create_dialog("demo_uistyler_tab_control", GTAC_cbs,  
            GTAC_CB_COUNT, NULL, &response));  
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

> 这段代码是一个NX Open的二次开发示例，用于创建一个包含标签页控件的对话框。
>
> 主要功能包括：
>
> 1. 定义了2个回调函数GTAC_switch_cb和GTAC_apply_cb，分别用于标签页切换和对话框确定时的处理。
> 2. GTAC_cbs数组定义了这2个回调函数的注册信息，以实现对话框与回调函数的关联。
> 3. do_it函数负责创建并显示对话框，其中包含一个标签页控件，回调函数会根据当前激活的标签页来更新current_page变量。
> 4. ufusr函数是NX的二次开发入口函数，用于初始化和创建对话框。
> 5. ufusr_ask_unload函数用于卸载二次开发模块。
>
> 通过这段代码，可以学习到如何在NX中创建一个带标签页控件的对话框，并实现回调函数以处理对话框事件。这对于NX的二次开发具有一定的参考价值。
>
