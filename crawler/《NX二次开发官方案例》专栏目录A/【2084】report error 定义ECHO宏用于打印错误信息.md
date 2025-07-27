### 【2084】report error 定义ECHO宏用于打印错误信息

#### 代码

```cpp
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

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，其中定义了两个宏：
>
> 1. ECHO(X): 该宏用于打开NX的日志窗口，并在窗口中写入参数X的内容，同时也会将X的内容打印到NX的syslog中。
> 2. UF_CALL(X): 该宏用于调用NX的UF函数，并在调用失败时输出错误信息，包括错误码、错误信息、调用失败的函数名以及文件名和行号。
>
> 这两个宏的作用是简化错误处理流程，当UF函数调用失败时，能够自动输出详细的错误信息，便于调试和定位问题。
>
> 除此之外，代码还包含了一个函数report_error()，用于实现UF_CALL宏的错误处理逻辑。该函数接收错误码、文件名、行号和函数名，在错误码非零的情况下，会构造错误信息并输出到日志窗口和syslog。
>
> 总的来说，这段代码通过宏和函数封装了NX API的错误处理流程，提高了二次开发的效率。
>
