### 【3045】use variable item id in uistyler 在Uistyler中使用变量项目ID

#### 代码

```cpp
    /*HEAD USE_VARIABLE_ITEM_ID_IN_UISTYLER CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            ii;  
        char  
            item_id[20],  
            label_string[20];  
        UF_STYLER_item_value_type_t  
            set_value;  
        set_value.item_attr = UF_STYLER_LABEL;  
        set_value.item_id = item_id;  
        set_value.value.string = label_string;  
        for (ii = 0; ii < 3; ii++)  
        {  
            sprintf(item_id, "REAL_%d", ii);  
            sprintf(label_string, "Label %d", ii+1);  
            UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        }  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t GTAC_cbs[2] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }};  
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

> 根据代码，这是一段NX Open Styler二次开发代码，其主要功能是创建一个对话框，并在其中添加3个标签。
>
> 主要步骤包括：
>
> 1. 包含必要的头文件，例如uf.h, uf_ui.h, uf_styler.h等。
> 2. 定义一个错误报告函数report_error，用于打印错误信息。
> 3. 定义一个回调函数GTAC_const_cb，用于在对话框创建后设置3个标签的文本。
> 4. 定义一个函数do_it，用于创建对话框并设置其回调。
> 5. 在ufusr函数中，初始化NX Open，调用do_it函数创建对话框，然后终止NX Open。
> 6. 定义一个卸载函数ufusr_ask_unload，用于立即卸载二次开发代码。
> 7. ufusr函数是NX二次开发的标准入口函数。
> 8. GTAC_const_cb回调函数用于在对话框创建后，设置3个标签的文本为"Label 1"、“Label 2”、“Label 3”。
> 9. do_it函数中，通过UF_STYLER_create_dialog函数创建一个名为"gtac.dlg"的对话框，并设置GTAC_const_cb为对话框的构造回调。
> 10. ufusr函数是NX二次开发的入口函数，用于初始化和终止NX Open。
> 11. ufusr_ask_unload函数用于立即卸载二次开发代码。
>
> 综上所述，这段代码的主要功能是创建一个包含3个标签的对话框。
>
