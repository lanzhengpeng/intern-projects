### 【1439】report current work layer 获取当前工作图层

#### 代码

```cpp
    /*HEAD REPORT_CURRENT_WORK_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能。 */  
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
            work_layer;  
        if (!UF_CALL(UF_LAYER_ask_work_layer(&work_layer)))  
        {  
            WRITE_D(work_layer);  
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

> 这是段NX二次开发代码，主要功能是获取当前的工作层并输出。代码的主要步骤包括：
>
> 1. 引入NX提供的头文件，如uf.h、uf_ui.h等。
> 2. 定义宏UF_CALL，用于执行NX函数，并在出错时输出错误信息。
> 3. 定义report_error函数，用于输出错误信息到日志窗口。
> 4. 定义宏WRITE_D，用于将整数输出到日志窗口。
> 5. 定义write_integer_to_listing_window函数，用于将整数输出到日志窗口。
> 6. 定义do_it函数，用于获取当前工作层并输出。
> 7. 定义ufusr函数，用于初始化NX环境，执行do_it函数，然后终止NX环境。
> 8. 定义ufusr_ask_unload函数，用于卸载NX二次开发环境。
>
> 整个代码逻辑清晰，通过UF_LAYER_ask_work_layer函数获取当前工作层，然后输出到日志窗口。在出错时，通过宏和函数捕获错误，并输出相关信息，方便调试。代码结构合理，注释清晰，具有良好的可读性。
>
