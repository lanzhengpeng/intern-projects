### 【1845】register file open callback 注册文件打开回调

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
    /* qq3123197280 */  
    static void my_open_part_callback(UF_callback_reason_e_t why,  
        const void *which_part, void *user_data)  
    {  
        tag_t  
            *objects,  
            *part_ptr = (tag_t *)which_part;  
        char  
            part_fspec[MAX_FSPEC_SIZE+1];  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_PART_ask_part_name(*part_ptr, part_fspec));  
        ECHO("Running File Open callback...\n\n" );  
        ECHO("Note that this runs BEFORE the part has finished opening.\n\n" );  
        uc1601("Pause:", 1);  
        UF_terminate();  
    }  
    static UF_registered_fn_p_t ftn_id = NULL;  
    static void do_it(void)  
    {  
        UF_CALL(UF_add_callback_function(UF_open_part_reason,  
            my_open_part_callback, NULL, &ftn_id));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_SEL_DIALOG);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要包括以下几个部分：
>
> 1. 头文件包含：引入了NX Open的UF头文件，用于调用NX的API函数。
> 2. 宏定义：定义了ECHO宏，用于输出信息到列表窗口和系统日志，以及UF_CALL宏，用于调用NX API函数并报告错误。
> 3. 错误报告函数：定义了report_error函数，用于在调用NX API出错时报告错误信息。
> 4. 文件打开回调函数：定义了my_open_part_callback函数，作为UF_open_part_reason的回调函数，在打开部件文件前会被调用。
> 5. 注册回调函数：在do_it函数中，使用UF_add_callback_function函数注册了my_open_part_callback回调函数。
> 6. UFSTA和UFUSR函数：定义了ufsta和ufusr_ask_unload函数，分别作为NX Open的启动函数和询问卸载函数。
> 7. NX Open初始化和终止：在ufsta函数中使用UF_initialize和UF_terminate初始化和终止NX Open。
> 8. 回调函数中打印提示信息：在回调函数my_open_part_callback中打印提示信息，并暂停。
>
> 总的来说，这段代码实现了在打开NX部件文件前注册一个回调函数，并在回调函数中打印提示信息的功能。
>
