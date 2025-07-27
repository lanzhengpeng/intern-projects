### 【2271】report part history of all parts listed in text files 报告文本文件中列出的所有零件的历史

#### 代码

```cpp
    /*HEAD REPORT_PART_HISTORY_OF_ALL_PARTS_LISTED_IN_TEXT_FILES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <time.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_assem.h>  
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
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
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
    /*  To run in external native mode UG:  
            program.exe /path/to/list_of_parts.txt  
        To run in external NX Manager mode:  
            program.exe -pim=yes /path/to/native_list_of_parts.txt  
        The content of the file should be in CLI format, e.g.  
        @DB/part_no_1/part_rev_1/  
        @DB/part_no_2/part_rev_2/  
    */  
    static void turn_off_loading_of_components(void)  
    {  
        UF_ASSEM_options_t  
            assy_options;  
        UF_CALL(UF_ASSEM_ask_assem_options(&assy_options));  
        if (assy_options.load_components != UF_ASSEM_dont_load_components)  
        {  
            assy_options.load_components = UF_ASSEM_dont_load_components;  
            UF_CALL(UF_ASSEM_set_assem_options(&assy_options));  
        }  
    }  
    int main( int argc, char *argv[] )  
    {  
        int  
            ii,  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        FILE  
            *the_file;  
        if (UF_CALL(UF_initialize())) return 1;  
        turn_off_loading_of_components();  
        for (ii = 1; ii < argc; ii++)  
            if ((the_file = fopen(argv[ii], "r")) != NULL)  
            {  
                while ((fgets(part_name, MAX_FSPEC_SIZE, the_file)) != NULL)  
                {  
                    part_name[strlen(part_name)-1] = '\0';  /* overwrite \n 里海译:覆盖写入 */  
                    printf("%d.  %s\n", ++cnt, part_name);  
                    UF_CALL(UF_PART_open(part_name, &part, &status));  
                    if (status.n_parts > 0) check_load_status(&status);  
                    if (!status.failed)  
                    {  
                        do_it();  
                        UF_PART_close_all();  
                    }  
                }  
                fclose(the_file);  
            }  
        printf("\nProcessed %d parts.\n", cnt);  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 获取当前显示部件的历史信息，包括版本号、修改时间、用户、机器和程序，并将这些信息输出到列表窗口。
> 2. 从文本文件中读取部件名称，逐个打开这些部件，并获取每个部件的历史信息。
> 3. 在打开部件时，关闭了组件的自动加载，以提高性能。
> 4. 对每个部件，输出其加载状态，包括成功或失败的部件名称及错误码。
> 5. 处理完所有部件后，输出处理的部件总数。
> 6. 使用宏定义了错误处理函数，方便在函数调用失败时输出错误信息。
> 7. 使用了标准的二次开发流程，包括初始化、执行功能函数、终止等。
> 8. 支持在UG和NX两种模式下运行，通过命令行参数来指定。
>
> 总的来说，这段代码实现了按部件名称批量获取历史信息的功能，并具备完善的错误处理和性能优化。
>
