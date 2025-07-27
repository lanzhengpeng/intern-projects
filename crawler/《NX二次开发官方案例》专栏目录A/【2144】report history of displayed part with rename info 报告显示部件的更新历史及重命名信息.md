### 【2144】report history of displayed part with rename info 报告显示部件的更新历史及重命名信息

#### 代码

```cpp
    /*HEAD REPORT_HISTORY_OF_DISPLAYED_PART_WITH_RENAME_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <time.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
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
    static void report_part_history_rename_info(tag_t part)  
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
        UF_CALL(UF_PART_ask_part_history_with_rename_info(part, history_list));  
        UF_CALL(UF_PART_ask_num_histories(history_list, &n_histories));  
        WRITE_D(n_histories);  
        for (ii = 0; ii < n_histories; ii++)  
        {  
            UF_CALL(UF_PART_ask_nth_history(history_list, ii, &program, &user,  
                &machine, &version, &v_time));  
            translate_gmtime(v_time, time_buff);  
    /*  
    If the file has been renamed,   
    the information will appear in the "program" string.  
    */  
            sprintf_s(msg, sizeof(msg),"%9d  %-20s  %-10s  %-15s  %s\n",   
                    version, time_buff, machine, user, program);  
            WRITE(msg);  
        }  
        UF_CALL(UF_PART_delete_history_list(history_list));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        report_part_history_rename_info(part);  
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

> 这段代码是一个NX Open的二次开发示例，其主要功能是获取当前显示的零件的历史记录，包括重命名信息。
>
> 代码的主要流程包括：
>
> 1. 定义了宏ECHO和UF_CALL，用于输出错误信息和调用UF函数。
> 2. 定义了report_error函数，用于处理UF函数调用错误。
> 3. 定义了write_integer_to_listing_window函数，用于将整数输出到列表窗口。
> 4. 定义了translate_gmtime函数，用于将GMT时间转换为本地时间字符串。
> 5. 定义了report_part_history_rename_info函数，用于获取零件的历史记录，并输出到列表窗口。
> 6. 定义了do_it函数，用于获取当前显示的零件，并调用report_part_history_rename_info函数。
> 7. 定义了ufusr函数，是NX二次开发的入口函数。在这个函数中，首先调用UF_initialize进行初始化，然后调用do_it函数获取历史记录，最后调用UF_terminate进行清理。
> 8. 定义了ufusr_ask_unload函数，用于在卸载二次开发时立即卸载。
>
> 总体来说，这段代码实现了获取当前显示零件的历史记录，包括重命名信息的功能。
>
