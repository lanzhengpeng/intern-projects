### 【3110】find expression tag from name 根据表达式名称查找表达式标签

#### 代码

```cpp
    /*HEAD FIND_EXPRESSION_TAG_FROM_NAME CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        ECHO(msg);  
    }  
    static tag_t ask_tag_of_expression_name(char *exp_name)  
    {  
        tag_t  
            exp_tag;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1] = { "" },  
            *lhs,  
            *rhs;  
    /*  In NX 4 and 5 the lookup of the expression name is case sensitive.  
        In NX 6 it is not.  
    */  
        if (!UF_CALL(UF_MODL_ask_exp(exp_name, exp_str)) &&  
            !UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs, &rhs, &exp_tag)))  
        {  
            WRITE_S(exp_str);  
            WRITE_S(lhs);  
            WRITE_S(rhs);  
            UF_free(lhs);  
            UF_free(rhs);  
            return (exp_tag);  
        }  
        else  
            return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    static void do_it(void)  
    {  
        char  
            exp_name[UF_MAX_EXP_LENGTH+1] = { "" };  
        while (prompt_for_text("Enter expression name", exp_name))  
        {  
            WRITE_S(exp_name);  
            WRITE_D(ask_tag_of_expression_name(exp_name));  
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

> 这是段NX（Siemens NX）的二次开发代码，其主要功能是从用户输入的表达式名称中获取表达式标签。以下是代码的主要组成部分：
>
> 1. 头文件包含：代码包含了必要的NX头文件，如uf.h、uf_ui.h、uf_modl.h，用于调用NX的函数。
> 2. 错误报告函数：report_error函数用于在发生错误时报告错误信息，包括文件名、行号、错误码和错误消息。
> 3. 提示输入文本函数：prompt_for_text函数用于提示用户输入文本，并返回输入结果。
> 4. 写入字符串和整数函数：write_string_to_listing_window和write_integer_to_listing_window函数用于将字符串和整数输出到列表窗口。
> 5. 获取表达式标签函数：ask_tag_of_expression_name函数根据表达式名称查询并返回表达式标签。
> 6. 主函数：do_it函数用于循环提示用户输入表达式名称，并获取对应的标签并输出。
> 7. NX初始化和终止：ufusr函数用于初始化NX环境，调用主函数，并终止NX环境。
> 8. 卸载请求：ufusr_ask_unload函数用于立即卸载用户自定义函数。
>
> 整体而言，该代码实现了根据用户输入的表达式名称获取表达式标签的功能，并在NX环境中进行调用。
>
