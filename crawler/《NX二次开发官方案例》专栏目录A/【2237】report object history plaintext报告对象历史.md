### 【2237】report object history plaintext报告对象历史

#### 代码

```cpp
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
    static void report_object_history(tag_t object)  
    {  
        int  
            cre_version,  
            gm_time,  
            mod_version,  
            n_histories,  
            version;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            *machine,  
            msg[MAX_LINE_SIZE+1],  
            *program,  
            time_buff[21],  
            *user;  
        UF_PART_history_list_p_t  
            history_list;  
        UF_CALL(UF_PART_create_history_list(&history_list));  
        UF_CALL(UF_PART_ask_part_history(part, history_list));  
        UF_CALL(UF_PART_ask_num_histories(history_list, &n_histories));  
    /*  Should always be true but there may be part corruption! 译:应该始终为真，但可能会有部分损坏！ */  
        if (!UF_CALL(UF_OBJ_ask_cre_mod_versions(object, &cre_version,  
            &mod_version)) && (cre_version <= n_histories))  
        {  
            UF_CALL(UF_PART_ask_nth_history(history_list, n_histories -  
                cre_version, &program, &user, &machine, &version, &gm_time));  
            translate_gmtime(gm_time, time_buff);  
            ECHO("  first created: %s by %s in %s\n\n", time_buff, user, program);  
            UF_CALL(UF_PART_ask_nth_history(history_list, n_histories -  
                mod_version, &program, &user, &machine, &version, &gm_time));  
            translate_gmtime(gm_time, time_buff);  
            ECHO("  last modified: %s by %s in %s\n\n", time_buff, user, program);  
        }  
        else  
            ECHO("  object history corrupt\n\n");  
        UF_CALL(UF_PART_delete_history_list(history_list));  
    }

```

#### 代码解析

> 这段代码是用于查询和报告NX零件的历史记录信息的二次开发函数。
>
> 主要功能包括：
>
> 1. 创建历史记录列表：使用UF_PART_create_history_list函数创建一个历史记录列表。
> 2. 查询零件历史记录：调用UF_PART_ask_part_history函数，传入零件的tag和创建的历史记录列表，以查询该零件的所有历史记录。
> 3. 获取历史记录数量：使用UF_PART_ask_num_histories函数获取历史记录的数量。
> 4. 查询创建和修改历史：通过UF_PART_ask_nth_history函数查询零件的创建和最后修改历史记录，包括程序、用户、机器、版本和时间等信息。
> 5. 时间格式转换：将获取的GMT时间转换为可读的日期时间字符串，使用strftime函数。
> 6. 报告历史信息：根据查询结果，打印零件的首次创建和最后修改时间、用户和程序等信息。
> 7. 删除历史记录列表：调用UF_PART_delete_history_list函数删除创建的历史记录列表。
>
> 这段代码主要用于获取和展示NX零件的版本历史信息，对于跟踪零件的变更历史非常有用。
>
