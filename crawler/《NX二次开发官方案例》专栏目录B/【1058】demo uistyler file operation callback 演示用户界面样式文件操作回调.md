### 【1058】demo uistyler file operation callback 演示用户界面样式文件操作回调

#### 代码

```cpp
    /*HEAD DEMO_UISTYLER_FILE_OPERATION_CALLBACK CCC UFUN */  
    /*  To test this example - create a simple dialog:  
        1.  Select Application->User Interface Styler  
        2.  Change the Prefix Name "CHANGE" to "GTAC"  
        3.  Select a Dialog Type of Top  
        4.  Select the Callbacks tab and enter "file_cb" for File Operation  
        5.  Save the dialog to your application directory as gtac.dlg  
        You will also need to create gtac.men in your startup directory  
        containing these lines:  
        VERSION 120  
        EDIT UG_GATEWAY_MAIN_MENUBAR  
        BEFORE UG_HELP  
          CASCADE_BUTTON UISTYLER_DLG_CASCADE_BTN  
          LABEL Dialog Launcher  
        END_OF_BEFORE  
        MENU UISTYLER_DLG_CASCADE_BTN  
          BUTTON TOP_DIALOG_BTN  
          LABEL Display GTAC dialog  
          ACTIONS gtac.dlg  
        END_OF_MENU  
        Also put the linked program (.sl, .so, or .dll) in the startup directory.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_mb.h>  
    #include <uf_styler.h>  
    #include <uf_exit.h>  
    #define GTAC_CB_COUNT (1 + 1 ) /* Add 1 for the terminator 里海译:加1用于终止符 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是在V18中新增的函数。 */  
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
    static int GTAC_file_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        if (UF_CALL(UF_initialize())) return (UF_UI_CB_CONTINUE_DIALOG );  
        switch (callback_data->value.notify->fileop.state)  
        {  
            case UF_STYLER_ENTER_FILE_OPERATION:  
                WRITE("Entering ");  
                break;  
            case UF_STYLER_EXIT_FILE_OPERATION:  
                WRITE("Exiting ");  
                break;  
            default:  
                WRITE_D(callback_data->value.notify->fileop.state);  
                break;  
        }  
        switch (callback_data->value.notify->fileop.id)  
        {  
            case UF_UI_FILE_NEW:  
                WRITE_D(UF_UI_FILE_NEW);  
                break;  
            case UF_UI_FILE_OPEN:  
                WRITE_D(UF_UI_FILE_OPEN);  
                break;  
            case UF_UI_FILE_SAVE:  
                WRITE_D(UF_UI_FILE_SAVE);  
                break;  
            case UF_UI_FILE_SAVE_AS:  
                WRITE_D(UF_UI_FILE_SAVE_AS);  
                break;  
            case UF_UI_FILE_SAVE_ALL:  
                WRITE_D(UF_UI_FILE_SAVE_ALL);  
                break;  
            case UF_UI_FILE_CLOSE:  
                WRITE_D(UF_UI_FILE_CLOSE);  
                break;  
            case UF_UI_FILE_QUIT:  
                WRITE_D(UF_UI_FILE_QUIT);  
                break;  
            case UF_UI_FILE_SAVE_AND_EXIT:  
                WRITE_D(UF_UI_FILE_SAVE_AND_EXIT);  
                break;  
            case UF_UI_FILE_CHANGE_PART:  
                WRITE_D(UF_UI_FILE_CHANGE_PART);  
                break;  
            case UF_UI_FILE_EXECUTE:  
                WRITE_D(UF_UI_FILE_EXECUTE);  
                break;  
            case UF_UI_FILE_REOPEN:  
                WRITE_D(UF_UI_FILE_REOPEN);  
                break;  
            case UF_UI_FILE_SAVEALL_AND_CLOSE:  
                WRITE_D(UF_UI_FILE_SAVEALL_AND_CLOSE);  
                break;  
            case UF_UI_FILE_SAVE_AND_CLOSE:  
                WRITE_D(UF_UI_FILE_SAVE_AND_CLOSE);  
                break;  
            case UF_UI_FILE_SAVEAS_AND_CLOSE:  
                WRITE_D(UF_UI_FILE_SAVEAS_AND_CLOSE);  
                break;  
            default:  
                WRITE_D(callback_data->value.notify->fileop.id);  
                break;  
        }  
        UF_terminate();  
        return (UF_UI_CB_CONTINUE_DIALOG );  
    }  
    static UF_STYLER_callback_info_t GTAC_cbs[GTAC_CB_COUNT] =  
    {  
        {UF_STYLER_DIALOG_INDEX, UF_STYLER_FILEOP_CB, 0, GTAC_file_cb},  
        {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
    };  
    static UF_MB_styler_actions_t the_actions[] = {  
        { "gtac.dlg",  NULL,   GTAC_cbs,  UF_MB_STYLER_IS_TOP },  
        { NULL,  NULL,  NULL,  0 } /* This is a NULL terminated list 里海译:这是一个以空字符结束的列表。 */  
    };  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_MB_add_styler_actions(the_actions));  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是NX Open UI Styler的二次开发示例，其主要功能是实现了文件操作回调函数。作者在代码中通过回调函数GTAC_file_cb来响应文件操作，如打开、保存、关闭等，并在回调函数中记录文件操作的状态和类型。在文件操作之前和之后，回调函数会分别输出"Entering "和"Exiting "，从而在列表窗口中记录文件操作的过程。此外，作者还在代码中定义了一个UF_MB_styler_actions_t结构体数组，用于向UI Styler添加回调函数，从而实现与UI Styler的交互。总体来说，这段代码通过文件操作回调函数，记录并输出了UI Styler的文件操作信息，为二次开发提供了参考和示范。
>
