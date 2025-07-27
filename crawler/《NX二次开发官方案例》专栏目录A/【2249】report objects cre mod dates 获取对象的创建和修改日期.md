### 【2249】report objects cre mod dates 获取对象的创建和修改日期

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
    static void report_objects_cre_mod_dates(tag_t object)  
    {  
        int  
            cre_version,  
            gm_time,  
            ii,  
            mod_version,  
            n_histories,  
            version;  
        tag_t  
            part;  
        char  
            *machine,  
            msg[UF_UI_MAX_STRING_LEN+1],  
            *program,  
            time_buff[21],  
            *user;  
        UF_PART_history_list_p_t  
            history_list;  
        UF_CALL(UF_OBJ_ask_owning_part(object, &part));  
        UF_CALL(UF_PART_create_history_list(&history_list));  
        UF_CALL(UF_PART_ask_part_history(part, history_list));  
        UF_CALL(UF_PART_ask_num_histories(history_list, &n_histories));  
        UF_CALL(UF_OBJ_ask_cre_mod_versions(object, &cre_version,  
            &mod_version));  
        for (ii = 0; ii < n_histories; ii++)  
        {  
            UF_CALL(UF_PART_ask_nth_history(history_list, ii,  
                &program, &user, &machine, &version, &gm_time));  
            if (version == cre_version) break;  
        }  
        if (ii < n_histories)  
        {  
            translate_gmtime(gm_time, time_buff);  
            sprintf(msg, "  first created: %s", time_buff);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        for (ii = 0; ii < n_histories; ii++)  
        {  
            UF_CALL(UF_PART_ask_nth_history(history_list, ii,  
                &program, &user, &machine, &version, &gm_time));  
            if (version == mod_version) break;  
        }  
        if (ii < n_histories)  
        {  
            translate_gmtime(gm_time, time_buff);  
            sprintf(msg, "  last modified: %s", time_buff);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        UF_CALL(UF_PART_delete_history_list(history_list));  
    }

```

#### 代码解析

> 这段代码的主要功能是获取NX部件对象的创建和修改日期，并将其格式化后输出到列表窗口。具体步骤如下：
>
> 1. 获取当前部件对象，并创建一个历史列表。
> 2. 获取部件对象的历史列表，并获取列表中的历史记录数量。
> 3. 获取部件对象的创建版本号和修改版本号。
> 4. 遍历历史列表，查找与创建版本号和修改版本号对应的记录。
> 5. 对于创建版本号对应的记录，将GMT时间戳转换为本地时间，并格式化为"日 月 年 时:分"的格式，然后输出到列表窗口。
> 6. 对于修改版本号对应的记录，也执行类似的转换和输出操作。
> 7. 最后删除创建的历史列表。
>
> 通过以上步骤，代码实现了查询和输出NX部件对象的创建和修改日期信息的功能。
>
