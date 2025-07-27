### 【2410】report uistyler option menu selection 报告选项菜单选择

#### 代码

```cpp
    /*HEAD REPORT_OPTION_MENU_SELECTION CCC UFUN */  
    /*  To test this example - you must create a simple dialog:  
        1.  change CHANGE to GTAC  
        2.  assign the constructor callback "GTAC_const_cb"  
        3.  check that the apply callback is "GTAC_apply_cb"  
        4.  add an option menu  
        5.  Save the dialog to your application directory as gtac.dlg  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_OPTION_0                  ("OPTION_0")  
    #define GTAC_CB_COUNT ( 2 + 1 ) /* Add 1 for the terminator 里海译:添加1以终止。 */  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数，用于打印系统日志信息。

只回答译文：UF_print_syslog是V18版本新增的函数，用于打印系统日志信息。 */  
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
    static int GTAC_const_cb ( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            value;  
        char  
            *opts[4] = { "Number One","Number Two","Number Three","Number Four" };  
        value.item_attr = UF_STYLER_SUBITEM_VALUES;  
        value.item_id = GTAC_OPTION_0;  
        value.count = 4;  
        value.value.strings = opts;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &value));  
    /*  Work around required to avoid a default value of -1 (See PR 4138769) 里海译:绕过需要避免默认值-1(请参阅PR 4138769) */  
        value.item_attr = UF_STYLER_VALUE;  
        value.item_id = GTAC_OPTION_0;  
        value.subitem_index = 2;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &value));  
    /*  End Work around 里海译:翻译结果为："结束工作"。 */  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static int GTAC_apply_cb ( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            count = 0;  
        UF_STYLER_item_value_type_t  
            values[2];  
        values[0].item_attr = UF_STYLER_SUBITEM_VALUES;  
        values[0].item_id = GTAC_OPTION_0;  
        values[1].item_attr = UF_STYLER_VALUE;  
        values[1].item_id = GTAC_OPTION_0;  
        UF_CALL(UF_STYLER_ask_values(dialog_id, 2, values, &count));  
        ECHO(values[1].value.integer);  
        uc1601(values[0].value.strings[values[1].value.integer], 1);  
        UF_CALL(UF_STYLER_free_value(&values[0]));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[GTAC_CB_COUNT] = {  
              {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
              {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB, 0, GTAC_apply_cb},  
              {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, GTAC_CB_COUNT,  
            NULL, &resp));  
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

> 这段代码是NX Open UI的二次开发示例，用于演示如何创建和使用NX Open UI的选项菜单。
>
> 代码的主要功能包括：
>
> 1. 定义了两个回调函数GTAC_const_cb和GTAC_apply_cb，分别用于在对话框打开时初始化选项菜单，以及在用户点击应用按钮时获取选择的选项。
> 2. GTAC_const_cb函数通过UF_STYLER_set_value函数设置了选项菜单的选项内容，并绕过了一个默认值问题。
> 3. GTAC_apply_cb函数通过UF_STYLER_ask_values函数获取了用户选择的选项索引，并打印了选择的选项内容。
> 4. do_it函数通过UF_STYLER_create_dialog函数创建对话框，并传入回调函数数组。
> 5. ufusr函数是NX Open程序的入口函数，其中调用了do_it函数。
> 6. ufusr_ask_unload函数定义了程序卸载时的行为。
>
> 总体来说，这段代码通过回调函数和NX Open UI函数的交互，实现了选项菜单的创建、初始化和获取选择的功能。
>
