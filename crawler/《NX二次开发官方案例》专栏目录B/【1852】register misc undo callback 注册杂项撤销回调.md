### 【1852】register misc undo callback 注册杂项撤销回调

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_undo.h>  
    #include <stdarg.h>  
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
    static UF_UNDO_misc_cb_ret_t pre_func(UF_UNDO_mark_id_t mark_id, void *user_data)  
    {  
        ECHO("before setting mark_id = %d\n", mark_id);  
        ECHO("UF_UNDO_MISC_CB_ANY_MARK = %d (see PR 6456678)\n", UF_UNDO_MISC_CB_ANY_MARK);  
        return UF_UNDO_misc_cb_continue;  
    }  
    static UF_UNDO_misc_cb_ret_t post_func(UF_UNDO_mark_id_t mark_id, void *user_data)  
    {  
        ECHO("after setting mark_id = %d\n", mark_id);  
        return UF_UNDO_misc_cb_continue;  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        UF_UNDO_misc_cb_id_t  
            id[2];  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_register_misc_cb(UF_UNDO_misc_cb_set_pre,  
            UF_UNDO_MISC_CB_ANY_MARK, UF_UNDO_any_vis, pre_func, NULL, &id[0]));  
        UF_CALL(UF_UNDO_register_misc_cb(UF_UNDO_misc_cb_set_post,  
            UF_UNDO_MISC_CB_ANY_MARK, UF_UNDO_any_vis, post_func, NULL, &id[1]));  
        for (ii = 0; ii < 5; ii++)  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL, &mark_id));  
            ECHO("Set mark_id = %d\n", mark_id);  
        }  
        UF_CALL(UF_UNDO_unregister_misc_cb(id[0]));  
        UF_CALL(UF_UNDO_unregister_misc_cb(id[1]));  
    }  
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

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是演示如何在NX中注册和注销UNDO操作的回调函数，并在执行UNDO操作前和后进行日志打印。
>
> 主要功能包括：
>
> 1. 定义了两个回调函数pre_func和post_func，分别在UNDO操作之前和之后调用，用于打印日志信息。
> 2. 在do_it函数中，使用UF_UNDO_register_misc_cb函数注册了两个回调函数，一个用于UNDO操作前，另一个用于UNDO操作后。
> 3. 在do_it函数中，通过调用UF_UNDO_set_mark函数设置5个UNDO标记点。
> 4. 在每个UNDO标记点设置前后，会调用相应的回调函数，打印日志信息。
> 5. 最后，使用UF_UNDO_unregister_misc_cb函数注销了前面注册的两个回调。
> 6. 在ufusr函数中，初始化NX Open环境，调用do_it函数执行上述功能，最后终止NX Open环境。
> 7. ufusr_ask_unload函数用于在插件卸载时立即清理资源。
>
> 总体来说，这段代码通过UNDO操作的回调机制，实现了在UNDO操作前后添加自定义处理的功能，并进行了日志记录，以演示NX Open二次开发中UNDO相关API的使用方法。
>
