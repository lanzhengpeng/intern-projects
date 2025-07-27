### 【2780】set multi line text values 设置多行文本值

#### 代码

```cpp
    /*HEAD SET_MULTI_LINE_TEXT_VALUES CCC UFUN */  
    /*  To test this example - you must create a simple dialog:  
        1.  Change CHANGE to GTAC  
        2.  Set "const_cb" as the apply and constructor Callbacks  
        3.  Add an Integer  
        4.  Set its Label "How many lines?"  
        5.  Set its Activate callback "const_cb"  
        6.  Add a Multi-Line Text Box  
        7.  Save the dialog to your application directory as gtac.dlg  
    */  
    #include <stdio.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于在用户函数中打印系统日志消息。 */  
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
    #define GTAC_INT_0                     ("INT_0")  
    #define GTAC_MTXT_1                    ("MTXT_1")  
    #define GTAC_DIALOG_OBJECT_COUNT       ( 2 )  
    #define GTAC_CB_COUNT ( 3 + 1 ) /* Add 1 for the terminator 里海译:增加1个终止符。 */  
    /* qq3123197280 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            err,  
            ii;  
        char  
            **text_ptrs;  
        UF_STYLER_item_value_type_t  
            ask_value,  
            set_value;  
        ask_value.item_attr = UF_STYLER_VALUE;  
        ask_value.item_id = GTAC_INT_0;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        text_ptrs = (char **)UF_allocate_memory(ask_value.value.integer *  
            sizeof(char *), &err);  
        for (ii = 0; ii < ask_value.value.integer; ii++)  
        {  
            text_ptrs[ii] = (char *)UF_allocate_memory(133 * sizeof(char), &err);  
            sprintf(text_ptrs[ii], "Line %d", ii+1);  
        }  
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.item_id = GTAC_MTXT_1;  
        set_value.count = ask_value.value.integer;  
        set_value.value.strings = text_ptrs;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        if (ask_value.value.integer > 0)  
            UF_free_string_array(ask_value.value.integer, text_ptrs);  
        UF_CALL(UF_STYLER_free_value(&ask_value));  
        return ( UF_UI_CB_CONTINUE_DIALOG );  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t GTAC_cbs[GTAC_CB_COUNT] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB      , 0, GTAC_const_cb},  
            {GTAC_INT_0            , UF_STYLER_ACTIVATE_CB   , 0, GTAC_const_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL(UF_STYLER_create_dialog( "gtac.dlg", GTAC_cbs, GTAC_CB_COUNT,  
            NULL, &resp));  
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

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是在NX中创建一个对话框，并在对话框中设置一个整数输入框和一个多行文本框。当用户改变整数输入框的值时，会触发回调函数，根据整数值在多行文本框中显示相应数量的行文本。具体实现步骤如下：
>
> 1. 定义了GTAC_CB_COUNT等常量，用于定义回调函数的个数等。
> 2. 定义了GTAC_const_cb函数，作为构造函数、应用按钮和多行文本框激活的回调函数。在该函数中，先获取整数输入框的值，然后根据这个值动态创建相应数量的文本行，并设置到多行文本框中。
> 3. 定义了do_it函数，用于创建对话框，并设置好回调函数。
> 4. ufusr函数作为二次开发的入口函数，初始化NX环境，调用do_it函数创建对话框，然后终止NX环境。
> 5. ufusr_ask_unload函数用于在二次开发模块卸载时立即卸载。
>
> 整体而言，这段代码展示了如何在NX中创建对话框，并使用回调函数来响应控件的事件，实现控件之间的交互，是一个典型的NX二次开发示例。
>
