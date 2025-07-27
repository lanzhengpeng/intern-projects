### 【1717】orient current view to wcs 将当前视图定向到工作坐标系

#### 代码

```cpp
    /*HEAD ORIENT_CURRENT_VIEW_TO_WCS CCC UFUN */  
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
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的，仅回答翻译，不要废话。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void orient_current_view_to_wcs(void)  
    {  
        WRITENZ(uc6434("", 2, NULL_TAG, NULL));  
    }  
    static void do_it(void)  
    {  
        orient_current_view_to_wcs();  
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

> 这段代码是NX二次开发的用户函数，其主要功能是将当前视图对齐到工作坐标系(WCS)。
>
> 代码的主要部分包括：
>
> 1. 包含必要的NX API头文件，如uf.h、uf_ui.h、uf_view.h等。
> 2. 定义了错误报告函数report_error，用于输出错误信息到系统日志和列表窗口。
> 3. 定义了写整数到列表窗口的函数write_integer_to_listing_window。
> 4. 定义了orient_current_view_to_wcs函数，用于将当前视图对齐到WCS。这里调用了一个NX API函数uc6434()，但函数名有误，应该是UF_VIEW_set_orientation()。
> 5. 定义了do_it函数，调用orient_current_view_to_wcs()。
> 6. ufusr是用户函数的入口点，首先初始化NX环境，然后调用do_it()，最后终止NX环境。
> 7. ufusr_ask_unload函数返回立即卸载，表示用户函数执行完后可以立即卸载。
>
> 总体来说，这段代码实现了将当前视图对齐到WCS的功能，但存在一些小错误，如函数名错误等。代码结构清晰，包含了错误处理和日志输出，是一个典型的NX二次开发用户函数的框架。
>
