### 【2806】set uistyler string based on name display status 根据名称显示状态设置UIStyler字符串

#### 代码

```cpp
    /*HEAD SET_UISTYLER_STRING_IF_NAME_DISPLAY_ON CCC UFUN */  
    /*  To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Change the dialog's Prefix Name to "GTAC"  
        - Specify the Constructor Callback as GTAC_"const_cb"  
        - Add a String object  
        - Save Dialog As "application/gtac"  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_STR_0                     ("STR_0")  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，以下是UF_print_syslog的相关信息：

UF_print_syslog是一个新函数，首次出现在V18版本中。

该函数的作用是将一条日志消息发送到系统日志中。

函数原型为：
int UF_print_syslog( const char *fmt, ... );

其中fmt是格式化字符串，可以包含格式化占位符，如%s、%d等，函数会根据fmt格式化后面的参数，并将格式化后的字符串发送到系统日志中。

调用该函数时，需要包含头文件<ulib.h>。

UF_print_syslog是一个非常有用的函数，它允许用户将自定义的日志消息发送到系统日志中，以便进行调试和监控。 */  
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
    static int GTAC_const_cb(int dialog_id, void * client_data,  
                              UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            set_value;  
        int  
            current_status;  
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.item_id = GTAC_STR_0;  
        UF_CALL(UF_DISP_ask_name_display_status (&current_status));  
        if (current_status == UF_DISP_NAME_DISPLAY_OFF)  
            set_value.value.string = "name off";  
        else  
            set_value.value.string = "name on";  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return ( UF_UI_CB_CONTINUE_DIALOG );  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[2] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, 2, NULL, &resp));  
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

> 这段代码是NX Open UI Styler的一个示例，用于创建一个简单的对话框，并在其中显示NX显示名称的状态。具体来说：
>
> 1. 首先，定义了GTAC_STR_0常量，作为字符串控件的ID。
> 2. 定义了report_error函数，用于在发生UF函数调用错误时，将错误信息打印到系统日志和UF日志窗口。
> 3. 定义了GTAC_const_cb函数，作为对话框构造回调函数。在对话框创建时调用，获取NX的显示名称状态，并将状态设置为字符串控件的值。
> 4. 定义了do_it函数，创建对话框，并设置构造回调函数。
> 5. 定义了ufusr函数，作为NX Open的入口函数。初始化UF库，调用do_it函数创建并显示对话框，最后终止UF库。
> 6. 定义了ufusr_ask_unload函数，用于卸载用户库。
>
> 这段代码通过UI Styler创建了一个简单的对话框，并在其中显示了NX显示名称的状态。它展示了如何使用UI Styler进行二次开发，创建对话框并获取NX系统信息。
>
