### 【2105】report failure message 报错信息

#### 代码

```cpp
    /*HEAD REPORT_FAILURE_MESSAGE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
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
            error = 0;  
        char  
             message[133];  
        while (prompt_for_an_integer("Get failure message", "Error", &error))  
        {  
            UF_get_fail_message(error, message);  
            WRITE_D(error);  
            WRITE_S(message);  
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

> 这段代码是NX Open C++的二次开发示例代码，其主要功能是获取NX的错误码对应的错误消息，并通过UI窗口显示出来。
>
> 主要内容包括：
>
> 1. 定义了错误报告宏，用于在调用NX API时捕获错误，并打印错误消息。
> 2. 定义了整数和字符串输出宏，用于向UI窗口输出变量名和值。
> 3. 实现了一个获取用户输入整数的方法，用于获取用户输入的错误码。
> 4. 实现了主体逻辑函数do_it，循环获取用户输入的错误码，并获取对应的错误消息，输出到UI窗口。
> 5. 定义了ufusr函数，用于初始化NX，调用主体逻辑，然后终止NX。
> 6. 定义了卸载宏，用于卸载NX Open C++库。
>
> 通过这段代码，用户可以方便地查询不同错误码对应的错误消息，帮助用户了解NX API调用中的错误信息。
>
