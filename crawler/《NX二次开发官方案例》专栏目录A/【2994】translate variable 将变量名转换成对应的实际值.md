### 【2994】translate variable 将变量名转换成对应的实际值

#### 代码

```cpp
    /*HEAD TRANSLATE_VARIABLE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本中新增的函数，用于打印系统日志。 */  
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
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
        char  
            mine[64] = {""},  
            *junk,  
            *uga_dir;  
        while(prompt_for_text("Enter variable to translate", mine))  
        {  
            if (!UF_CALL(UF_translate_variable("UGALLIANCE_DIR", &uga_dir)) &&  
                (uga_dir != NULL)) WRITE_S(uga_dir);  
            if (!UF_CALL(UF_translate_variable(mine, &junk)) &&  
                (junk != NULL)) WRITE_S(junk);  
        /*  Do NOT free or UF_free the returned strings - they point to the actual  
            values in the environment not allocated copies */  
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

> 根据代码注释，这是一段用于NX二次开发的UFUNC程序，主要功能是翻译环境变量。
>
> 代码主要包括以下几个部分：
>
> 1. 错误处理函数：report_error，用于在调用UFUNC函数失败时打印错误信息。
> 2. 写日志函数：write_string_to_listing_window，用于将字符串写入日志窗口。
> 3. 获取用户输入：prompt_for_text，用于提示用户输入变量名，并返回用户输入的字符串。
> 4. 翻译环境变量：do_it，循环提示用户输入变量名，然后调用UF_translate_variable函数翻译环境变量，并打印翻译结果。
> 5. UFUNC初始化：ufusr，用于初始化UFUNC环境，并调用do_it函数执行主体功能。
> 6. 卸载函数：ufusr_ask_unload，用于在卸载UFUNC时调用。
>
> 总体来说，该代码通过循环获取用户输入的变量名，并调用UFUNC函数翻译环境变量，最后将翻译结果打印到日志窗口。它主要用于测试和演示如何使用UFUNC函数翻译环境变量。
>
