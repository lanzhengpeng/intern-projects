### 【1550】list views in layout 列出当前布局中的视图

#### 代码

```cpp
    /*HEAD LIST_VIEWS_IN_LAYOUT CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译如下：

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
            view[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while (!uc6473("L4", view) && strcmp(view, ""))  
            printf("Found %s in current layout\n", view);  
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

> 这是段NX Open C++的二次开发代码，其主要功能是遍历当前布局中的所有视图，并打印视图的名称。以下是代码的主要功能介绍：
>
> 1. 包含了NX Open C++的头文件，定义了UF函数、UI函数和布局函数。
> 2. 定义了一个错误报告函数report_error，用于打印错误代码、错误消息和出错的函数调用。其中UF_print_syslog是NX 18版本新增的函数，用于将错误信息打印到NX日志文件。
> 3. 定义了一个函数do_it，用于遍历当前布局中的所有视图。该函数使用UF_LAYOUT函数uc6473来获取布局中的视图名称，直到获取失败为止。如果获取成功，则打印视图名称。
> 4. 定义了ufusr函数，这是NX二次开发程序的入口函数。在该函数中，首先调用UF_initialize进行初始化，然后调用do_it函数遍历并打印视图名称，最后调用UF_terminate进行清理。
> 5. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示二次开发程序可以立即卸载。
> 6. 代码使用了NX Open C++提供的UF和UI函数，通过调用这些函数实现与NX的交互。
> 7. 代码采用了NX二次开发的标准框架，包括初始化、执行主要逻辑、清理三个步骤。
> 8. 使用了NX提供的错误处理机制，通过检查UF函数的返回码来处理错误。
> 9. 代码简洁明了，通过一个简单的例子展示了NX Open C++二次开发的基本流程。
>
