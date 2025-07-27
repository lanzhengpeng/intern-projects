### 【1325】freeze unfreeze all parts list rows 冻结解冻零件列表的所有行

#### 代码

```cpp
    /*HEAD FREEZE_UNFREEZE_ALL_PARTS_LIST_ROWS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_plist.h>  
    #include <uf_retiring.h>  
    #include <uf_tabnot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    static logical ask_lock_or_unlock(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Lock", NULL, "Unlock", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            pl_tag = NULL_TAG;  
        int  
            num;  
        pl_tag = UF_PLIST_ask_tag();  
        UF_CALL( UF_TABNOT_ask_nm_rows( pl_tag, &num) );  
        if (ask_lock_or_unlock("Lock or Unlock?", "Lock?"))  
        {  
            UF_CALL(UF_PLIST_set_frozen_level(pl_tag, num));  
            uc1601("should be frozen", TRUE);  
        }  
        else  
        {     
            UF_CALL(UF_PLIST_set_frozen_level(pl_tag, 0));  
            uc1601("unfrozen", TRUE);  
        }  
    }  
    /* qq3123197280 */  
    extern void ufusr(char *param, int *retcode, int paramLen)  
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

> 这是一段NX Open C++的二次开发代码，主要功能是锁定或解锁NX部件列表的所有行。代码的主要部分包括：
>
> 1. 包含了NX Open C++所需的头文件。
> 2. 定义了宏UF_CALL，用于调用NX API函数，并在出错时打印错误信息。
> 3. 定义了report_error函数，用于打印错误信息。
> 4. 定义了ask_lock_or_unlock函数，用于弹出一个对话框，让用户选择锁定或解锁。
> 5. 定义了do_it函数，用于获取当前部件列表的tag，查询行数，并根据用户选择锁定或解锁所有行。
> 6. 定义了ufusr函数，这是NX二次开发的入口函数。在函数中初始化NX，调用do_it执行功能，然后终止NX。
> 7. 定义了ufusr_ask_unload函数，用于在卸载时立即清理。
> 8. 使用UF_PRINT_SYSLOG打印系统日志是NX 18版本新增的功能。
>
> 这段代码通过对话框让用户选择锁定或解锁，然后调用NX API函数来实现锁定或解锁部件列表所有行的功能。
>
