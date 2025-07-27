### 【1723】pass an integer to grip 传递一个整数给GRIP

#### 代码

```cpp
    /*HEAD PASS_AN_INTEGER_TO_GRIP CCC UFUN */  
    /*  Here is the corresponding GRIP which modifies the integer.  
            NUMBER/n  
            UFARGS/n  
            n = 2  
            HALT  
    */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是在 V18 中新增的，请只回答译文，不要添加其他内容。 */  
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
        int  
            n = 1;  
        double  
            dn;  
        UF_args_t  
            arg;  
        WRITE_D(n);  
        dn = (double)n;  
        arg.type = UF_TYPE_DOUBLE;  
        arg.length = 0;  
        arg.address = &dn;  
    #ifdef _WIN32  
        UF_CALL(UF_call_grip("n:\\grip\\modify_number.grx", 1, &arg));  
    #else  
        UF_CALL(UF_call_grip("grip/modify_number.grx", 1, &arg));  
    #endif  
        n = (int)dn;  
        WRITE_D(n);  
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

> 这段代码是NX Open C++的一个示例，用于演示如何通过UF调用GRIP脚本来修改整数。
>
> 代码的主要步骤如下：
>
> 1. 定义了错误报告函数report_error，用于在UF函数调用失败时打印错误信息。
> 2. 定义了write_integer_to_listing_window函数，用于将整数值写入列表窗口。
> 3. 定义了do_it函数，用于进行UF调用GRIP脚本的流程：初始化整数值n为1将n转换为double类型，并作为UF_args_t结构体参数传递给UF_call_grip函数调用UF_call_grip函数，传入GRIP脚本路径和参数将double类型的值转换回int，并更新n的值将更新后的n值写入列表窗口
> 4. 初始化整数值n为1
> 5. 将n转换为double类型，并作为UF_args_t结构体参数传递给UF_call_grip函数
> 6. 调用UF_call_grip函数，传入GRIP脚本路径和参数
> 7. 将double类型的值转换回int，并更新n的值
> 8. 将更新后的n值写入列表窗口
> 9. 定义了ufusr函数，作为UF的入口函数。该函数会初始化UF，调用do_it函数，然后终止UF。
> 10. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总的来说，这段代码演示了如何使用NX Open C++调用GRIP脚本来进行参数传递和修改，并包含了错误处理和日志输出。
>
