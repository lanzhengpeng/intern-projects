### 【1954】report all routing env charx 报告所有路由环境变量

#### 代码

```cpp
    /*HEAD REPORT_ALL_ROUTING_ENV_CHARX CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_route.h>  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        ECHO(msg);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %f\n", title, n);  
        ECHO(msg);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_charx;  
        UF_ROUTE_charx_t  
            *charx;  
        UF_CALL(UF_ROUTE_ask_charx_env(&n_charx, &charx));  
        WRITE_D(n_charx);  
        for (ii = 0; ii < n_charx; ii++)  
        {  
            WRITE_S(charx[ii].title);  
            switch (charx[ii].type)  
            {  
                case UF_EPLIB_CHARX_TYPE_INT:  
                    WRITE_D(charx[ii].value.i_value);  
                    break;  
                case UF_EPLIB_CHARX_TYPE_REAL:  
                    WRITE_F(charx[ii].value.r_value);  
                    break;  
                case UF_EPLIB_CHARX_TYPE_STR:  
                case UF_EPLIB_CHARX_TYPE_INT_EXPR:  
                case UF_EPLIB_CHARX_TYPE_REAL_EXPR:  
                case UF_EPLIB_CHARX_TYPE_STR_EXPR:  
                    WRITE_S(charx[ii].value.s_value);  
                    break;  
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

> 这段代码是NX的二次开发代码，主要功能是报告所有路由环境的特征。其主要流程如下：
>
> 1. 包含了必要的头文件，如stdio.h、string.h、uf.h、uf_ui.h、uf_route.h等。
> 2. 定义了宏ECHO，用于将信息输出到日志窗口和syslog。
> 3. 定义了宏UF_CALL，用于执行NX的API函数，并捕获错误。
> 4. 定义了函数report_error，用于在API调用出错时输出错误信息。
> 5. 定义了宏WRITE_S、WRITE_D、WRITE_F，用于输出字符串、整数、浮点数到日志窗口。
> 6. 定义了函数do_it，其中：
>
> 1. 定义了ufusr函数，用于初始化NX环境、执行do_it函数，然后终止NX环境。
> 2. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总的来说，这段代码通过调用NX的API获取环境特征，并将特征信息输出到日志窗口，用于调试和记录环境信息。
>
