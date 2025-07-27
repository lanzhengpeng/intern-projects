### 【2498】report system info 报告系统信息

#### 代码

```cpp
    /*HEAD REPORT_SYSTEM_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中的新功能，用于在系统日志中打印消息。在V18版本之前，并没有这个函数。 */  
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
    #define WRITE_L(X) (write_long_to_listing_window(#X, X))  
    void write_long_to_listing_window(char *title, long number)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %ld\n", title, number);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
        UF_system_info_t  
            info;  
        if (!UF_CALL(UF_ask_system_info(&info)))  
        {  
            WRITE_S(info.date_buf);  
            WRITE_S(info.user_name);  
            WRITE_S(info.program_name);  
            WRITE_S(info.node_name);  
            WRITE_S(info.os_name);  
            WRITE_S(info.os_version);  
            WRITE_L(info.physical_memory);  
            UF_CALL(UF_free_system_info(&info));  
        }  
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
    int main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++程序，其主要功能是报告NX系统的信息。具体来说：
>
> 1. 定义了一个错误报告函数report_error，用于在出错时打印错误信息到系统日志和控制台。
> 2. 定义了两个宏WRITE_L和WRITE_S，用于将长整型和字符串信息输出到列表窗口。
> 3. 定义了一个函数do_it，该函数使用UF_ask_system_info获取NX系统的信息，并使用WRITE_L和WRITE_S将信息输出到列表窗口。
> 4. 定义了ufusr函数，该函数是NX二次开发的入口函数，在其中调用了UF_initialize、do_it和UF_terminate。
> 5. 定义了ufusr_ask_unload函数，用于在卸载时立即卸载。
> 6. 主函数main中，初始化NX，调用do_it函数，然后终止NX。
>
> 总体来说，该程序通过调用NX提供的API获取系统信息，并输出到列表窗口，是一个典型的NX二次开发示例。
>
