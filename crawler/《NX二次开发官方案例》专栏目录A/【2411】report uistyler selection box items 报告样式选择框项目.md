### 【2411】report uistyler selection box items 报告样式选择框项目

#### 代码

```cpp
    /*HEAD REPORT_UISTYLER_SELECTION_BOX_ITEMS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_SEL_BOX_0                ("SEL_BOX_0")  
    #define GTAC_CB_COUNT ( 1 + 1 ) /* Add 1 for the terminator 里海译:加1作为终结符 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，UF_print_syslog 是 V18 新增的功能。 */  
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
    static int GTAC_apply_cb ( int dialog_id, void * client_data,  
            UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            ii;  
        UF_STYLER_item_value_type_t  
            ask_value;  
        ask_value.item_attr = UF_STYLER_SUBITEM_VALUES;  
        ask_value.item_id = GTAC_SEL_BOX_0;  
        UF_CALL(UF_STYLER_ask_value( dialog_id, &ask_value));  
        for (ii = 0; ii < ask_value.count; ii++)  
            printf("%d. %s\n", ii+1, ask_value.value.strings[ii]);  
        if (ask_value.count > 0) UF_CALL(UF_STYLER_free_value(&ask_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        int  
            response;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[GTAC_CB_COUNT] = {  
                {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB, 0, GTAC_apply_cb},  
                {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_STYLER_create_dialog( "gtac.dlg", GTAC_cbs, GTAC_CB_COUNT,  
               NULL, &response));  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了回调函数GTAC_apply_cb，用于在用户选择下拉列表项目时执行，打印选择的值。
> 3. 主函数ufusr用于初始化NX，创建一个名为"gtac.dlg"的对话框，并为其绑定回调函数，然后等待用户操作，最后关闭NX。
> 4. ufusr_ask_unload函数用于卸载二次开发程序，设置为立即卸载。
> 5. 使用宏UF_CALL来封装UF函数调用，并检查错误。
> 6. 定义了下拉列表的ID和回调函数数组。
> 7. 使用NX的UF_Styler接口来创建对话框和下拉列表，并获取用户选择。
> 8. 使用NX的UF_UI接口来打印日志和错误信息。
> 9. 定义了立即卸载标志。
>
> 总体来说，这段代码提供了一个NX二次开发中对话框和下拉列表的基本示例，包含了错误处理、日志打印、回调函数等常见操作。
>
