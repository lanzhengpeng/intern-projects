### 【1437】report current users login name 报告当前用户的登录名

#### 代码

```cpp
    /*HEAD REPORT_CURRENT_USERS_LOGIN_NAME CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新增的功能，因此翻译为：UF_print_syslog在V18中是一个新增功能，仅需要提供翻译，不要添加其他无关的内容。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        char  
            your_name[30];  
        UF_CALL(uc4595(1, your_name));  /* API equiv. to GRIP's GPA &USERID 里海译:根据提供的信息，GPA和USERID在GRIP中的API等效翻译为：

GPA: grade point average (平均成绩点)
USERID: user identification number (用户识别号码) */  
        WRITE_S(your_name);  
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

> 这段代码是一个NX Open C++应用程序，用于报告当前登录用户的登录名。其主要功能和流程如下：
>
> 1. 包含了必要的NX Open头文件，如uf.h、uf_ui.h等。
> 2. 定义了宏UF_CALL，用于调用NX API函数，并在发生错误时记录错误信息。
> 3. 定义了report_error函数，用于打印错误信息到系统日志和控制台窗口。
> 4. 定义了WRITE_S宏，用于将字符串写入控制台窗口。
> 5. 定义了do_it函数，其中调用uc4595 API函数获取当前登录用户的登录名，并写入控制台窗口。
> 6. 定义了ufusr函数，作为程序的入口点。在该函数中，初始化NX系统，调用do_it函数，然后终止NX系统。
> 7. 定义了ufusr_ask_unload函数，用于指示应用程序可以被立即卸载。
>
> 总体而言，该代码通过调用NX的uc4595 API函数获取当前登录用户的登录名，并将其显示在控制台窗口中。该程序采用了NX Open C++开发，利用了NX提供的API函数来实现所需的功能。
>
