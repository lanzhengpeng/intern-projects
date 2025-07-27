### 【2272】report part history 报告零件历史

#### 代码

```cpp
    #include <time.h>  
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
            msg[MAX_LINE_SIZE],  
            *time_format = "%d %b %Y %H:%M",  
            time_buff[21],  
            *user;  
        time_t  
            displayed_time;  
        struct tm  
            *time_ptr;  
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
            displayed_time = (time_t) v_time;  
            time_ptr = localtime ( &displayed_time );  
            strftime(time_buff, sizeof(time_buff), time_format, time_ptr );  
            sprintf(msg, "%9d  %-20s  %-10s  %-15s  %s\n", version, time_buff,  
                machine, user, program);  
            WRITE(msg);  
        }  
        UF_CALL(UF_PART_delete_history_list(history_list));  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于查询和显示NX部件的版本历史记录。
>
> 主要功能包括：
>
> 1. 定义宏WRITE和WRITE_D，分别用于向列表窗口输出字符串和整数。
> 2. 定义函数write_integer_to_listing_window，用于向列表窗口输出整数和对应的标题。
> 3. 定义函数report_part_history，用于查询指定部件的版本历史记录并输出。
> 4. 使用UF_PART_create_history_list创建历史记录列表。
> 5. 使用UF_PART_ask_part_history查询指定部件的历史记录。
> 6. 使用UF_PART_ask_num_histories获取历史记录的数量。
> 7. 遍历历史记录列表，获取每个历史记录的版本号、时间、用户名、机器名和程序名。
> 8. 使用strftime函数格式化时间戳。
> 9. 将每个历史记录的信息格式化成字符串并输出到列表窗口。
> 10. 使用UF_PART_delete_history_list删除历史记录列表。
> 11. 整个流程通过NX Open API调用实现。
>
> 这段代码实现了查询和显示NX部件版本历史的功能，通过NX Open API调用，并以列表窗口的形式展示了历史记录的详细信息。
>
