### 【1743】plot all drawings as one job 统一将所有图纸作为一个作业打印

#### 代码

```cpp
    /*HEAD PLOT_ALL_DRAWINGS_AS_ONE_JOB CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_plot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本新增的函数。该函数的作用是打印系统日志。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jobid,  
            n_dwgs;  
        tag_t  
            *dwgs;  
        char  
            bannerMsg[132+1] = { "" },  
            dname[MAX_ENTITY_NAME_SIZE+1],  
            jobname[16+1] = { "test" },  
            node[132+1] = { "www-gtac" },  
            pauseMsg[44+1] = { "" },  
            plotter[16+1] = { "GTACHP5MX" };  
        UF_PLOT_options_t  
            opts =  
            {  
                1.0, 0.0,               /* scale and rotation 里海译:缩放和平移 */  
                { 0.0, 0.0, 0.0 },      /* origin 里海译:根据文中的内容，origin 的翻译是“起源”或“来源”。 */  
                { 0.0, 0.0 },           /* offset 里海译:OFFSET 的翻译是“偏移量”。 */  
                "", "", "", "", "",     /* temporary pointers 里海译:临时指针 */  
                FALSE,                  /* print banner? 里海译:print banner 翻译为打印横幅。 */  
                UF_PLOT_PLOT_SAVE,      /* plot option 里海译:plot option的翻译是“绘图选项”。 */  
                1,                      /* media number 里海译:媒体号码 */  
                1,                      /* copies 里海译:根据文中内容，可以翻译为“副本”。 */  
                1,                      /* pen type ; 1=width 2=color 里海译:pen type ; 1=width 2=color */  
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }  
            };  
    /*  Assign the pointers to point to the allocated space 里海译:将指针指向分配的空间 */  
        opts.plotter = plotter;  
        opts.node = node;  
        opts.jobname = jobname;  
        opts.pauseMsg = pauseMsg;  
        opts.bannerMsg = bannerMsg;  
        UF_CALL(UF_DRAW_ask_drawings(&n_dwgs, &dwgs));  
        if (n_dwgs > 0)  
        {  
            for (ii = 0; ii < (n_dwgs-1); ii++)  
            {  
                UF_CALL(UF_OBJ_ask_name(dwgs[ii], dname));  
                UF_CALL(UF_PLOT_drawing(dname, &opts, &jobid));  
            }  
            opts.option = UF_PLOT_PLOT_RELEASE;  
            UF_CALL(UF_OBJ_ask_name(dwgs[ii], dname));  
            UF_CALL(UF_PLOT_drawing(dname, &opts, &jobid));  
            WRITE_D(jobid);  
            UF_free(dwgs);  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
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
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
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

> 这段代码是一个NX二次开发示例，主要实现了批量打印多个NX图纸的功能。代码的主要功能包括：
>
> 1. 初始化和终止NX，确保正确打开和关闭NX。
> 2. 定义了一个错误报告函数report_error，用于在发生错误时打印错误信息。
> 3. 定义了一个写入整数到列表窗口的函数write_integer_to_listing_window。
> 4. do_it函数是主函数，首先获取当前所有打开的图纸，然后逐个调用UF_PLOT_drawing函数打印图纸，最后一个图纸调用时设置选项为UF_PLOT_PLOT_RELEASE以完成打印。
> 5. ufusr函数是NX二次开发的标准入口函数，调用do_it完成批量打印。
> 6. main函数用于在命令行模式下接收多个零件文件路径，打开每个零件并调用do_it打印图纸。
> 7. check_load_status函数用于检查零件加载状态，并打印出错信息。
> 8. 通过UF_PART_open和UF_PART_close_all函数管理零件的打开和关闭。
>
> 总体来说，这段代码实现了在NX中批量打印多个图纸的功能，并且通过命令行参数支持打印指定零件文件中的图纸。
>
