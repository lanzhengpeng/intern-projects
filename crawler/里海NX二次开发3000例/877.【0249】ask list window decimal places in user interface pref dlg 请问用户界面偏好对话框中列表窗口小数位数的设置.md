### 【0249】ask list window decimal places in user interface pref dlg 请问用户界面偏好对话框中列表窗口小数位数的设置

#### 代码

```cpp
    /*HEAD ASK_LIST_WINDOW_DECIMAL_PLACES_IN_USER_INTERFACE_PREF_DLG CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，可以得知UF_print_syslog是V18版本中新增的功能。 */  
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
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
      int  
        mode,  
        dec_places;  
      UF_CALL(UF_UI_ask_iw_decimal_places( &mode, &dec_places ));  
      WRITE_D(UF_UI_SYSTEM_DECIMAL_PLACES);  
      WRITE_D(UF_UI_USER_DECIMAL_PLACES);  
      WRITE_D(mode);  
      if (mode == UF_UI_USER_DECIMAL_PLACES) WRITE_D(dec_places);  
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

> 这段代码是NX Open C++的一个二次开发示例，主要实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用的错误信息。
> 2. 定义了一个写整数到列表窗口的函数write_integer_to_listing_window，用于将整数参数输出到NX的列表窗口。
> 3. 定义了do_it函数，用于获取用户界面小数位模式及小数位数值，并输出到列表窗口。
> 4. 定义了ufusr函数，作为NX二次开发的入口点，在其中初始化UF模块，调用do_it函数获取并输出小数位信息，最后终止UF模块。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发应用程序可以立即卸载。
>
> 通过这段代码，可以获取和显示NX用户界面的小数位设置，包括系统默认的小数位数和用户自定义的小数位数。这为NX的二次开发提供了如何获取和操作NX用户界面参数的示例。
>
