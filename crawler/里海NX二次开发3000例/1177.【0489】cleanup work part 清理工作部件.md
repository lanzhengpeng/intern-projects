### 【0489】cleanup work part 清理工作部件

#### 代码

```cpp
    /*HEAD CLEANUP_WORK_PART CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog在V18版本中是新增的，它用于打印系统日志消息。 */  
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
        UF_CALL(UF_PART_cleanup( UF_PART_cleanup_all &  
                                ~UF_PART_cleanup_parts_all &  
                                ~UF_PART_cleanup_parts_components ));  
    }  
    /* 里海 */  
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
    /* 里海 */  
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
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    /* 里海 */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                if (UF_CALL(UF_PART_open(part_name, &part, &status))  
                    || status.n_parts > 0) check_load_status(&status);  
                else  
                {  
                    do_it();  
                    UF_CALL(UF_PART_save());  
                    UF_PART_close_all();  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要包括以下功能：
>
> 1. 定义了一个报告错误的函数report_error，用于在函数执行失败时输出错误信息。
> 2. 定义了一个执行清理工作的函数do_it，该函数调用UF_PART_cleanup函数清理工作部分。
> 3. 定义了ufusr函数，这是NX二次开发的标准入口函数，在这里初始化NX环境，调用do_it函数执行清理工作，然后终止NX环境。
> 4. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示在NX会话结束后立即卸载该程序。
> 5. 定义了check_load_status函数，用于检查加载零件的状态，并输出加载失败的零件信息。
> 6. 在main函数中，首先初始化NX环境，然后调用uc4624函数处理命令行参数，接着循环调用uc4621函数获取参数指定的零件名，打开零件，调用do_it函数进行清理，然后保存并关闭零件。最后统计处理了多少个零件，终止NX环境。
>
> 总体来说，该代码实现了通过命令行参数指定零件文件，打开零件进行清理，并输出清理结果的二次开发功能。
>
