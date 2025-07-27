### 【2212】report multi line text strings from uistyler dialog 报告UStyler对话框的多行文本字符串

#### 代码

```cpp
    /* HEAD REPORT_MULTI_LINE_TEXT_STRINGS_FROM_USTYLER_DIALOG  CCC UFUN 里海译:翻译结果如下：

多行文本字符串从UStyler对话框 */  
    /*----------------------------------------------------------  
    To test this, create a UIStyler dialog, change the "CHANGE"  
    to "GTAC_MTXT".  
    Use OK, Apply and Cancel.  
    Add a multi-line text box.  
    Save it as GTAC_MTXT.  
    Put GTAC_MTXT.dlg in a valid "application" directory.  
    ----------------------------------------------------------*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_defs.h>  
    #include <uf_styler.h>   
    #define GTAC_MTXT_MTXT_0               ("MTXT_0")  
    #define GTAC_MTXT_DIALOG_OBJECT_COUNT  ( 1 )  
    #define GTAC_MTXT_CB_COUNT ( 1 + 1 ) /* Add 1 for the terminator 里海译:添加1作为终结符。 */  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    int GTAC_MTXT_apply_cb ( int dialog_id,  
                 void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data);  
    static UF_STYLER_callback_info_t GTAC_MTXT_cbs[GTAC_MTXT_CB_COUNT] =   
    {  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB        , 0, GTAC_MTXT_apply_cb},  
     {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
    };  
    static void do_it(void)  
    {  
         int    
             response = 0;  
         UF_CALL( UF_STYLER_create_dialog ( "GTAC_MTXT.dlg",  
               GTAC_MTXT_cbs,      /* Callbacks from dialog 里海译:对话框的回调 */  
               GTAC_MTXT_CB_COUNT, /* number of callbacks*/  
               NULL,        /* This is your client data 里海译:这是您的客户数据。 */  
               &response ) );  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize()))   
        {  
                    return;  
        }  
        else  
        {  
            do_it();  
        }  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }  
    int GTAC_MTXT_apply_cb ( int dialog_id, void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            inx = 0;  
        UF_STYLER_item_value_type_t  
            ask_value;  
        ask_value.item_attr = UF_STYLER_VALUE;  
        ask_value.item_id = GTAC_MTXT_MTXT_0;   
        ask_value.value.integer = UF_STYLER_MULTI_LINE_TEXT_TYPE;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        WRITE_D( ask_value.count );  
        for (inx = 0; inx < ask_value.count; inx++ )  
        {  
            WRITE( ask_value.value.strings[inx] );  
            WRITE( "\n" );  
        }  
        UF_free_string_array( ask_value.count, ask_value.value.strings );  
        return (UF_UI_CB_CONTINUE_DIALOG);   
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的一个示例，主要功能是测试UStyler对话框中多行文本字符串的回调。
>
> 主要功能包括：
>
> 1. 定义了GTAC_MTXT_MTXT_0常量，表示对话框中多行文本控件的ID。
> 2. 定义了GTAC_MTXT_cbs回调数组，其中包含了一个对话框ID为UF_STYLER_DIALOG_INDEX,回调类型为UF_STYLER_APPLY_CB的回调函数GTAC_MTXT_apply_cb。
> 3. GTAC_MTXT_apply_cb回调函数会在用户点击OK或Apply按钮时被调用。在该函数中，通过UF_STYLER_ask_value获取多行文本控件的值，并将每行文本输出到列表窗口和日志文件。
> 4. ufusr函数是NX二次开发的主入口函数，在其中调用do_it函数创建并显示对话框。
> 5. do_it函数使用UF_STYLER_create_dialog创建并显示对话框，并传入GTAC_MTXT_cbs回调数组。
> 6. ufusr_ask_unload函数用于卸载二次开发，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
> 7. 使用了宏ECHO和WRITE_D用于输出信息到列表窗口和日志文件。
> 8. 使用了宏UF_CALL用于封装NX函数调用，并在出错时输出错误信息。
>
> 总的来说，这段代码通过UStyler对话框和多行文本控件，实现了获取用户输入并打印的功能，并展示了NX二次开发的基本流程。
>
