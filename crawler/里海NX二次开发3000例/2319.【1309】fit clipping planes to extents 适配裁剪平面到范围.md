### 【1309】fit clipping planes to extents 适配裁剪平面到范围

#### 代码

```cpp
    /*HEAD FIT_CLIPPING_PLANES_TO_EXTENTS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中提到的UF_print_syslog是V18版本新增的内容，可以翻译为：

UF_print_syslog是V18版本新增的函数。 */  
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
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ip2[2] = { 3, 3 };  
        double  
            rp3[2] = { 0, 0 };  
        WRITENZ(uc6437("", ip2, rp3));  
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

> 这段代码是NX（Unigraphics）软件的二次开发代码，主要用于在NX中调用UF函数来控制视图的裁剪平面。
>
> 主要内容包括：
>
> 1. 头文件包含：包含了必要的头文件，如stdio.h、string.h、uf.h、uf_ui.h、uf_view.h。
> 2. 错误报告函数：定义了report_error函数，用于在调用UF函数出错时报告错误信息。
> 3. 整数写入窗口函数：定义了write_integer_to_listing_window函数，用于将整数写入NX的列表窗口。
> 4. 主体函数：定义了do_it函数，其中调用了UF函数uc6437来设置视图的裁剪平面。这里使用了硬编码的参数。
> 5. NX启动和终止函数：定义了ufusr函数，用于在NX启动时初始化UF模块，调用do_it函数，并在NX终止时关闭UF模块。
> 6. 卸载请求函数：定义了ufusr_ask_unload函数，用于在NX卸载用户自定义模块时立即卸载。
>
> 总体来说，这段代码主要用于在NX中设置视图的裁剪平面，并包含错误处理和日志输出功能。
>
