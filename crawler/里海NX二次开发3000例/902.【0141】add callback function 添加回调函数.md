### 【0141】add callback function 添加回调函数

#### 代码

```cpp
    /*HEAD ADD_CALLBACK_FUNCTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的提示，UF_print_syslog 是在 V18 版本中新增的函数。因此，翻译为：

注意：UF_print_syslog 是在 V18 版本中新增的函数，请只提供翻译，不要添加其他内容。 */  
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
    static void my_open_callback(UF_callback_reason_e_t reason,  
        const void *application_data, void *user_data)  
    {  
        const tag_p_t  
            part = (tag_p_t)application_data;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        if (UF_CALL(UF_initialize())) return;  
        UF_PART_ask_part_name(*part, part_name);  
        uc1601(part_name, TRUE);  
        UF_terminate();  
    }  
    static UF_registered_fn_p_t function_id = NULL;  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        if (function_id == NULL)  
        {  
            if (!UF_CALL(UF_add_callback_function(UF_open_part_reason,  
                my_open_callback, NULL, &function_id)))  
                uc1601("Added callback", TRUE);  
        }  
        else  
        {  
            if (!UF_CALL(UF_remove_callback_function(function_id)))  
            {  
                uc1601("Removed callback", TRUE);  
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

> 这段代码是NX二次开发的UF函数，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息到系统日志和控制台窗口。
> 2. 定义了一个回调函数my_open_callback，用于在打开部件时触发。该函数会获取打开的部件名称并显示。
> 3. 在ufusr函数中，通过UF_add_callback_function添加回调函数my_open_callback，在打开部件时触发。
> 4. ufusr函数还包含一个标志function_id，用于判断是否已添加回调。如果已添加，则调用UF_remove_callback_function移除回调。
> 5. 最后定义了ufusr_ask_unload函数，返回卸载模式。
>
> 这段代码的主要功能是，在打开部件时，通过回调函数获取部件名称并显示。同时提供了添加/移除回调函数的功能。
>
