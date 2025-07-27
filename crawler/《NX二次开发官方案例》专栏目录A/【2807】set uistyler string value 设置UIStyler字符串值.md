### 【2807】set uistyler string value 设置UIStyler字符串值

#### 代码

```cpp
    /*HEAD SET_UISTYLER_STRING_VALUE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，翻译如下：

注意：UF_print_syslog 是 V18 版本中新增的。 */  
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
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.item_id = GTAC_STR_0;  
        set_value.value.string = "my test";  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return ( UF_UI_CB_CONTINUE_DIALOG );  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t GTAC_cbs[2] =  
        {  
          {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB  , 0, GTAC_const_cb},  
          {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
        };  
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

> 这段代码是一个NX Open的二次开发示例，用于创建一个简单的UIStyler对话框并设置一个字符串控件的值。
>
> 主要功能如下：
>
> 1. 包含了必要的头文件，如uf.h、uf_ui.h、uf_styler.h等。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息。
> 3. 定义了一个构造函数回调GTAC_const_cb，用于在对话框创建时初始化一个字符串控件的值。
> 4. 定义了一个do_it函数，用于创建对话框，并设置构造函数回调。
> 5. 定义了ufusr函数，用于在NX中初始化和调用do_it。
> 6. 定义了ufusr_ask_unload函数，用于卸载用户模块。
>
> 代码流程如下：
>
> 1. ufusr函数初始化UF模块。
> 2. 调用do_it函数创建对话框。
> 3. 在do_it中，使用UF_STYLER_create_dialog创建对话框，并设置构造函数回调GTAC_const_cb。
> 4. GTAC_const_cb在对话框创建时被调用，用于设置一个字符串控件的值。
> 5. ufusr函数最后终止UF模块。
> 6. ufusr_ask_unload用于卸载用户模块。
>
> 总的来说，这段代码展示了如何使用UIStyler创建一个简单的对话框，并设置控件的值。
>
