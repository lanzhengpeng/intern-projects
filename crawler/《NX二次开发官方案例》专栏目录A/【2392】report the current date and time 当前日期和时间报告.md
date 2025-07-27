### 【2392】report the current date and time 当前日期和时间报告

#### 代码

```cpp
    /*HEAD REPORT_THE_CURRENT_DATE_AND_TIME CCC UFUN */  
    #include <stdio.h>  
    #include <time.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 新增的函数，主要用于打印系统日志。 */  
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
        int  
            now[2] = { -1, -1 };  
        char  
            dstr[21],  
            mstr[MAX_LINE_SIZE+1],  
            tstr[21];  
        time_t  
            the_time;  
        struct tm  
            *time_ptr;  
    /*  One way 里海译:翻译为：一条途径 */  
        the_time = time((time_t *)0);  
        time_ptr = localtime (&the_time);  
        sprintf( mstr, "%d/%d/%d %d:%02d:%02d",  
            time_ptr->tm_mday,  
            time_ptr->tm_mon,  
            time_ptr->tm_year + 1900,  
            time_ptr->tm_hour,  
            time_ptr->tm_min,  
            time_ptr->tm_sec );  
        uc1601(mstr, TRUE);  
    /*  Another way 里海译:另一种方式 */  
        UF_CALL(uc4582(now, 2, dstr, tstr));  
        sprintf(mstr, "%s %s", dstr, tstr);  
        uc1601(mstr, TRUE);  
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

> 这段代码是NX二次开发代码，主要功能是报告当前日期和时间。下面是对代码的详细解释：
>
> 1. 头文件包含：代码包含了一些必要的头文件，如stdio.h、time.h、string.h、uf.h、uf_ui.h、uf_cfi.h等，以便使用标准输入输出、时间操作、字符串操作以及NX的UF接口。
> 2. 错误报告函数：定义了一个report_error函数，用于在UF接口调用出错时报告错误信息，包括错误代码、出错文件、行号、函数调用以及错误描述。
> 3. 日期时间处理函数：定义了一个do_it函数，用于获取和格式化当前日期和时间。该函数使用两种方式：使用C标准库函数time和localtime获取当前时间，并格式化为字符串。调用UF接口函数uc4582获取当前日期和时间，并格式化为字符串。
> 4. 使用C标准库函数time和localtime获取当前时间，并格式化为字符串。
> 5. 调用UF接口函数uc4582获取当前日期和时间，并格式化为字符串。
> 6. 主函数：定义了一个ufusr函数，该函数在NX启动时被调用。在该函数中，首先调用UF_initialize初始化UF环境，然后调用do_it函数报告当前日期和时间，最后调用UF_terminate终止UF环境。
> 7. 卸载函数：定义了一个ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示立即卸载该UF。
>
> 总的来说，这段代码实现了获取当前日期和时间，并以两种不同的方式报告，同时提供了错误处理和NX的UF接口调用。
>
