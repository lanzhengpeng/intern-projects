### 【1849】register function to run after part open 注册在打开部件后运行的函数

#### 代码

```cpp
    /*HEAD REGISTER_FUNCTION_TO_RUN_AFTER_PART_OPEN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_attr.h>  
    #include <uf_exit.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的函数。 */  
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
    /* qq3123197280 */  
    static void my_open_callback(UF_callback_reason_e_t reason,  
        const void *application_data, void *user_data)  
    {  
        const tag_p_t  
            part = (tag_p_t)application_data;  
        int  
            now[2] = { -1, -1 };  
        char  
            dstr[21],  
            mstr[MAX_LINE_SIZE+1],  
            tstr[21],  
            user_name[30];  
        UF_ATTR_value_t  
            value = { UF_ATTR_string, NULL };  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(uc4595(1, user_name));  
        UF_CALL(uc4582(now, 2, dstr, tstr));  
        sprintf(mstr,"%s opened on %s at %s\n", user_name, dstr, tstr);  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_UI_write_listing_window(mstr));  
        value.value.string = mstr;  
        UF_CALL(UF_ATTR_assign(*part, "LAST_OPEN", value));  
        UF_terminate();  
    }  
    static UF_registered_fn_p_t function_id = NULL;  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_add_callback_function(UF_open_part_reason,  
                my_open_callback, NULL, &function_id));  
        UF_terminate();  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在出错时打印错误信息到系统日志和列表窗口。
> 2. 定义了一个回调函数my_open_callback，用于在打开部件后执行自定义操作。该函数会获取当前登录的用户名、日期和时间，然后将其组合成一条信息并打印到列表窗口。同时，还会将这条信息作为属性值赋给部件的"LAST_OPEN"属性。
> 3. 定义了一个注册函数ufsta，该函数会在NX启动时被调用。它将my_open_callback函数注册为打开部件的回调函数。
> 4. 使用宏UF_CALL来调用UF函数，并在出错时自动报告错误。
> 5. 使用UF提供的API函数来获取用户名、日期时间，以及打印信息到系统日志和列表窗口。
> 6. 使用UF提供的属性API来给部件赋值属性。
>
> 总之，这段代码通过在NX中注册一个回调函数，实现了在打开部件后自动记录相关信息的功能，并且提供了错误处理机制。
>
