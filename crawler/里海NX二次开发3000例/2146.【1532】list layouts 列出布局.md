### 【1532】list layouts 列出布局

#### 代码

```cpp
    /*HEAD LIST_LAYOUTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layout.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，我翻译如下：

注意：UF_print_syslog是V18版本新增的。 */  
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
        char  
            layout[MAX_ENTITY_NAME_SIZE + 1] = { "" };  
        while (!uc6471(layout) && strcmp(layout, ""))  
            printf("found layout %s\n", layout);  
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

> 根据代码内容，这是NX的二次开发代码，主要实现了以下功能：
>
> 1. 引入了UF、UF_UI、UF_LAYOUT等NX二次开发常用的头文件。
> 2. 定义了宏UF_CALL，用于调用NX函数，并在函数调用失败时输出错误信息。
> 3. 定义了report_error函数，用于输出错误信息，包括错误码、错误描述和失败的函数调用。
> 4. 定义了do_it函数，用于遍历当前部件的所有布局(Layout)，并打印出布局名称。
> 5. 定义了ufusr函数，作为NX的入口函数。在该函数中，首先调用UF_initialize进行初始化，然后调用do_it函数遍历布局，最后调用UF_terminate进行清理。
> 6. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示卸载该UF后立即释放资源。
> 7. 整体来看，这段代码主要实现了遍历当前NX部件的所有布局，并打印布局名称的功能。通过NX的二次开发接口UF_LAYOUT来实现。
>
