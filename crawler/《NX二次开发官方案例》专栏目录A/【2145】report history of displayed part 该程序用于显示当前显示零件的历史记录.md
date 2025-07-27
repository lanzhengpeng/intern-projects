### 【2145】report history of displayed part 该程序用于显示当前显示零件的历史记录

#### 代码

```cpp
    /*HEAD REPORT_HISTORY_OF_DISPLAYED_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <time.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的。它是一个打印系统日志的函数。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void translate_gmtime(int gm_time, char *time_buff)  
    {  
        time_t  
            the_time;  
        struct tm  
            *time_ptr;  
        char  
            *time_format = "%d %b %Y %H:%M";  
        the_time = (time_t) gm_time;  
        time_ptr = localtime (&the_time);  
        strftime( time_buff, 21*sizeof(char), time_format, time_ptr);  
    }  
    static void report_part_history(tag_t part)  
    {  
        int  
            ii,  
            v_time,  
            n_histories,  
            version;  
        char  
            *program,  
            *machine,  
            msg[UF_UI_MAX_STRING_LEN+1],  
            time_buff[21],  
            *user;  
        UF_PART_history_list_p_t  
            history_list;  
        UF_CALL(UF_PART_create_history_list(&history_list));  
        UF_CALL(UF_PART_ask_part_history(part, history_list));  
        UF_CALL(UF_PART_ask_num_histories(history_list, &n_histories));  
        WRITE_D(n_histories);  
        for (ii = 0; ii < n_histories; ii++)  
        {  
            UF_CALL(UF_PART_ask_nth_history(history_list, ii, &program, &user,  
                &machine, &version, &v_time));  
            translate_gmtime(v_time, time_buff);  
            sprintf(msg, "%9d  %-20s  %-10s  %-15s  %s\n", version, time_buff,  
                machine, user, program);  
            WRITE(msg);  
        }  
        UF_CALL(UF_PART_delete_history_list(history_list));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        report_part_history(part);  
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

> 根据代码，这段NX二次开发代码主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在调用UF函数失败时打印错误信息到系统日志和Listing窗口。
> 2. 历史记录查询函数：定义了一个函数report_part_history，用于查询当前显示部件的历史版本记录，并打印到Listing窗口。其中使用了NX提供的部件历史记录API函数。
> 3. 初始化和执行：在ufusr函数中，首先调用UF_initialize初始化NX环境，然后调用do_it函数执行查询操作，最后调用UF_terminate结束NX环境。
> 4. 卸载函数：定义了ufusr_ask_unload函数，用于卸载该UF程序。
> 5. 宏定义：使用了宏UF_CALL来简化UF函数的调用，并在失败时调用report_error函数。
> 6. 时间转换函数：定义了translate_gmtime函数，用于将GMT时间转换为本地时间格式。
> 7. 交互式功能：代码实现了交互式功能，允许用户在NX中查询当前显示部件的历史版本记录。
>
> 总体来说，该代码实现了一个简单的NX二次开发程序，提供了查询部件历史版本记录的功能，并通过交互式方式在NX中执行。
>
