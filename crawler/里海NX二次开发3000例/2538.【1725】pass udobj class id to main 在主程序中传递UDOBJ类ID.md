### 【1725】pass udobj class id to main 在主程序中传递UDOBJ类ID

#### 代码

```cpp
    /*HEAD PASS_UDOBJ_CLASS_ID_TO_MAIN CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_udobj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    DllExport extern UF_UDOBJ_class_t get_my_class_id(void);  
    static void do_it(void)  
    {  
        printf("In main\n");  
        printf("The class_id is %d.\n", get_my_class_id());  
    }  
    void main( int argc, char *argv[] )  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_CALL(UF_terminate());  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发程序的示例，主要功能是获取当前NX对象的类ID并打印出来。
>
> 代码中的关键部分包括：
>
> 1. 包含了必要的NX Open头文件，用于调用NX Open的API函数。
> 2. 定义了一个错误报告函数report_error，用于在调用NX Open API函数出错时打印错误信息。
> 3. 定义了一个静态函数do_it，用于打印当前NX对象的类ID。它调用了外部函数get_my_class_id来获取类ID。
> 4. 在main函数中，首先调用UF_initialize()初始化NX Open环境，然后调用do_it()执行打印类ID的操作，最后调用UF_terminate()终止NX Open环境。
> 5. get_my_class_id()函数是一个外部函数，用于获取当前NX对象的类ID，需要用户自行实现。
> 6. 使用了宏UF_CALL来包装NX Open API调用，以简化错误处理。
> 7. 使用了NX Open V18新增的UF_print_syslog函数来打印系统日志，以提供更丰富的错误信息。
>
> 总体来说，这段代码提供了获取当前NX对象类ID的一个基本框架，用户只需实现get_my_class_id()函数即可实现所需功能。
>
