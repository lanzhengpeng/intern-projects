### 【1192】exit ug session 退出UG会话

#### 代码

```cpp
    /*HEAD EXIT_UG_SESSION CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /*  Work around for PR 5938082 - define hidden prototype 里海译:对于PR 5938082，建议采用以下解决方法：

定义一个隐藏的原型。 */  
    extern UFUNEXPORT void UF_sys_exit ( void );  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

UF_print_syslog是V18版本新增的功能，请只提供翻译，不要添加任何额外内容。 */  
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
    static logical confirm_exit(void)  
    {  
        int  
            ii,  
            n,  
            resp;  
        tag_t  
            part;  
        char  
            *message[2] = { "You have open files that have been modified.",  
                            "Do you really want to exit?" };  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, NULL, NULL, NULL, 1, 0, 2 };  
        n = UF_PART_ask_num_parts();  
        for (ii = 0; ii < n; ii++)  
        {  
            part = UF_PART_ask_nth_part(ii);  
            if (UF_PART_is_modified(part)) break;  
        }  
        if (ii == n)  
        {  
            UF_CALL(UF_UI_message_dialog("Exit", UF_UI_MESSAGE_QUESTION,  
                &message[1], 1, TRUE, &buttons, &resp));  
        }  
        else  
        {  
            UF_CALL(UF_UI_message_dialog("Exit", UF_UI_MESSAGE_QUESTION,  
                message, 2, TRUE, &buttons, &resp));  
        }  
        return (2 - resp);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        if (confirm_exit())  
        {  
            UF_terminate();  
            UF_sys_exit ();  /*  See PR 5938082 里海译:根据您的要求，以下是翻译：

See PR 5938082 */  
        }  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要用于实现NX会话的退出功能。代码的主要功能包括：
>
> 1. 错误处理：定义了一个错误报告函数report_error，用于输出函数调用失败时的错误信息。
> 2. 退出确认：定义了一个确认退出函数confirm_exit，用于在退出前提示用户确认是否保存已修改的文件。
> 3. UF初始化：调用UF_initialize初始化UF环境。
> 4. 确认退出：调用confirm_exit确认用户是否要退出，若确认则调用UF_terminate终止UF环境，并调用UF_sys_exit退出NX会话。
> 5. 立即卸载：定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发应用程序支持立即卸载。
> 6. UF终止：在ufusr函数的末尾调用UF_terminate终止UF环境。
>
> 总的来说，这段代码提供了NX会话退出的一个二次开发示例，实现了用户确认、错误处理等功能。
>
