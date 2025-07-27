### 【2811】set view layer visibility mask for work view pre v19 设置V19之前的工作视图层可见性掩码

#### 代码

```cpp
    /*HEAD SET_VIEW_LAYER_VISIBILITY_MASK_FOR_WORK_VIEW_PRE_V19 CCC UFUN */  
    /* This sample program is valid only through V18 里海译:这个示例程序仅适用于V18版本。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以总结如下：

1. UF_print_syslog是V18版本新增的函数。

2. 该函数的功能是打印系统日志信息。

3. 调用该函数时，需要传递一个参数，参数类型为const char*。

4. 该函数的声明位于user_functions.h头文件中。

5. 调用示例：UF_print_syslog("This is a test log message");

6. 该函数将传入的字符串作为一条日志信息打印到系统日志中。

7. 这个函数是为了方便在代码中打印日志信息，而不需要直接调用系统日志函数。

8. V18版本之前的代码如果需要打印日志，需要调用系统日志函数如syslog等。

9. 这个新增函数简化了在代码中打印日志信息的流程。

10. 在使用这个函数时，需要包含user_functions.h头文件。

以上是对文档内容的概括，希望对您有所帮助。 */  
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
        int  
            ip3[256];  
        UF_CALL(uc6446("", 2, ip3));  
        ip3[0]=0;  
        ip3[5]=1;  
        UF_CALL(uc6447("", 2, ip3));  
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

> 这是NX的二次开发代码，主要实现了以下功能：
>
> 1. 包含了必要的头文件，定义了UF_call宏用于调用NX API，并在出错时打印错误信息。
> 2. 实现了report_error函数，用于在API调用出错时打印错误信息，包括文件名、行号、出错API、错误码、错误描述等。同时，该函数会调用UF_print_syslog打印错误信息到系统日志。
> 3. 定义了do_it函数，用于设置视图层的可见性掩码。具体通过调用uc6446和uc6447两个未公开的API实现。
> 4. 实现了ufusr函数，用于在NX中加载和运行二次开发代码。该函数首先调用UF_initialize初始化二次开发环境，然后调用do_it执行主要功能，最后调用UF_terminate终止二次开发环境。
> 5. 实现了ufusr_ask_unload函数，用于设置二次开发代码卸载方式为立即卸载。
> 6. 整体来看，这段代码实现了在NX中设置视图层的可见性掩码的功能，并通过UF_print_syslog将错误信息打印到系统日志，方便调试。同时，遵循了NX二次开发的规范，通过ufusr函数加载和运行代码。
>
