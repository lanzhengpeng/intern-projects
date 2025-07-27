### 【0912】create smart scalar expression 创建智能标量表达式

#### 代码

```cpp
    /*HEAD CREATE_SMART_SCALAR_EXPRESSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog 是 V18 新增的功能。 */  
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
    static tag_t ask_tag_of_expression_name(char *exp_name)  
    {  
        tag_t  
            exp_tag;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1] = { "" },  
            *lhs,  
            *rhs;  
        UF_CALL(UF_MODL_ask_exp(exp_name, exp_str));  
        if (!UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs, &rhs, &exp_tag)))  
        {  
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
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            exp_tag,  
            scalar;  
        char  
            exp_name[UF_MAX_EXP_LENGTH+1] = { "" };  
        while (prompt_for_text("Enter expression name for smart scalar", exp_name))  
        {  
            if ((exp_tag = ask_tag_of_expression_name(exp_name)) != NULL_TAG)  
            {  
                UF_CALL(UF_SO_create_scalar_exp(exp_tag,  
                    UF_SO_update_after_modeling, exp_tag, &scalar));  
                WRITE_D(scalar);  
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

> 这段代码是NX二次开发中的用户函数(ufusr)，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在UF函数调用失败时打印错误信息。
> 2. 定义了一个提示输入文本的函数prompt_for_text，用于提示用户输入表达式名称。
> 3. 定义了一个解析表达式名称并获取表达式tag的函数ask_tag_of_expression_name。
> 4. 定义了一个将整数写入列表窗口的函数write_integer_to_listing_window。
> 5. 在do_it函数中，通过循环提示用户输入表达式名称，然后解析名称获取表达式tag，并创建智能标量表达式，将结果写入列表窗口。
> 6. ufusr函数是用户函数的入口点，首先初始化NX环境，然后调用do_it函数执行主体逻辑，最后终止NX环境。
> 7. ufusr_ask_unload函数用于返回卸载模式，表示用户函数可以立即卸载。
>
> 总的来说，这段代码实现了通过用户输入的表达式名称，创建对应的智能标量表达式并显示结果的功能。
>
