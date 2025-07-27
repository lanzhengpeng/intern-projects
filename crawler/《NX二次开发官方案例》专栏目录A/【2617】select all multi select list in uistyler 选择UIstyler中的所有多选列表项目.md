### 【2617】select all multi select list in uistyler 选择UIstyler中的所有多选列表项目

#### 代码

```cpp
    /*HEAD SELECT_ALL_MULTI_SELECT_LIST_IN_UISTYLER CCC UFUN */  
    /* This selects or unselects all items in a multi-select list.                里海译:翻译：这会选择或取消选择一个多选列表中的所有项目。 */  
    /* These include files are needed for the following template code.            里海译:这些包含文件对于以下模板代码是必需的。 */  
    /*  To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Change the dialog's Prefix Name to "GTAC"  
        - Add a Multi List object  
        - Type in a few lines to add to your Multi List  
        - Add a Push Button object  
        - Change the Push Button label to "Select/Unselect All"  
        - Save Dialog As "$UGII_USER_DIR/application/gtac_multi_toggle"  
        - Execute the program and it will bring up the dialog  
        - Click on "Select/Unselect All" and it will Select all the items.  
    */  
    #include <stdio.h>   
    #include <uf.h>  
    #include <uf_defs.h>  
    #include <uf_exit.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_mb.h>   
    #include <gtac_multi_toggle.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，可以翻译为：

注意：UF_print_syslog是在V18版本中新增的，请只回答翻译，不要添加其他无关内容。 */  
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
    #define GTAC_CB_COUNT ( 2 + 1 ) /* Add 1 for the terminator 里海译:加上1作为终止符。 */  
    static UF_STYLER_callback_info_t GTAC_cbs[GTAC_CB_COUNT] =   
    {  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB        , 0, GTAC_apply_cb},  
     {GTAC_ACTION_1         , UF_STYLER_ACTIVATE_CB     , 0, GTAC_action_1_act_cb},  
     {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
    };  
    #define DISPLAY_FROM_USER_EXIT  
    #ifdef DISPLAY_FROM_USER_EXIT  
    /* qq3123197280 */  
    extern void ufusr (char *param, int *retcode, int rlen)  
    {  
        int  response   = 0;  
        int  error_code = 0;  
        if ( ( UF_initialize() ) != 0 )   
               return;  
        if ( ( error_code = UF_STYLER_create_dialog ( "gtac_multi_toggle.dlg",  
               GTAC_cbs,      /* Callbacks from dialog 里海译:对不起，我不太清楚你想要我翻译什么。 */  
               GTAC_CB_COUNT, /* number of callbacks*/  
               NULL,        /* This is your client data 里海译:这是您的客户数据。 */  
               &response ) ) != 0 )  
        {  
              char fail_message[133];  
              /* Get the user function fail message based on the fail code.*/  
              UF_get_fail_message(error_code, fail_message);  
              UF_UI_set_status (fail_message);  
              printf ( "%s\n", fail_message );   
        }  
        UF_terminate();                               
        return;  
    }  
    extern int ufusr_ask_unload (void)  
    {  
         /* unload immediately after application exits*/  
         return ( UF_UNLOAD_IMMEDIATELY );  
         /*via the unload selection dialog... */  
         /*return ( UF_UNLOAD_SEL_DIALOG );   */  
         /*when UG terminates...              */  
         /*return ( UF_UNLOAD_UG_TERMINATE ); */  
    }  
    extern void ufusr_cleanup (void)  
    {  
        return;  
    }  
    #endif /* DISPLAY_FROM_USER_EXIT 里海译:DISPLAY_FROM_USER_EXIT */   
    /* qq3123197280 */  
    int GTAC_apply_cb ( int dialog_id,  
                 void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
         /* Make sure User Function is available. 里海译:确保用户功能可用。 */    
         if ( UF_initialize() != 0)   
              return ( UF_UI_CB_CONTINUE_DIALOG );  
         /* ---- Enter your callback code here ----- 里海译:翻译如下：

输入您的回调代码。 */  
        /* (Right now, it does not do anything) 里海译:目前，它还无法完成任何操作。 */  
         UF_terminate ();  
        /* Callback acknowledged, do not terminate dialog                 里海译:确认收到回调，不要终止对话 */  
        /* A return value of UF_UI_CB_EXIT_DIALOG will not be accepted    里海译:返回值UF_UI_CB_EXIT_DIALOG将不被接受。 */  
        /* for this callback type.  You must respond to your apply button.*/  
        return (UF_UI_CB_CONTINUE_DIALOG);   
    }  
    /* qq3123197280 */  
    int GTAC_action_1_act_cb ( int dialog_id,  
                 void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            ask_value;  
         /* Make sure User Function is available. 里海译:确保用户功能可用。 */    
         if ( UF_initialize() != 0)   
              return ( UF_UI_CB_CONTINUE_DIALOG );  
         /* ---- Enter your callback code here ----- 里海译:请输入您的回调代码。 */  
        ask_value.item_attr = UF_STYLER_VALUE;  
        ask_value.item_id = GTAC_MULTI_LIST_0;  
        ask_value.subitem_index = UF_STYLER_NO_SUB_INDEX;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &ask_value));  
         UF_terminate ();  
        /* Callback acknowledged, do not terminate dialog 里海译:翻译Callback acknowledged, do not terminate dialog为：

已确认回调，不要结束对话。 */  
        return (UF_UI_CB_CONTINUE_DIALOG);   
        /* or Callback acknowledged, terminate dialog.    里海译:好的，以下是翻译：

"Callback acknowledged, terminate dialog." */  
        /* return ( UF_UI_CB_EXIT_DIALOG );               里海译:根据上下文，return ( UF_UI_CB_EXIT_DIALOG ); 的翻译是“返回 ( 用户界面回调 - 退出对话框 )”。 */  
    }

```

#### 代码解析

> 这是段NX二次开发代码，主要功能是在NX的用户界面Styler中实现一个多选列表，并添加一个按钮来选择或取消选择列表中的所有项目。
>
> 主要内容包括：
>
> 1. 定义了回调函数GTAC_apply_cb和GTAC_action_1_act_cb，分别用于响应“应用”按钮和“选择/取消选择所有”按钮的点击事件。
> 2. GTAC_action_1_act_cb函数通过调用UF_STYLER_set_value来选择或取消选择多选列表中的所有项目。
> 3. 通过UF_STYLER_create_dialog创建一个对话框，并传入回调函数数组GTAC_cbs，用于处理对话框事件。
> 4. 在ufusr函数中调用UF_STYLER_create_dialog来显示对话框。
> 5. 使用宏UF_CALL来记录函数调用失败时的错误信息。
> 6. 使用UF_print_syslog和UF_UI_write_listing_window来打印错误信息到系统日志和控制台窗口。
> 7. 定义了GTAC_CB_COUNT常量来记录回调函数数组的长度。
> 8. 定义了GTAC_cbs数组来保存回调函数信息，包括对话框索引、回调类型、回调函数指针等。
> 9. 使用DISPLAY_FROM_USER_EXIT宏来控制是否从用户出口函数ufusr中调用对话框。
> 10. 使用UF_terminate终止UF初始化。
>
> 以上是代码的主要功能介绍。
>
