### 【2441】report whether specified directory is readable 报告指定目录是否可读

#### 代码

```cpp
    /*HEAD REPORT_WHETHER_SPECIFIED_DIRECTORY_IS_READABLE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的功能，用于打印系统日志信息。 */  
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
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static logical is_directory_readable(char *dspec)  
    {  
        if (uc4508(dspec, (1 << 13), 0, NULL) >= 0)  
        {  
            uc4548();  
            return TRUE;  
        }  
        else  
            return FALSE;  
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
            dir_spec[UF_CFI_MAX_PATH_NAME_SIZE] = { "" };  
        while (prompt_for_text("Enter directory path", dir_spec))  
        {  
            WRITE_S(dir_spec);  
            WRITE_L(is_directory_readable(dir_spec));  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了报告错误函数report_error，用于在函数调用失败时输出错误信息到系统日志和列表窗口。
> 2. 定义了写入逻辑值到列表窗口的函数write_logical_to_listing_window。
> 3. 定义了写入字符串到列表窗口的函数write_string_to_listing_window。
> 4. 定义了检查目录是否可读的函数is_directory_readable。
> 5. 定义了提示用户输入文本的函数prompt_for_text。
> 6. 定义了主函数do_it，循环提示用户输入目录路径，然后检查目录是否可读，并将路径和结果写入列表窗口。
> 7. 定义了ufusr函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. 定义了卸载函数ufusr_ask_unload，用于立即卸载NX环境。
>
> 通过这些函数，代码实现了在NX环境下提示用户输入目录路径，检查目录是否可读，并将结果输出到列表窗口的功能。
>
