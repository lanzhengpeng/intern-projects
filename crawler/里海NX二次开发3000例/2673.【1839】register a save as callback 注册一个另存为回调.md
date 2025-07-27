### 【1839】register a save as callback 注册一个另存为回调

#### 代码

```cpp
    /*HEAD REGISTER_A_SAVE_AS_CALLBACK CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_exit.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的提示，UF_print_syslog是在V18版本中新增的。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static void my_rename_part_callback(UF_callback_reason_e_t why,  
        const void *which_part, void *user_data)  
    {  
        tag_t  
            *part_ptr = (tag_t *)which_part;  
        char  
            part_fspec[MAX_FSPEC_SIZE+1];  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_PART_ask_part_name(*part_ptr, part_fspec));  
        WRITE_S(part_fspec);  
        UF_terminate();  
    }  
    static UF_registered_fn_p_t ftn_id = NULL;  
    static void do_it(void)  
    {  
        UF_CALL(UF_add_callback_function(UF_rename_part_reason,  
            my_rename_part_callback, NULL, &ftn_id));  
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

> 这段代码是NX二次开发代码，其主要功能是注册一个回调函数，用于在零件重命名时执行。具体来说：
>
> 1. 错误处理：代码定义了一个report_error函数，用于在函数调用失败时输出错误信息。它使用UF_get_fail_message获取错误信息，并使用UF_print_syslog和UF_UI_write_listing_window输出错误信息。
> 2. 字符串写入：代码定义了一个write_string_to_listing_window函数，用于将字符串写入列表窗口。
> 3. 回调函数：代码定义了一个my_rename_part_callback回调函数，它将在零件重命名时被调用。该函数获取重命名零件的tag，并使用UF_PART_ask_part_name获取零件名称，然后将其写入列表窗口。
> 4. 注册回调函数：在do_it函数中，使用UF_add_callback_function注册my_rename_part_callback回调函数，用于处理UF_rename_part_reason事件。
> 5. UFSTA宏：ufsta宏用于初始化NX环境、执行do_it函数，并在完成后终止NX环境。
>
> 总的来说，这段代码的主要目的是在零件重命名时，通过注册回调函数，获取并输出重命名零件的名称。
>
