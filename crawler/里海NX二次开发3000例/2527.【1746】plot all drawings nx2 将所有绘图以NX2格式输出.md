### 【1746】plot all drawings nx2 将所有绘图以NX2格式输出

#### 代码

```cpp
    /*HEAD PLOT_ALL_DRAWINGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_plot.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog 是 V18 中新增的函数。 */  
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
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
    static void do_it(void)  
    {  
        int  
            jobid;  
        tag_t  
            drawing = NULL_TAG,  
            part = UF_PART_ask_display_part();  
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
                1.0, 0.0,               /* scale and rotation 里海译:比例缩放和旋转 */  
                { 0.0, 0.0, 0.0 },      /* origin 里海译:origin的翻译是“起源”。 */  
                { 0.0, 0.0 },           /* offset 里海译:OFFSET的中文意思是“偏移量”。 */  
                "", "", "", "", "",     /* temporary pointers 里海译:临时指针 */  
                FALSE,                  /* print banner? 里海译:print banner?

打印横幅 */  
                UF_PLOT_PLOT_RELEASE,   /* plot option 里海译:根据上下文，plot option 可翻译为“绘图选项”或“作图选项”。 */  
                1,                      /* media number 里海译:媒体号 */  
                1,                      /* copies 里海译:复制品 */  
                1,                      /* pen type ; 1=width 2=color 里海译:笔类型；1=宽度 2=颜色 */  
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }  
            };  
    /*  Assign the pointers to point to the allocated space 里海译:将指针分配给已分配的空间 */  
        opts.plotter = plotter;  
        opts.node = node;  
        opts.jobname = jobname;  
        opts.pauseMsg = pauseMsg;  
        opts.bannerMsg = bannerMsg;  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, dname));  
            UF_CALL(UF_PLOT_drawing(dname, &opts, &jobid));  
            printf("%s - JOBID = %d\n", dname, jobid);  
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
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
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

> 这段NX二次开发代码主要用于实现将NX中的所有图纸进行批量打印的功能。主要思路是遍历当前打开的部件中的所有图纸，并调用UF_PLOT_drawing接口进行打印。
>
> 代码中包含了以下关键点：
>
> 1. 报错函数report_error，用于捕捉并打印出NX API调用错误。
> 2. ask_next_drawing函数，用于遍历当前打开部件中的所有图纸。
> 3. do_it函数，用于遍历所有图纸并调用UF_PLOT_drawing接口进行打印。
> 4. ufusr函数，NX二次开发入口函数，初始化NX API并调用do_it函数。
> 5. main函数，程序入口函数，用于处理命令行参数，打开指定的部件，然后调用do_it函数打印所有图纸。
> 6. check_load_status函数，用于检查部件打开状态，打印出打开失败的部件信息。
> 7. 使用了UF_PLOT_options_t结构体设置打印参数。
> 8. 代码注释详细，使用了中文注释来解释每个关键函数的作用。
>
> 总体来说，这段代码实现了在NX中批量打印所有图纸的功能，代码结构清晰，注释详尽，具有很好的参考价值。
>
