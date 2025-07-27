### 【1752】plot current drawing 当前绘图绘制

#### 代码

```cpp
    /*HEAD PLOT_CURRENT_DRAWING CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_draw.h>  
    #include <uf_plot.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的功能。 */  
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
            jobid;  
        tag_t  
            drawing = NULL_TAG;  
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
                1.0, 0.0,               /* scale and rotation 里海译:缩放和旋转 */  
                { 0.0, 0.0, 0.0 },      /* origin 里海译:原始的 */  
                { 0.0, 0.0 },           /* offset 里海译:offset 的中文翻译是：抵消、补偿、弥补、弥补差额。 */  
                "", "", "", "", "",     /* temporary pointers 里海译:临时指针 */  
                FALSE,                  /* print banner? 里海译:print banner 翻译为：打印横幅 */  
                UF_PLOT_PLOT_RELEASE,   /* plot option 里海译:plot option的翻译是“绘图选项”。 */  
                1,                      /* media number 里海译:媒体编号 */  
                1,                      /* copies 里海译:复制品 */  
                1,                      /* pen type ; 1=width 2=color 里海译:pen type; 1=width 2=color */  
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }  
            };  
    /*  Assign the pointers to point to the allocated space 里海译:将指针分配到分配的空间 */  
        opts.plotter = plotter;  
        opts.node = node;  
        opts.jobname = jobname;  
        opts.pauseMsg = pauseMsg;  
        opts.bannerMsg = bannerMsg;  
        if (!(UF_CALL(UF_DRAW_ask_current_drawing(&drawing))) &&  
            (drawing != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, dname));  
            if (!UF_CALL(UF_PLOT_drawing(dname, &opts, &jobid)))  
            {  
                WRITE_D(jobid);  
            }  
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

> 这段代码是一个NX二次开发示例，其主要功能是获取当前打开的图纸并打印。具体步骤包括：
>
> 1. 初始化NX环境
> 2. 获取当前打开的图纸
> 3. 打印图纸
>
> 在打印时，可以设置打印选项，如打印比例、旋转角度、偏移量等。同时，代码中还定义了一个错误报告函数report_error，用于在函数调用出错时输出错误信息。
>
> 此外，代码中包含了一个主函数main，用于在外部模式下运行。主函数首先初始化NX环境，然后读取命令行传入的零件文件路径，逐个打开并打印每个零件的图纸。最后，输出处理了多少个零件，并关闭NX环境。
>
> 综上所述，这段代码实现了获取当前打开图纸并打印的基本功能，并提供了外部运行模式，用户可以通过命令行传入零件文件路径来打印零件图纸。
>
