### 【1339】get fail message 获取失败消息

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是在V18版本中新增的。 */  
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

> 这段代码是一个NX Open C++程序，用于获取NX API调用失败的错误消息。主要功能包括：
>
> 1. 定义了一个report_error函数，用于在API调用失败时，打印文件名、行号、错误码和错误消息。
> 2. 主函数中，首先调用UF_initialize初始化NX环境，然后遍历命令行参数，将每个参数转换为错误码，并调用UF_get_fail_message获取错误消息，最后打印错误码和错误消息。
> 3. 使用了宏UF_CALL封装API调用，以便在API调用失败时，自动调用report_error函数打印错误信息。
> 4. 最后，调用UF_terminate终止NX环境。
>
> 这段代码通过封装错误处理，使得API调用更加健壮，方便开发者调试。同时，也展示了NX Open C++编程的一些常用技巧。
>
