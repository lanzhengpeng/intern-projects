### 【1748】plot all drawings to one disk file 将所有绘图保存到单个磁盘文件中

#### 代码

```cpp
    /*HEAD PLOT_ALL_DRAWINGS_TO_ONE_DISK_FILE CCC UFUN */  
    /* This sample program is valid for PRE-NX3 versions only 里海译:这个示例程序仅适用于PRE-NX3版本。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog是在V18版本中新增的功能。因此，只翻译功能名称，无需添加任何注释或解释。 */  
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
    static void build_similar_filespec(char *fspec, int ftype)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        UF_CALL(uc4575(dspec, ftype, part_name, fspec));  
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
            dname[MAX_ENTITY_NAME_SIZE+1],  
            bannerMsg[132+1] = { "" },  
            jobname[16+1] = { "" },  
            node[132+1] = { "" },  
            pauseMsg[44+1] = { "" },  
            plotter[16+1] = { "TODISK" };  
        UF_PLOT_options_t  
            opts =  
            {  
                1.0, 0.0,               /* scale and rotation 里海译:比例和旋转 */  
                { 0.0, 0.0, 0.0 },      /* origin 里海译:起源 */  
                { 0.0, 0.0 },           /* offset 里海译:根据上下文，offset在此处翻译为“偏移量”。 */  
                "", "", "", "", "",     /* temporary pointers 里海译:临时指针 */  
                FALSE,                  /* print banner? 里海译:打印横幅 */  
                UF_PLOT_PLOT_SAVE,      /* plot option 里海译:plot option的译文为“绘图选项”。 */  
                1,                      /* media number 里海译:媒体编号 */  
                1,                      /* copies 里海译:根据上下文，"copies"可翻译为：

1. 复印件
2. 复制品
3. 抄本
4. 副本
5. 拷贝
6. 复写本
7. 一式两份

根据具体上下文，选择合适的翻译。 */  
                1,                      /* pen type ; 1=width 2=color 里海译:笔类型：1=宽度 2=颜色 */  
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }  
            };  
    /*  Assign the pointers to point to the allocated space 里海译:分配指针，使其指向已分配的内存空间。 */  
        opts.plotter = plotter;  
        opts.node = node;  
        opts.jobname = jobname;  
        opts.pauseMsg = pauseMsg;  
        opts.bannerMsg = bannerMsg;  
        build_similar_filespec(opts.node, 34);  
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
            part_name[UF_CFI_MAX_PATH_NAME_SIZE];  
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

> 根据代码内容，这是一段基于NX的二次开发代码，其主要功能是批量绘制NX图纸到单个磁盘文件中。以下是代码的主要组成部分：
>
> 1. 包含必要的NX开发库头文件，如uf.h、uf_ui.h、uf_draw.h等。
> 2. 定义了错误报告函数report_error，用于输出错误信息到系统日志和列表窗口。
> 3. 定义了build_similar_filespec函数，用于构建与当前打开部件相似的文件路径。
> 4. 定义了do_it函数，用于执行绘制操作。首先获取所有图纸，然后逐个调用UF_PLOT_drawing函数绘制到磁盘文件。
> 5. 定义了ufusr函数，作为NX二次开发的入口函数。在函数中初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，用于处理插件卸载。
> 7. 定义了check_load_status函数，用于检查部件加载状态。
> 8. main函数作为程序的入口。首先初始化NX环境，然后处理命令行传入的部件文件，打开每个部件，执行绘制操作，最后关闭所有部件并终止NX环境。
>
> 总体来说，这段代码实现了NX图纸的批量绘制功能，适用于PRE-NX3版本。
>
