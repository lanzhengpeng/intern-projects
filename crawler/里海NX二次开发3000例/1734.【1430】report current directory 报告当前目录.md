### 【1430】report current directory 报告当前目录

#### 代码

```cpp
    /*HEAD REPORT_CURRENT_DIRECTORY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。因此，我的回答是：

UF_print_syslog是V18版本新增的功能。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        char  
            current_directory[MAX_FSPEC_SIZE+1];  
        UF_CALL(uc4565(1, current_directory));  
        WRITE_S(current_directory);  
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

> 这段代码是一个NX Open C++的UF二次开发示例，主要功能是获取当前目录并在列表窗口中显示。
>
> 具体代码的介绍如下：
>
> 1. 包含了必要的NX Open C++头文件。
> 2. 定义了宏UF_CALL，用于调用NX Open C++函数，并在出错时报告错误。
> 3. 定义了report_error函数，用于输出错误信息到系统日志和列表窗口。
> 4. 定义了宏WRITE_S，用于在列表窗口中输出字符串。
> 5. 定义了write_string_to_listing_window函数，用于在列表窗口中输出字符串。
> 6. 定义了do_it函数，用于获取当前目录并输出到列表窗口。
> 7. 定义了ufusr函数，这是NX二次开发程序的入口函数。在该函数中初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
> 9. 代码中使用了NX Open C++的UF函数，如UF_initialize、uc4565、UF_terminate等，来初始化、获取当前目录、终止环境等。
>
> 总体而言，这段代码实现了一个简单的NX二次开发程序，可以获取当前目录并在列表窗口中显示。它采用了NX Open C++的UF函数来实现所需功能，并包含了错误处理和日志输出。
>
