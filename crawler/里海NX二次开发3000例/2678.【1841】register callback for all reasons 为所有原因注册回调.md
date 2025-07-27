### 【1841】register callback for all reasons 为所有原因注册回调

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_exit.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void my_part_callback(UF_callback_reason_e_t reason,  
        const void *which_part, void *user_data)  
    {  
        tag_t  
            *part_ptr = (tag_t *)which_part;  
        char  
            *reason_name = (char *)user_data,  
            part_fspec[MAX_FSPEC_SIZE+1];  
        if (UF_CALL(UF_initialize())) return;  
        ECHO("In %s", reason_name);  
        switch (reason)  
        {  
            case UF_create_part_reason:  
                ECHO(" callback (UF_create_part_reason) ");  
                break;  
            case UF_open_part_reason:  
                ECHO(" callback (UF_open_part_reason) ");  
                break;  
            case UF_save_part_reason:  
                ECHO(" callback (UF_save_part_reason) ");  
                break;  
            case UF_close_part_reason:  
                ECHO(" callback (UF_close_part_reason) ");  
                break;  
            case UF_modified_part_reason:  
                ECHO(" callback (UF_modified_part_reason) ");  
                break;  
            case UF_rename_part_reason:  
                ECHO(" callback (UF_rename_part_reason) ");  
                break;  
            case UF_change_work_part_reason:  
                ECHO(" callback (UF_change_work_part_reason) ");  
                break;  
            default:  
                ECHO(" callback (Unknown) ");  
                break;  
        }  
        WRITE_D(reason);  
        if (*part_ptr == NULL_TAG) return;  
        UF_CALL(UF_PART_ask_part_name(*part_ptr, part_fspec));  
        WRITE_S(part_fspec);  
        ECHO("\n");  
        UF_terminate();  
    }  
    static void do_it(void)  
    {  
        UF_registered_fn_p_t  
            ftn_ids[7];  
        UF_CALL(UF_add_callback_function(UF_create_part_reason,  
            my_part_callback, "Create part", &ftn_ids[0]));  
        UF_CALL(UF_add_callback_function(UF_open_part_reason,  
            my_part_callback, "Open part", &ftn_ids[1]));  
        UF_CALL(UF_add_callback_function(UF_save_part_reason,  
            my_part_callback, "Save part", &ftn_ids[2]));  
        UF_CALL(UF_add_callback_function(UF_close_part_reason,  
            my_part_callback, "Close part", &ftn_ids[3]));  
        UF_CALL(UF_add_callback_function(UF_modified_part_reason,  
            my_part_callback, "Modified part", &ftn_ids[4]));  
        UF_CALL(UF_add_callback_function(UF_rename_part_reason,  
            my_part_callback, "Rename part", &ftn_ids[5]));  
        UF_CALL(UF_add_callback_function(UF_change_work_part_reason,  
            my_part_callback, "Change work part", &ftn_ids[6]));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是基于NX的二次开发，主要用于注册和实现零件事件的回调函数。
>
> 主要功能包括：
>
> 1. 定义了一个ECHO宏，用于输出信息到日志窗口。
> 2. 定义了一个report_error函数，用于报告函数调用错误。
> 3. 定义了一个write_string_to_listing_window函数，用于将字符串输出到日志窗口。
> 4. 定义了一个my_part_callback函数，作为零件事件回调函数。根据不同的零件事件，输出相关信息。
> 5. 定义了一个do_it函数，用于注册各种零件事件回调函数。
> 6. 定义了一个ufsta函数，用于初始化NX，调用do_it函数注册回调，然后终止NX。
> 7. 通过UF_add_callback_function接口，为创建、打开、保存、关闭、修改、重命名、切换工作零件等事件注册了my_part_callback回调函数。
> 8. 在my_part_callback中，根据不同的事件类型，输出事件名称，并获取零件名输出。
> 9. 通过UF_terminate终止NX的初始化。
>
> 总体而言，这段代码的主要目的是通过NX提供的零件事件机制，在零件生命周期中的关键点注册回调，用于监控零件状态的变化，并输出相关信息。
>
