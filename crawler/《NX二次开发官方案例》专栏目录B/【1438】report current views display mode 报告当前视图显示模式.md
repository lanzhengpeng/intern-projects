### 【1438】report current views display mode 报告当前视图显示模式

#### 代码

```cpp
    /*HEAD REPORT_CURRENT_VIEWS_DISPLAY_MODE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，关于UF_print_syslog的翻译如下：

UF_print_syslog是V18版本中的新增功能。

翻译内容：
UF_print_syslog是V18版本中的新增功能。

只回答译文，不要添加其他内容。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            work_view;  
        UF_VIEW_visualization_t  
            view_data;  
        UF_CALL(UF_VIEW_ask_work_view(&work_view));  
        UF_CALL(UF_VIEW_ask_visualization(work_view, &view_data));  
        WRITE_D(view_data.display_mode);  
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

> 这段NX二次开发代码的主要功能是获取当前视图的显示模式并报告给用户。以下是代码的主要组成部分：
>
> 1. 包含头文件：包含了必要的NX Open头文件，如uf.h、uf_ui.h、uf_view.h等。
> 2. 错误报告函数：report_error()用于检测函数调用是否成功，如果失败，会报告错误信息。
> 3. 写入整数值函数：write_integer_to_listing_window()用于将整数值写入列表窗口。
> 4. 获取视图数据函数：do_it()用于获取当前工作视图的显示模式，并写入列表窗口。
> 5. 用户自定义函数：ufusr()是用户自定义函数，用于初始化和执行do_it()函数。
> 6. 卸载函数：ufusr_ask_unload()用于卸载用户自定义函数。
> 7. 获取工作视图：使用UF_VIEW_ask_work_view()获取当前工作视图。
> 8. 获取视图显示模式：使用UF_VIEW_ask_visualization()获取视图的显示模式。
> 9. 打印日志和列表窗口：使用UF_print_syslog()和UF_UI_write_listing_window()打印日志和列表窗口。
>
> 总的来说，这段代码通过NX Open API获取当前视图的显示模式，并以日志和列表窗口的形式呈现给用户。
>
