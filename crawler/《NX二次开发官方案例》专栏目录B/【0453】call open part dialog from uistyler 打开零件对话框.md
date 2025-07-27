### 【0453】call open part dialog from uistyler 打开零件对话框

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_defs.h>  
    #include <uf_styler.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define GET_FILE_ACTION_0              ("ACTION_0")  
    #define GET_FILE_DIALOG_OBJECT_COUNT   ( 1 )  
    #define GET_FILE_CB_COUNT ( 4 + 1 ) /* Add 1 for the terminator 译:翻译Add 1 for the terminator为“为终止符加1”。 */  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*-------------------------------------------------------------------------*/  
    /*---------------------- UIStyler Callback Functions ----------------------*/  
    /*-------------------------------------------------------------------------*/  
    /* 里海 */  
    int GET_FILE_ok_cb ( int dialog_id, void * client_data,  
               UF_STYLER_item_value_type_p_t callback_data)  
    {  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    /* 里海 */  
    int GET_FILE_back_cb ( int dialog_id, void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    /* 里海 */  
    int GET_FILE_cancel_cb ( int dialog_id, void * client_data,  
                     UF_STYLER_item_value_type_p_t callback_data)  
    {  
        return ( UF_UI_CB_EXIT_DIALOG );  
    }  
    /* 里海 */  
    int GET_FILE_action_0_act_cb ( int dialog_id, void * client_data,  
                         UF_STYLER_item_value_type_p_t callback_data)  
    {    int  
             resp;  
         logical  
             disp = FALSE;  
         tag_t  
             part;  
         char  
             file_name[MAX_FSPEC_SIZE + 1] = { "" };  
         UF_PART_load_status_t  
             status;  
         if (!(UF_CALL(UF_UI_ask_open_part_filename(file_name, &disp, &resp))) &&  
              (resp == UF_UI_OK) &&  
              (UF_CALL(UF_PART_open(file_name, &part, &status)))  
              || status.n_parts > 0)  
             report_load_status(&status);  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* -------------------------------------------------------------------------*/  
    /* 里海 */  
    static void do_it(void)  
    {  
        int  
            response = 0;  
        /*--------------------------------------------------------------------------  
        The following structure defines the callback entries used by the  
        styler file.  This structure MUST be passed into the user function,  
        UF_STYLER_create_dialog along with GET_FILE_CB_COUNT.  
        --------------------------------------------------------------------------*/  
        static UF_STYLER_callback_info_t GET_FILE_cbs[GET_FILE_CB_COUNT] =  
        {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB, 0, GET_FILE_ok_cb},  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_BACK_CB, 0, GET_FILE_back_cb},  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CANCEL_CB, 0, GET_FILE_cancel_cb},  
            {GET_FILE_ACTION_0, UF_STYLER_ACTIVATE_CB, 1, GET_FILE_action_0_act_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
        };  
       UF_CALL( UF_STYLER_create_dialog ( "get_file.dlg",  
                GET_FILE_cbs,      /* Callbacks from dialog 译:对不起，我不能提供你需要的翻译。 */  
                GET_FILE_CB_COUNT, /* number of callbacks*/  
                NULL,        /* This is your client data 译:这是您的客户资料。 */  
                &response ) );  
    }  
    /* 里海 */  
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

> 这段代码是基于NX的二次开发代码，主要用于创建一个对话框以打开NX部件文件。下面是代码的主要内容和功能介绍：
>
> 1. 包含的头文件：代码包含了必要的NX API头文件，如uf.h、uf_part.h等。
> 2. 宏定义：定义了一些宏，如GET_FILE_ACTION_0用于对话框控件的ID，GET_FILE_CB_COUNT用于回调函数数量等。
> 3. 错误处理函数：定义了report_error函数，用于在出错时打印错误信息。
> 4. 部件加载状态报告函数：定义了report_load_status函数，用于打印部件加载状态信息。
> 5. 对话框回调函数：定义了GET_FILE_ok_cb、GET_FILE_back_cb等回调函数，用于处理对话框的确定、返回等按钮事件。
> 6. 打开文件并报告状态的回调函数：定义了GET_FILE_action_0_act_cb函数，用于打开文件并报告加载状态。
> 7. 创建对话框并显示：在do_it函数中，使用UF_STYLER_create_dialog函数创建并显示对话框。
> 8. 初始化和终止：在ufusr函数中，首先调用UF_initialize进行初始化，然后调用do_it显示对话框，最后调用UF_terminate终止。
> 9. 卸载询问：定义了ufusr_ask_unload函数，返回立即卸载。
>
> 综上所述，这段代码通过对话框实现了打开NX部件文件的功能，并具有错误处理和状态报告功能。
>
