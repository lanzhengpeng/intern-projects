### 【1456】initialize and terminate sketch 初始化和终止草图

#### 代码

```cpp
    /*HEAD INITIALIZE_AND_TERMINATE_SKETCH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sket.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，UF_print_syslog是V18版本中新增的功能。 */  
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
        tag_t  
            sket_tag;  
        char  
            sket_name[MAX_ENTITY_NAME_SIZE+1] = { "SKETCH_000" };  
        if (prompt_for_text("Enter sketch name", sket_name) &&  
            !UF_CALL(UF_SKET_initialize_sketch(sket_name, &sket_tag)))  
                UF_CALL(UF_SKET_terminate_sketch());  
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

> 这段代码是NX Open C++的一个示例，用于初始化和终止NX草图。
>
> 主要功能包括：
>
> 1. 初始化NX环境：使用UF_initialize()函数初始化NX环境。
> 2. 提示输入草图名称：使用自定义的prompt_for_text()函数提示用户输入草图名称，并保存在sket_name数组中。
> 3. 初始化草图：调用UF_SKET_initialize_sketch()函数，根据输入的草图名称sket_name初始化草图，并获取草图tag。
> 4. 终止草图：如果初始化成功，调用UF_SKET_terminate_sketch()函数终止草图。
> 5. 错误处理：使用自定义的report_error()函数处理UF函数调用错误，打印错误信息。
> 6. 清理NX环境：在函数结束时调用UF_terminate()清理NX环境。
> 7. 提供卸载函数：提供了ufusr_ask_unload()函数，用于在卸载NX Open C++应用程序时调用。
>
> 整个代码通过UF函数实现了NX草图的初始化和终止，同时提供了错误处理和交互式输入，是一个完整的NX Open C++二次开发示例。
>
