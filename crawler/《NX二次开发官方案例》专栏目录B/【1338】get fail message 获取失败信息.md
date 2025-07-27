### 【1338】get fail message 获取失败信息

#### 代码

```cpp
    /*HEAD GET_FAIL_MESSAGE CCC UFUN */  
    #include <stdlib.h>  
    #include <stdio.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的功能，它用于打印系统日志。 */  
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
    void main( int argc, char *argv[] )  
    {  
        int  
            error_code,  
            i;  
        char  
            error_message[133];  
        if (!UF_CALL(UF_initialize()))  
        {  
            for (i=1; i<argc; i++)  
            {  
                error_code = atoi(argv[i]);  
                (UF_get_fail_message(error_code, error_message)) ?  
                    printf("Error %d: not found\n", error_code) :  
                    printf("Error %d: %s\n", error_code, error_message);  
            }  
            UF_CALL(UF_terminate());  
        }  
    }

```

#### 代码解析

> 这段NX Open C++代码主要用于获取NX的错误码对应的错误信息，并在控制台和系统日志中打印输出。
>
> 代码的主要功能如下：
>
> 1. 定义了一个宏UF_CALL，用于调用NX函数，并在调用失败时报告错误。
> 2. 实现了一个报告错误的函数report_error，用于获取错误码对应的错误信息，并在控制台和系统日志中打印输出。
> 3. 主函数中，首先初始化NX环境，然后遍历命令行传入的错误码，调用UF_get_fail_message获取错误信息，并在控制台打印输出。
> 4. 最后，终止NX环境。
>
> 这段代码的主要作用是用于NX开发过程中，快速查询错误码对应的错误信息，以便于定位问题。通过UF_get_fail_message函数，可以获取错误码对应的错误描述字符串，从而更直观地理解错误码的含义。
>
