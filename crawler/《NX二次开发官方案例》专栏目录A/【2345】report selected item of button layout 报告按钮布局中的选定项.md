### 【2345】report selected item of button layout 报告按钮布局中的选定项

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_ITEM_OF_BUTTON_LAYOUT CCC UFUN */  
    /*  To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Add a button layout with 3 buttons and name it BUTTON_LAYOUT  
        - Add a string or wide string item and name it COLOR_STRING  
        - Save Dialog As "application/button_layout.dlg"  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    #define CHANGE_BUTTON_LAYOUT ("BUTTON_LAYOUT")  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog在V18版本中是新增的功能。因此，对于该功能的介绍，我们只需翻译以下内容：

UF_print_syslog是V18版本新增的功能。

不需要添加任何废话或额外的解释。 */  
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
    static int CHANGE_button_layout_act_cb ( int dialog_id,  
                 void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            set_value;  
        char  
            *color[]= {"red", "green", "blue"},  
            str[10];  
        strcpy(str, color[callback_data->value.integer]);  
        set_value.item_id = "COLOR_STRING";  
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.value.string = str;  
        UF_CALL( UF_STYLER_set_value(dialog_id, &set_value) );  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t  
            CHANGE_cbs[2] = {  
                {CHANGE_BUTTON_LAYOUT , UF_STYLER_ACTIVATE_CB, 0,  
                    CHANGE_button_layout_act_cb},  
                {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
            };  
        UF_CALL(UF_STYLER_create_dialog("button_layout.dlg", CHANGE_cbs, 3, NULL,  
            &resp));  
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

> 这段代码是NX二次开发的示例，主要功能包括：
>
> 1. 报告错误函数：定义了一个report_error函数，用于在发生错误时报告错误代码、错误消息和导致错误的调用，并通过UF_print_syslog打印到系统日志。
> 2. 按钮布局变更回调函数：定义了CHANGE_button_layout_act_cb函数，当用户点击按钮时，该函数会被调用。它会根据用户点击的按钮设置COLOR_STRING项的值。
> 3. 主函数：do_it函数创建一个名为"button_layout.dlg"的对话框，并注册了CHANGE_button_layout_act_cb回调函数。当用户点击按钮时，回调函数会被调用，实现按钮的交互功能。
> 4. UFusr主函数：ufusr函数是NX二次开发的入口点。在初始化NX环境后，调用do_it函数执行主要功能，最后关闭NX环境。
> 5. 卸载函数：ufusr_ask_unload函数用于处理NX关闭时模块的卸载。
>
> 总的来说，这段代码展示了如何通过NX的UI Styler工具创建对话框，并通过回调函数实现用户交互。代码结构清晰，功能简单，适合作为NX二次开发的入门示例。
>
