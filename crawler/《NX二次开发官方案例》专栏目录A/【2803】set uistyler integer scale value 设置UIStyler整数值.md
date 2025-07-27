### 【2803】set uistyler integer scale value 设置UIStyler整数值

#### 代码

```cpp
    /*HEAD SET_UISTYLER_INTEGER_SCALE_VALUE CCC UFUN */  
    /*  To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Change the dialog's Prefix Name to "GTAC"  
        - Specify the Constructor Callback as GTAC_"const_cb"  
        - Add an Integer Scale object  
        - Save Dialog As "gtac"  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_SCALE_INT_0                     ("SCALE_INT_0")  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的函数。文档要求只回答翻译结果，不要添加任何其他内容。因此，我的回答是：

UF_print_syslog 是 V18 版本中新增的函数。 */  
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
        set_value.item_id = GTAC_SCALE_INT_0;  
        set_value.value.integer = 3; /* Setting the default integer 里海译:设置默认整数 */  
        set_value.count = 1;  
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

> 这是段NX Open UI Styler二次开发代码，主要功能是创建一个对话框，其中包含一个整数比例尺控件，并将比例尺控件的默认值设置为3。
>
> 代码的主要部分包括：
>
> 1. 定义了对话框的回调函数GTAC_const_cb，用于在对话框创建时初始化整数比例尺控件的值。
> 2. 在GTAC_const_cb函数中，使用UF_STYLER_set_value函数将整数比例尺控件的值设置为3。
> 3. 定义了do_it函数，用于创建对话框。其中，使用UF_STYLER_create_dialog函数创建对话框，并传入GTAC_cbs回调数组，以便在对话框创建时调用GTAC_const_cb函数。
> 4. ufusr函数是NX Open的入口函数，在NX启动时调用。在该函数中，先调用UF_initialize进行初始化，然后调用do_it函数创建对话框，最后调用UF_terminate终止。
> 5. ufusr_ask_unload函数用于在卸载NX Open时调用，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 总的来说，这段代码通过UI Styler创建了一个包含整数比例尺控件的对话框，并设置了比例尺控件的默认值。
>
