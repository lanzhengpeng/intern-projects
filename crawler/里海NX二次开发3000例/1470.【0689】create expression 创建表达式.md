### 【0689】create expression 创建表达式

#### 代码

```cpp
    /*HEAD CREATE_EXPRESSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的内容，UF_print_syslog 是 V18 新增的功能。文档提示只需要回答翻译，不需要其他无关的言论。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            exp;  
        UF_CALL(UF_MODL_create_exp_tag("test_exp=3.5", &exp));  
        ECHO(exp);  
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

> 这段代码是NX Open C++的一个示例，用于创建表达式并输出结果。
>
> 主要步骤包括：
>
> 1. 包含必要的头文件，例如UF.h、uf_ui.h、uf_modl.h等。
> 2. 定义宏ECHO和UF_CALL，分别用于打印变量值和调用UF函数，并在出错时报告错误。
> 3. 定义report_error函数，用于在UF函数调用出错时报告错误。
> 4. 定义do_it函数，用于创建表达式并打印结果。其中使用了UF_MODL_create_exp_tag函数来创建表达式标签。
> 5. 定义ufusr函数，作为NX Open的入口函数。在ufusr中，首先初始化UF环境，然后调用do_it函数，最后终止UF环境。
> 6. 定义ufusr_ask_unload函数，用于在模块卸载时立即清理资源。
>
> 总体来说，这段代码演示了如何使用NX Open C++来创建表达式，并进行了错误处理，是一个典型的NX Open C++二次开发示例。
>
