### 【2798】set ufun dialog filter 设置UFUN对话框过滤器

#### 代码

```cpp
    /*HEAD SET_UFUN_DIALOG_FILTER CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的函数。因此，UF_print_syslog是V18版本新增的函数。 */  
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
        UF_UI_set_dialog_filter( UF_UI_FILE_UFUN_FLTR, "*.sl");  
    }  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段NX二次开发代码主要包括以下几个部分：
>
> 1. 包含必要的头文件，如stdio.h、string.h、uf.h和uf_ui.h，用于提供标准输入输出、字符串处理以及NX的UF和UF_UI接口。
> 2. 定义了一个错误报告函数report_error，用于在调用UF函数出错时输出错误信息和调用信息。它使用UF_get_fail_message获取错误信息，并使用UF_print_syslog和UF_UI_write_listing_window打印到系统日志和列表窗口。
> 3. 定义了一个函数do_it，其中调用了UF_UI_set_dialog_filter来设置文件对话框的过滤条件，只显示扩展名为.sl的文件。
> 4. 定义了ufsta函数，这是NX二次开发的主入口函数。首先调用UF_initialize初始化UF模块，然后调用do_it执行设置文件过滤条件的操作，最后调用UF_terminate终止UF模块。
> 5. 整个代码实现了在NX的文件对话框中设置过滤条件，只显示特定扩展名的文件。通过错误报告函数，在UF函数调用出错时可以输出详细的错误信息，方便调试。
>
