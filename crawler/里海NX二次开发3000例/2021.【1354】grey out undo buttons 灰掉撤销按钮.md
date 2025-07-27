### 【1354】grey out undo buttons 灰掉撤销按钮

#### 代码

```cpp
    /*HEAD GREY_OUT_UNDO_BUTTONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_mb.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的提示，UF_print_syslog是在V18版本中新增的。因此，我的回答仅限于翻译，不会添加任何评论或解释：

UF_print_syslog在V18版本中是新增的。 */  
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
    static void pause_execution(char *prompt)  
    {  
        int  
            resp = 1;  
        char  
            options[1][38] = { "Continue" };  
        uc1603(prompt, resp, options, 1);  
    }  
    static void do_it(void)  
    {  
        int  
            button_id,  
            ii;  
        UF_MB_state_t  
            state[3];  
        char  
            *button_names[3] = { "UG_VIEW_POPUP_UNDO",  
                                 "UG_EDIT_UNDO",  
                                 "UG_EDIT_UNDO_LIST" };  
        for (ii = 0; ii < 3; ii++)  
        {  
            UF_CALL(UF_MB_ask_button_id(button_names[ii], &button_id));  
            UF_CALL(UF_MB_ask_button_sensitivity(button_id, &state[ii]));  
            if (state[ii] != UF_MB_OFF)  
                UF_CALL(UF_MB_set_button_sensitivity(button_id, UF_MB_OFF));  
        }  
        pause_execution("UNDO is greyed out!");  
        for (ii = 0; ii < 3; ii++)  
        {  
            if (state[ii] == UF_MB_OFF) continue;  
            UF_CALL(UF_MB_ask_button_id(button_names[ii], &button_id));  
            UF_CALL(UF_MB_set_button_sensitivity(button_id, UF_MB_ON));  
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

> 这段代码是用于NX二次开发的用户函数，主要功能是禁用NX中与撤销相关的三个按钮，并提示用户“撤销已禁用”。具体介绍如下：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数失败时，记录错误信息并打印到日志窗口。
> 2. 定义了一个暂停执行函数pause_execution，用于显示提示信息，并等待用户点击“继续”。
> 3. 定义了主函数do_it，该函数首先获取与撤销相关的三个按钮的ID，然后获取它们的灵敏度状态，并将它们设置为禁用。接着，通过pause_execution函数提示用户“撤销已禁用”。最后，将按钮的灵敏度状态恢复为启用。
> 4. 定义了ufusr函数，这是用户函数的入口点。该函数首先初始化UF环境，然后调用do_it函数，最后终止UF环境。
> 5. 定义了ufusr_ask_unload函数，该函数返回立即卸载用户函数的标志。
>
> 综上所述，这段代码的主要作用是演示如何通过UF函数控制NX界面上按钮的显示状态，并通过暂停执行来提示用户。
>
