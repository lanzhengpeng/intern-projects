### 【1896】report all clearance datasets 报告所有间隙数据集

#### 代码

```cpp
    /*HEAD REPORT_ALL_CLEARANCE_DATASETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <time.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_clear.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我理解 UF_print_syslog 是一个在 V18 版本中新增的函数。以下是该函数的翻译：

UF_print_syslog 是在 V18 版本中新增的。

该函数的功能是将系统日志信息打印到日志文件中。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnD(X,n) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void ensure_fully_loaded(tag_t item)  
    {  
        tag_t  
            part;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        UF_ASSEM_ask_part_name_of_child(  
            UF_ASSEM_ask_inst_of_part_occ(  
            UF_ASSEM_ask_part_occurrence(item)), partname);  
        if ((UF_PART_is_loaded(partname)) != 1)  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            report_load_status(&error_status);  
        }  
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
    static void do_it(void)  
    {  
        int  
            n_datasets,  
            mode;  
        tag_t  
            dataset = NULL_TAG,  
            object1 = NULL_TAG,  
            object2 = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            dataset_name[MAX_LINE_SIZE+1],  
            end_time[21],  
            start_time[21];  
        UF_CLEAR_summary_t  
            summary;  
        UF_CLEAR_interf_data_t  
            data;  
        UF_CALL(UF_CLEAR_count_datasets(part, &n_datasets));  
        WRITE_D(n_datasets);  
        while (!UF_CALL(UF_CLEAR_ask_next_dataset(part, &dataset)) &&  
            (dataset != NULL_TAG))  
        {  
            WRITE("\n");  
            UF_CALL(UF_CLEAR_ask_dataset_name(dataset, dataset_name));  
            WRITE_S(dataset_name);  
            UF_CALL(UF_CLEAR_ask_analysis_mode(dataset, &mode));  
            WRITE_D(mode);  
            UF_CALL(UF_CLEAR_do_clearance_analysis(dataset));  
            UF_CALL(UF_CLEAR_ask_results(dataset, &summary));  
            translate_gmtime(summary.start_time, start_time);  
            WRITE_S(start_time);  
            translate_gmtime(summary.end_time, end_time);  
            WRITE_S(end_time);  
            WRITE_D(summary.run_time);  
            WRITE_D(summary.version);  
            WRITE_D(summary.analysis_mode);  
            WRITE_D(summary.num_lists);  
            WRITE_D(summary.n_list1);  
            WRITE_D(summary.n_list2);  
            WRITE_D(summary.n_pairs);  
            WRITE_D(summary.excluded_pairs);  
            WRITE_D(summary.changed_pairs);  
            WRITE_D(summary.changed_objs);  
            WRITE_D(summary.checked_pairs);  
            WRITE_D(summary.new_n_hard);  
            WRITE_D(summary.new_n_soft);  
            WRITE_D(summary.new_n_containment);  
            WRITE_D(summary.new_n_all_interf);  
            WRITE_D(summary.n_hard);  
            WRITE_D(summary.n_soft);  
            WRITE_D(summary.n_touching);  
            WRITE_D(summary.n_containment);  
            WRITE_D(summary.n_all_interf);  
            WRITE_D(summary.job_aborted);  
            WRITE_D(summary.num_zones);  
            while (!UF_CALL(UF_CLEAR_ask_next_interf(dataset, &object1, &object2))  
                && (object1 != NULL_TAG) && (object2 != NULL_TAG))  
            {  
                ensure_fully_loaded(object1);  
                ensure_fully_loaded(object2);  
                if (!UF_CALL(UF_CLEAR_ask_interf_data(dataset, object1, object2,  
                    &data)))  
                {  
                    WRITE_D(data.type);  
                    WRITE_L(data.new_interference);  
                    WRITE_D(data.n_bodies);  
                    WRITEnD((int *)data.interf_bodies, data.n_bodies);  
                    if (data.n_bodies > 0) UF_free(data.interf_bodies);  
                    WRITE3F(data.point1);  
                    WRITE3F(data.point2);  
                    WRITE_S(data.text);  
                    WRITE_D(data.interf_num);  
                }  
            }  
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

```

#### 代码解析

> 这段代码是NX的二次开发代码，其主要功能是遍历并报告当前活动零件中的所有间隙分析结果。具体来说，代码的主要内容包括：
>
> 1. 定义了报告错误和写入日志的宏函数。
> 2. 定义了写入不同类型数据到日志窗口的函数。
> 3. 获取当前活动零件，并遍历其中的所有间隙分析数据集。
> 4. 对于每个数据集，报告其名称、分析模式、开始结束时间、运行时间、版本等信息。
> 5. 对于每个数据集，遍历其中的所有干涉项，报告干涉类型、是否为新增干涉、干涉体、干涉点等信息。
> 6. 在遍历过程中，确保参与干涉的对象已完全加载。
> 7. 在函数ufusr中初始化NX环境，执行主体功能，然后终止NX环境。
> 8. 提供了卸载函数，以便在不需要时卸载该代码。
>
> 总体来说，这段代码利用NX提供的间隙分析API，实现了遍历和报告间隙分析结果的功能。代码结构清晰，包含错误处理和日志记录，具有较好的可读性和可维护性。
>
