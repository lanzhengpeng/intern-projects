### 【0748】create layout and fit all views 创建布局并适应所有视图

#### 代码

```cpp
    /*HEAD CREATE_LAYOUT_AND_FIT_ALL_VIEWS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layout.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是 V18 中新增的函数。 */  
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
        const char  
            layout_name[31]={"test1"};  
        int  
            status,  
            layout_style=4;  
        const char  
            view_names[][31]={"","","",""};  
        int  
            scale_option=2;  
        double  
            scale=1;  
        status=uc6460(layout_name, layout_style, view_names, scale_option, scale);  
        if (status != 0) printf("error %d\n",status);  
    }  
    /* 里海 */  
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

> 这段代码是NX的二次开发代码，其主要功能包括：
>
> 1. 头文件包含：代码包含了必要的头文件，如stdio.h、stdlib.h、string.h、uf.h、uf_ui.h、uf_layout.h等，以支持后续的函数调用。
> 2. 宏定义：ECHO(X)用于打印变量X的值，UF_CALL(X)用于调用函数X并报告错误。
> 3. 错误报告函数：report_error()用于打印函数调用失败时的错误信息。
> 4. 创建布局和视图函数：do_it()函数用于创建布局和视图，其中包含布局名称、样式、视图名称、缩放选项和缩放比例等参数。
> 5. UFusr主函数：该函数首先初始化NX环境，然后调用do_it()函数进行布局和视图的创建，最后终止NX环境。
> 6. 卸载函数：ufusr_ask_unload()用于设置立即卸载二次开发程序。
>
> 总体来说，这段代码实现了在NX中创建布局和视图的功能，通过定义宏、错误报告函数、布局和视图创建函数等，实现了布局和视图的创建逻辑，并通过UFusr主函数进行调用。
>
