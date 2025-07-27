### 【1984】report all ug default user defined symbols 报告所有UG默认用户定义符号

#### 代码

```cpp
    /*HEAD REPORT_ALL_UG_DEFAULT_USER_DEFINED_SYMBOLS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        ECHO(msg);  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %f\n", title, n);  
        ECHO(msg);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_symbols;  
        char  
            **symbol_names;  
        UF_DRF_ud_symbol_font_info_p_t  
            symbol_info;  
    /*  This is for User Defined Symbols NOT Custom Symbols 里海译:这是为用户定义符号而不是自定义符号。 */  
        if (UF_CALL(UF_DRF_ask_symbol_data_from_name("ug_default", &symbol_names,  
            &n_symbols, &symbol_info))) return;  
        WRITE_D(n_symbols);  
        for (ii = 0; ii < n_symbols; ii++)  
        {  
            WRITE_S(symbol_names[ii]);  
            WRITE_F(symbol_info[ii].length);  
            WRITE_F(symbol_info[ii].height);  
        }  
        UF_free_string_array(n_symbols, symbol_names);  
        UF_free(symbol_info);  
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

> 这段代码是用于NX CAD软件的二次开发，主要功能是获取并打印出UG默认的用户定义符号的相关信息。
>
> 代码主要包括以下几个部分：
>
> 1. 头文件包含：包含了必要的NX Open API头文件，如uf.h、uf_ui.h、uf_drf.h等，用于调用NX的函数。
> 2. 错误处理宏：定义了ECHO、UF_CALL、WRITE_D等宏，用于打印错误信息、调用NX函数以及输出变量值。
> 3. 获取用户定义符号信息：调用UF_DRF_ask_symbol_data_from_name函数，传入"ug_default"作为符号集名称，获取默认的用户定义符号信息，包括符号名称、长度和高度。
> 4. 打印符号信息：遍历获取到的符号信息数组，使用WRITE_S和WRITE_F宏打印出每个符号的名称、长度和高度。
> 5. 内存释放：释放获取的符号名称数组和符号信息结构体数组占用的内存。
> 6. 主函数：定义了ufusr函数作为主入口，在NX中注册并运行。
> 7. 卸载处理：定义了ufusr_ask_unload函数，用于在插件卸载时立即终止。
>
> 总体来说，这段代码通过调用NX的API，实现了获取并打印出默认用户定义符号信息的功能，展现了NX二次开发的基本流程。
>
