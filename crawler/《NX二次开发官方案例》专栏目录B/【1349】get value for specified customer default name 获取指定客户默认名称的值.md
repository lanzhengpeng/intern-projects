### 【1349】get value for specified customer default name 获取指定客户默认名称的值

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        char  
            theName[MAX_LINE_SIZE+1] = { "" },  
            *theValue;  
        while (prompt_for_text("Get Customer Default", theName))  
        {  
            if ((resp = UF_get_customer_default(theName, 0, &theValue)) == 0)  
            {  
                ECHO("%s = %s\n", theName, theValue);  
                UF_free(theValue);  
            }  
            else if (resp == 1)  
                ECHO("Default Name %s not found");  
            else  
                UF_CALL(resp);  
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

> 根据代码内容，这是一段基于NX Open C++ API的NX二次开发代码，主要用于获取NX中的客户默认值。以下是该代码的主要功能和流程：
>
> 1. 定义了ECHO函数，用于在NX的日志窗口输出信息，方便调试。
> 2. 定义了UF_CALL宏，用于调用NX的函数，并报告错误。
> 3. 定义了prompt_for_text函数，用于在NX的对话框中提示用户输入文本，并获取输入值。
> 4. 定义了do_it函数，用于循环提示用户输入默认名称，并调用UF_get_customer_default函数获取该名称对应的默认值。获取成功则输出默认值，未找到则输出提示信息，调用出错则报告错误。
> 5. 定义了ufusr函数，这是NX二次开发的主要入口函数。在该函数中，首先调用UF_initialize函数初始化NX Open C++ API，然后调用do_it函数执行获取默认值的功能，最后调用UF_terminate函数结束API使用。
> 6. 定义了ufusr_ask_unload函数，用于卸载二次开发应用程序。
>
> 总的来说，该代码通过NX Open C++ API实现了与NX交互，获取用户输入的客户默认值名称，并查询对应的默认值，实现了获取NX客户默认值的功能。代码结构清晰，功能明确，是一个典型的NX二次开发示例。
>
