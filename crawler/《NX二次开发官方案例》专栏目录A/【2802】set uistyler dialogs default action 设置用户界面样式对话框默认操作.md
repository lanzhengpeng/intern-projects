### 【2802】set uistyler dialogs default action 设置用户界面样式对话框默认操作

#### 代码

```cpp
    /*HEAD SET_UISTYLER_DIALOGS_DEFAULT_ACTION CCC UFUN */  
    /*  To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Change the dialog's Prefix Name to "GTAC"  
        - Specify the Constructor Callback as GTAC_"const_cb"  
        - Add a Push Button object  
        - Specify its Activate callback ask GTAC_"push_cb"  
        - Save Dialog As "gtac"  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_ACTION_0                  ("ACTION_0")  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新增功能，用于打印系统日志。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            set_value;  
        WRITE("Setting default to action button...\n");  
        set_value.item_attr = UF_STYLER_DEFAULT_ACTION;  
        set_value.item_id = GTAC_ACTION_0;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_push_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            set_value;  
        WRITE("Setting default to OK button...\n");  
        set_value.item_attr = UF_STYLER_SENSITIVITY;  
        set_value.item_id = UF_STYLER_NAV_INDEX;  
        set_value.subitem_index = UF_STYLER_OK_INDEX;  
        set_value.value.integer = TRUE;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        set_value.item_attr = UF_STYLER_DEFAULT_ACTION;  
        set_value.item_id = UF_STYLER_NAV_INDEX;  
        set_value.subitem_index = UF_STYLER_OK_INDEX;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t GTAC_cbs[3] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB  , 0, GTAC_const_cb},  
            {GTAC_ACTION_0         , UF_STYLER_ACTIVATE_CB     , 0, GTAC_push_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }};  
        UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, 3, NULL, &resp));  
        WRITE_D(resp);  
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

> 这段代码是一个NX Open UI Styler对话框的示例，其主要功能包括：
>
> 1. 定义宏：用于错误报告和打印日志，方便调试。
> 2. GTAC_const_cb：用于在对话框构造时设置默认按钮。
> 3. GTAC_push_cb：用于在按下按钮时设置OK按钮为默认按钮并激活。
> 4. do_it：用于创建并显示对话框。
> 5. ufusr：作为NX Open的入口函数，初始化环境并调用do_it。
> 6. ufusr_ask_unload：用于卸载用户函数。
>
> 该代码通过回调函数实现了设置默认按钮的功能，使用了UF_STYLER API来创建和管理对话框，从而展示了NX Open UI Styler对话框编程的基本流程。
>
