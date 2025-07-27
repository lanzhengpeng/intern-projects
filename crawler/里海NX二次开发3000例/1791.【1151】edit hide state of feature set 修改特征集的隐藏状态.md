### 【1151】edit hide state of feature set 修改特征集的隐藏状态

#### 代码

```cpp
    /*HEAD EDIT_HIDE_STATE_OF_FEATURE_SET CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志消息。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static logical ask_yes_or_no(char *prompt, int *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "Yes", "No" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        char  
            fset_name[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        tag_t  
            fset_tag = NULL_TAG;  
        int  
            hide_state = TRUE;  
        while((prompt_for_text("Name of Feature Set", fset_name)) &&  
            (ask_yes_or_no("Hide Feature Set members?", &hide_state)))  
        {  
            UF_CALL(UF_MODL_ask_set_from_name(fset_name, &fset_tag));  
            WRITE("setting to ");  
            WRITE_D(hide_state);  
            UF_CALL(UF_MODL_edit_set_hide_state(fset_tag, &hide_state));  
            WRITE("the previous setting was ");  
            WRITE_D(hide_state);  
            UF_MODL_update();  
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

> 这段代码是一个NX Open C++程序，用于隐藏或显示NX中的特征集成员。
>
> 主要功能包括：
>
> 1. 通过提示用户输入特征集名称，并询问是否隐藏该特征集的成员，程序可以循环处理多个特征集。
> 2. 使用UF_MODL_ask_set_from_name函数根据特征集名称获取特征集tag。
> 3. 使用UF_MODL_edit_set_hide_state函数设置特征集成员的隐藏状态为隐藏或显示。
> 4. 每处理完一个特征集，程序会更新NX模型。
> 5. 通过UF_print_syslog和UF_UI_write_listing_window函数在日志窗口和NX日志中打印信息。
> 6. 使用自定义的report_error函数来处理函数调用失败并打印错误信息。
> 7. 使用宏WRITE和WRITE_D简化了在日志窗口打印字符串和整数的代码。
> 8. 主函数ufusr完成初始化、执行主体功能和终止NX Open的调用。
>
> 总的来说，这段代码实现了隐藏或显示NX特征集成员的功能，并提供了友好的用户交互。
>
