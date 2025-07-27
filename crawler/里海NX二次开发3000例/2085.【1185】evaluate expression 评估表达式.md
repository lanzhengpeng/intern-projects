### 【1185】evaluate expression 评估表达式

#### 代码

```cpp
    /*HEAD EVALUATE_EXPRESSION CCC UFUN */  
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
    static void do_it(void)  
    {  
        char  
            exp_name[UF_MAX_EXP_LENGTH+1] = "",  
            msg[UF_MAX_EXP_LENGTH+1];  
        double  
            exp_val;  
        while (prompt_for_text("Enter expression name", exp_name))  
        {  
            if (!UF_CALL(UF_MODL_eval_exp(exp_name, &exp_val)))  
            {  
                sprintf(msg, "%s = %f\n", exp_name, exp_val);  
                ECHO(msg);  
            }  
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

> 这段代码是一个NX Open二次开发的应用程序，其主要功能是让用户输入表达式名称，然后计算并显示表达式的值。以下是代码的主要内容和功能介绍：
>
> 1. 包含了必要的NX Open头文件，如uf.h, uf_ui.h, uf_modl.h。
> 2. 定义了宏ECHO和UF_CALL，用于打印日志信息和调用NX Open函数，并检查函数返回值。
> 3. 定义了report_error函数，用于打印错误信息。
> 4. 定义了prompt_for_text函数，用于提示用户输入文本。
> 5. 定义了do_it函数，包含一个循环，提示用户输入表达式名称，调用UF_MODL_eval_exp计算表达式的值，并打印结果。
> 6. 定义了ufusr函数，是NX Open应用程序的入口函数。初始化NX Open环境，调用do_it函数执行功能，然后终止NX Open环境。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载应用程序的标志。
> 8. 代码整体采用C语言编写，符合NX Open应用程序的开发规范。主要功能是利用NX Open提供的表达式求值接口，实现用户交互式输入表达式并计算结果的功能。
>
