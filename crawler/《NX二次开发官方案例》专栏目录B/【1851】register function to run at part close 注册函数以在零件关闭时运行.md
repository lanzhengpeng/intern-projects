### 【1851】register function to run at part close 注册函数以在零件关闭时运行

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
    static void my_close_callback(UF_callback_reason_e_t reason,  
        const void *application_data, void *user_data)  
    {  
        const tag_p_t  
            part = (tag_p_t)application_data;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        if (UF_CALL(UF_initialize())) return;  
        UF_PART_ask_part_name(*part, part_name);  
        ECHO("closing %s\n", part_name);  
        UF_terminate();  
    }  
    static UF_registered_fn_p_t function_id = NULL;  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        if (function_id == NULL)  
        {  
            if (!UF_CALL(UF_add_callback_function(UF_close_part_reason,  
                my_close_callback, NULL, &function_id)))  
                ECHO("Added close part callback\n");  
        }  
        else  
        {  
            if (!UF_CALL(UF_remove_callback_function(function_id)))  
            {  
                ECHO("Removed close part callback", TRUE);  
                function_id = NULL;  
            }  
        }  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_SEL_DIALOG);  
    }

```

#### 代码解析

> 这是NX二次开发代码，其主要功能是管理NX的部件关闭回调函数。具体介绍如下：
>
> 1. 函数介绍：ECHO: 打印信息到NX的日志窗口和系统日志。report_error: 如果UF函数调用失败，报告错误信息。my_close_callback: 部件关闭的回调函数，打印关闭的部件名称。ufusr: NX二次开发的主入口函数。用于添加或移除部件关闭的回调函数。ufusr_ask_unload: 定义了当二次开发库被卸载时是否显示提示框。
> 2. ECHO: 打印信息到NX的日志窗口和系统日志。
> 3. report_error: 如果UF函数调用失败，报告错误信息。
> 4. my_close_callback: 部件关闭的回调函数，打印关闭的部件名称。
> 5. ufusr: NX二次开发的主入口函数。用于添加或移除部件关闭的回调函数。
> 6. ufusr_ask_unload: 定义了当二次开发库被卸载时是否显示提示框。
> 7. 功能说明：通过ufusr函数，用户可以添加或移除部件关闭的回调函数my_close_callback。添加回调函数时，将my_close_callback注册到部件关闭事件，并打印提示信息。移除回调函数时，将my_close_callback从部件关闭事件中注销，并打印提示信息。my_close_callback会打印出关闭的部件名称。通过ufusr_ask_unload函数，当二次开发库被卸载时，NX会显示提示框。
> 8. 通过ufusr函数，用户可以添加或移除部件关闭的回调函数my_close_callback。
> 9. 添加回调函数时，将my_close_callback注册到部件关闭事件，并打印提示信息。
> 10. 移除回调函数时，将my_close_callback从部件关闭事件中注销，并打印提示信息。
> 11. my_close_callback会打印出关闭的部件名称。
> 12. 通过ufusr_ask_unload函数，当二次开发库被卸载时，NX会显示提示框。
> 13. 代码逻辑：ufusr函数首先初始化NX API。如果回调函数尚未注册，则调用UF_add_callback_function添加my_close_callback到部件关闭事件，并打印提示信息。如果回调函数已注册，则调用UF_remove_callback_function移除my_close_callback，并打印提示信息。最后，终止NX API。
> 14. ufusr函数首先初始化NX API。
> 15. 如果回调函数尚未注册，则调用UF_add_callback_function添加my_close_callback到部件关闭事件，并打印提示信息。
> 16. 如果回调函数已注册，则调用UF_remove_callback_function移除my_close_callback，并打印提示信息。
> 17. 最后，终止NX API。
> 18. 注意事项：使用了UF错误报告宏UF_CALL来简化错误处理。my_close_callback中重新初始化了NX API，这是不必要的，可以直接使用ufusr中初始化的NX API。function_id作为全局变量存储回调函数的ID，可以考虑使用更好的管理方式。ufusr_ask_unload函数默认会显示提示框，可以根据实际需要修改。
> 19. 使用了UF错误报告宏UF_CALL来简化错误处理。
> 20. my_close_callback中重新初始化了NX API，这是不必要的，可以直接使用ufusr中初始化的NX API。
> 21. function_id作为全局变量存储回调函数的ID，可以考虑使用更好的管理方式。
> 22. ufusr_ask_unload函数默认会显示提示框，可以根据实际需要修改。
>
> 综上所述，这段代码提供了管理NX部件关闭回调函数的功能，通过简单的接口实现了添加/移除回调函数的逻辑。
>
