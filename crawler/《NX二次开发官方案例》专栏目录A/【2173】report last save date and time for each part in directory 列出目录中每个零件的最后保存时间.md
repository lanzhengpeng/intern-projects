### 【2173】report last save date and time for each part in directory 列出目录中每个零件的最后保存时间

#### 代码

```cpp
    /*HEAD REPORT_LAST_SAVE_DATE_AND_TIME_FOR_EACH_PART_IN_DIRECTORY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <time.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。因此，对于这个问题的答案是：

UF_print_syslog是V18版本中新增的函数。 */  
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
    /* qq3123197280 */  
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /* qq3123197280 */  
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
    /* qq3123197280 */  
    static void report_most_recent_part_history(tag_t part)  
    {  
        int  
            v_time,  
            n_histories,  
            version;  
        char  
            *program,  
            *machine,  
            time_buff[21],  
            *user;  
        UF_PART_history_list_p_t  
            history_list;  
        UF_CALL(UF_PART_create_history_list(&history_list));  
        UF_CALL(UF_PART_ask_part_history(part, history_list));  
        UF_CALL(UF_PART_ask_num_histories(history_list, &n_histories));  
        if( n_histories > 0 )  
        {      
            UF_CALL(UF_PART_ask_nth_history(history_list, 0, &program, &user,  
                                                 &machine, &version, &v_time ));  
            translate_gmtime(v_time, time_buff);  
        /* optionally you could uncomment the next  
            printf statement to get additional info about the part */  
           /* printf( "%9d  %-20s  %-10s  %-15s  %s\n", version, time_buff,  
                 machine, user, program); */  
            printf( "%-20s  \n\n",time_buff );  
        }  
        UF_CALL(UF_PART_delete_history_list(history_list));  
    }  
    int main( void )  
    {  
        int  
            cnt = 0,  
            resp;  
        tag_t  
            part;  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fname[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (UF_CALL(UF_initialize())) return 1;  
        while (printf("Enter directory path:\n") && gets(dirspec) &&  
            strlen(dirspec))  
        {  
            uc4508(dirspec, (1<<13)|(1<<11), 0, "*.prt");  
            while ((resp = uc4518()) == 0)  
            {  
                UF_CALL(uc4600(fname));  
                UF_CALL(uc4575(dirspec, 2, fname, part_name));  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    printf("%d.  %s   :   ", ++cnt, part_name);  
                    report_most_recent_part_history( part );  
                    UF_PART_close_all();  
                }  
            }  
            UF_CALL(uc4548());  
            printf("\nProcessed %d parts.\n", cnt);  
        }  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个用于查询NX零件目录中每个零件的最近保存时间及保存用户信息的二次开发代码。
>
> 主要功能包括：
>
> 1. 初始化NX环境，并提示用户输入零件目录路径。
> 2. 遍历目录中的所有零件文件，并打开每个零件。
> 3. 查询每个零件的最近保存历史，包括保存时间、保存用户等信息。
> 4. 打印每个零件的最近保存时间和保存用户。
> 5. 统计处理过的零件数量。
> 6. 关闭NX环境。
>
> 代码中的主要函数包括：
>
> 1. report_error：用于打印错误信息。
> 2. check_load_status：用于检查打开零件时的状态。
> 3. translate_gmtime：用于将GMT时间转换为本地时间。
> 4. report_most_recent_part_history：用于查询并打印零件的最近保存历史。
> 5. main：主函数，用于流程控制。
>
> 这段代码实现了对零件目录中每个零件的最近保存时间和保存用户信息的查询，对于需要了解零件历史信息的用户非常有用。
>
