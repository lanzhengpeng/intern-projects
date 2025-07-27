### 【3100】demo invoke pdm server 调用PDM服务器程序使用NX输入代码和字符串

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            in_code = 0,  
            out_code = 0;  
        char  
            in_string[50],  
            *out_string = NULL;  
        while (prompt_for_an_integer("Invoke PDM server", "in_code", &in_code))  
        {  
            WRITE_D(in_code);  
            sprintf(in_string, "Input string passed from NX for code %d", in_code);  
            WRITE_S(in_string);  
            UF_CALL(UF_UGMGR_invoke_pdm_server(in_code, in_string,  
                &out_code, &out_string));  
            WRITE_D(out_code);  
            WRITE_S(out_string);  
            if (out_string != NULL) UF_free(out_string);  
        }  
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

> 这是段NX二次开发代码，主要功能是调用NX的PDM(产品数据管理)服务器。下面是对代码的详细解释：
>
> 1. 代码结构：代码包括头文件引用、宏定义、全局变量声明、函数声明和主体函数。
> 2. 头文件引用：包含了必要的NX API的头文件，如uf.h、uf_ui.h等。
> 3. 宏定义：定义了UF_CALL和WRITE_系列宏，用于调用NX API和输出调试信息。
> 4. 全局变量声明：声明了一些全局变量，如输入输出代码、字符串等。
> 5. 函数声明：声明了错误报告函数、字符串输出函数和提示输入函数。
> 6. 主体函数：ufusr：NX二次开发的入口函数，初始化NX环境，调用do_it函数，然后终止NX环境。do_it：循环提示用户输入一个整数，调用NX的UF_UGMGR_invoke_pdm_server函数，将输入整数和字符串传递给PDM服务器，并获取返回的输出代码和字符串，最后输出这些信息。ufusr_ask_unload：当NX关闭时卸载二次开发代码。
> 7. ufusr：NX二次开发的入口函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. do_it：循环提示用户输入一个整数，调用NX的UF_UGMGR_invoke_pdm_server函数，将输入整数和字符串传递给PDM服务器，并获取返回的输出代码和字符串，最后输出这些信息。
> 9. ufusr_ask_unload：当NX关闭时卸载二次开发代码。
> 10. 代码逻辑：通过循环提示用户输入，调用PDM服务器，并输出结果，实现了与PDM服务器的交互。
> 11. 注意事项：需要注意释放返回的字符串内存，避免内存泄漏。
>
> 以上是对代码的详细解释，如果有任何其他问题，请随时提出。
>
