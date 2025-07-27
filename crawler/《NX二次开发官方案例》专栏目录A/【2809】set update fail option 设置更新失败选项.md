### 【2809】set update fail option 设置更新失败选项

#### 代码

```cpp
    /*HEAD SET_UPDATE_FAIL_OPTION CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译如下：

注意：UF_print_syslog在V18中是新增的，请仅回答翻译，无需添加其他内容。 */  
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
    static logical prompt_for_update_fail_option(char *prompt, int *selection)  
    {  
        int  
            resp;  
        char  
            options[6][38] = { "Ask",  
                               "Undo",  
                               "Suppress",  
                               "Suppress All",  
                               "Accept",  
                               "Accept All" };  
        resp = *selection + 1;  
        resp = uc1603(prompt, resp, options, 6);  
        if ((resp > 4) && (resp < 11))  
        {  
            *selection = resp - 5;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        UF_MODL_update_option_t  
            option;  
    /*  This may not return the correct results - see PR 5169001 里海译:这可能不会返回正确的结果 - 请参阅 PR 5169001。 */  
        UF_CALL(UF_MODL_ask_update_fail_option(&option));  
        if (prompt_for_update_fail_option("Failure During Update Action",  
            (int *)&option)) UF_CALL(UF_MODL_set_update_fail_option(option));  
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

> 这段代码是NX二次开发的一个示例，主要功能是处理更新失败选项。其中，ufusr函数是NX二次开发的入口函数，用于初始化和调用业务逻辑函数do_it，然后终止。在do_it函数中，首先调用UF_MODL_ask_update_fail_option获取当前的更新失败选项，然后调用prompt_for_update_fail_option函数弹窗提示用户选择更新失败选项，用户选择后，调用UF_MODL_set_update_fail_option设置更新失败选项。prompt_for_update_fail_option函数用于弹窗提示用户选择更新失败选项，包括Ask、Undo、Suppress、Suppress All、Accept、Accept All。此外，report_error函数用于打印错误信息到syslog和NX listing窗口。整体上，这段代码通过调用NX的UF_MODL模块获取和设置更新失败选项，实现了处理更新失败选项的功能。
>
