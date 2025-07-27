### 【2412】report uistyler text entry 报告UIStyler文本输入

#### 代码

```cpp
    /*HEAD REPORT_UISTYLER_TEXT_ENTRY CCC UFUN */  
    /* To create a dialog for use with this program:  
        Application-> UIStyler  
        change CHANGE to GTAC  
        enter ok callback:  ok_cb  
        add a string object  
        File-> Save As-> gtac  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_STR_0              ("STR_0")  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的功能。它用于打印系统日志。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int GTAC_ok_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            ask_value;  
        ask_value.item_attr = UF_STYLER_VALUE;  
        ask_value.item_id =  GTAC_STR_0;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        strcpy((char *)client_data, ask_value.value.string);  
        UF_CALL(UF_STYLER_free_value(&ask_value));  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        char  
            the_string[UF_UI_MAX_STRING_LEN+1];  
        UF_STYLER_callback_info_t GTAC_cbs[2] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB, 0, GTAC_ok_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }};  
        while (!UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, 2,  
            the_string, &resp)) && (resp == UF_UI_OK))  
        {  
            WRITE_S(the_string);  
        }  
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

> 这是NX Open UIStyler二次开发的一个示例代码，主要用于创建一个简单的字符串输入对话框。
>
> 主要功能包括：
>
> 1. 定义了一个报告错误函数report_error，用于打印错误信息和调用信息到系统日志和列表窗口。
> 2. 定义了一个写入字符串到列表窗口的函数write_string_to_listing_window。
> 3. 定义了对话框的OK回调函数GTAC_ok_cb，用于获取输入的字符串，并复制到client_data指向的缓冲区。
> 4. 定义了主函数do_it，用于循环创建对话框，直到用户取消。在每次循环中，会调用write_string_to_listing_window将输入的字符串写入列表窗口。
> 5. ufusr函数是NX二次开发程序的入口函数，在这里调用了do_it函数。
> 6. ufusr_ask_unload函数用于告诉NX立即卸载二次开发程序。
>
> 总体来说，这段代码通过UIStyler定义了一个简单的字符串输入对话框，实现了获取用户输入字符串并打印到列表窗口的功能。
>
