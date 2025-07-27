### 【1461】insure toolbar is visible 确保工具栏可见

#### 代码

```cpp
    /*HEAD INSURE_TOOLBAR_IS_VISIBLE CCC UFUN */  
    /*  This program will insure that the "my.tbr" toolbar is visible.  The my.tbr  
        file should be in an application directory.  E.g.  
        $UGII_USER_DIR/application  
    */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。在回答问题时，只需提供这个函数的翻译即可，无需添加其他内容。 */  
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
    static UF_UI_toolbar_id_t MY_toolbar_id = NULL;  
    static void do_it(void)  
    {  
        int  
            shown;  
        if (MY_toolbar_id == NULL)  
            UF_CALL(UF_UI_create_toolbar("my.tbr", UF_UI_SHOW, &MY_toolbar_id));  
        if (MY_toolbar_id != NULL)  
        {  
            UF_CALL(UF_UI_ask_toolbar_vis(MY_toolbar_id, &shown));  
            if (shown != UF_UI_SHOW)  
                UF_CALL(UF_UI_set_toolbar_vis(MY_toolbar_id, UF_UI_SHOW));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是一个NX Open的UF程序，用于确保名为“my.tbr”的工具栏在NX界面上可见。
>
> 主要思路是：
>
> 1. 定义了一个错误报告函数report_error，用于在出错时输出错误信息到syslog和listing窗口。
> 2. 定义了一个全局变量MY_toolbar_id，用于存储工具栏的ID。
> 3. 定义了一个do_it函数，用于创建工具栏并确保其可见。
> 4. 在ufusr函数中，先调用UF_initialize进行初始化，然后调用do_it函数，最后调用UF_terminate进行清理。
>
> do_it函数的逻辑是：
>
> 1. 如果MY_toolbar_id为NULL，调用UF_UI_create_toolbar创建工具栏，并设置MY_toolbar_id。
> 2. 如果MY_toolbar_id不为NULL，调用UF_UI_ask_toolbar_vis查询工具栏是否可见。
> 3. 如果工具栏不可见，调用UF_UI_set_toolbar_vis将其设置为可见。
>
> 这样，只要运行这个程序，就可以确保“my.tbr”工具栏在NX界面上始终可见。这个程序适用于需要经常使用特定工具栏的场景，可以简化操作，提高效率。
>
