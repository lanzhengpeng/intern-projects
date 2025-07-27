### 【2530】say when parts are modified 当部件被修改时，会报告“部件已被修改”

#### 代码

```cpp
    /*HEAD SAY_WHEN_PARTS_ARE_MODIFIED CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，翻译内容如下：

注意：UF_print_syslog是在V18版本中新增的。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_part_name(tag_t part)  
    {  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_PART_ask_part_name(part, fspec);  
        WRITE_S(fspec);  
    }  
    /* qq3123197280 */  
    static void say_part_is_modified(UF_callback_reason_e_t reason,  
        const void *part_tag_p, void *user_data)  
    {  
        tag_p_t  
            part = (tag_p_t)part_tag_p;  
        if (UF_CALL(UF_initialize())) return;  
        WRITE("Modified part ");  
        report_part_name(*part);  
        UF_terminate();  
    }  
    static void do_it(void)  
    {  
        UF_registered_fn_p_t  
            func_id;  
        UF_CALL(UF_add_callback_function(UF_modified_part_reason,  
            say_part_is_modified, NULL, &func_id));  
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

> 这段NX Open C++代码用于在NX部件被修改时打印出被修改部件的名称。
>
> 主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了字符串打印函数write_string_to_listing_window，用于将字符串输出到列表窗口。
> 3. 定义了报告部件名称的函数report_part_name，用于打印出指定部件的名称。
> 4. 定义了回调函数say_part_is_modified，当部件被修改时会调用这个函数，并打印出被修改的部件名称。
> 5. 主函数do_it中，注册了回调函数say_part_is_modified，用于处理部件被修改的事件。
> 6. ufsta函数是NX Open C++的入口函数，在初始化NX后调用do_it，完成回调函数的注册，最后终止NX。
>
> 通过这个程序，可以监控NX部件的修改情况，并在部件被修改时打印出其名称。这可以用于跟踪部件的变更情况。
>
