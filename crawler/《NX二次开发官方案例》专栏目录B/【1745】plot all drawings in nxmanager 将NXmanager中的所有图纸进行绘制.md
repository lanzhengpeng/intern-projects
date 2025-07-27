### 【1745】plot all drawings in nxmanager 将NXmanager中的所有图纸进行绘制

#### 代码

```cpp
    /*HEAD PLOT_ALL_DRAWINGS_IN_UGMANGER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_plot.h>  
    #include <uf_cfi.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog在V18中是新增的，只需回答翻译，不要添加其他内容。 */  
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
                 1.0, 0.0,               /* scale and rotation 里海译:scale 和 rotation 的中文翻译分别是“缩放”和“旋转”。 */  
                 { 0.0, 0.0, 0.0 },      /* origin 里海译:起源 */  
                 { 0.0, 0.0 },           /* offset 里海译:offset的中文意思是“抵消；补偿；弥补”。

主要含义包括：

1. 抵消：指相互对立或相反的事物或力量相互抵消。
2. 补偿：指对损失的补偿或赔偿。
3. 弥补：指对不足之处的弥补或补足。

综上所述，offset主要指相互对立或相反的事物相互抵消，损失得到补偿或赔偿，以及不足之处得到弥补或补足。 */  
                 "", "", "", "", "",     /* temporary pointers 里海译:临时指针 */  
                 FALSE,                  /* print banner? 里海译:print banner的中文翻译是“打印横幅”。 */  
                 UF_PLOT_PLOT_RELEASE,   /* plot option 里海译:翻译plot option为：绘图选项 */  
                 1,                      /* media number 里海译:媒体编号 */  
                 1,                      /* copies 里海译:复印件 */  
                 1,                      /* pen type ; 1=width 2=color 里海译:笔类型；1=宽度 2=颜色 */  
                 { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }  
             };  
     /*  Assign the pointers to point to the allocated space 里海译:将指针分配给分配的空间 */  
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
    int main( int argc, char *argv[] )  
    {  
        const char  
            *args[1] = { "-a" };  
        tag_t  
            part;  
        UF_PART_load_status_t  
            status;  
        char  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            encoded_name[MAX_FSPEC_SIZE+1];  
        if (argc > 1) strcpy(part_number, argv[1]);  
        else strcpy(part_number, "");  
        if (argc > 2) strcpy(part_revision, argv[2]);  
        else strcpy(part_revision, "");  
        if (argc > 3) strcpy(part_file_type, argv[3]);  
        else strcpy(part_file_type, "");  
        if (argc > 4) strcpy(part_file_name, argv[4]);  
        else strcpy(part_file_name, "");  
        printf("calling UF_UGMGR_initialize\n");  
        if (!UF_CALL(UF_UGMGR_initialize(1, args)))  
        {  
            if (!UF_CALL(UF_UGMGR_encode_part_filename( part_number, part_revision,  
                part_file_type, part_file_name, encoded_name)))  
            {  
                if (UF_CALL(UF_PART_open(encoded_name, &part, &status))  
                    || status.n_parts > 0)  
                    check_load_status(&status);  
                else  
                {  
                    do_it();  
                    UF_PART_close_all();  
                }  
            }  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，其主要功能是遍历当前打开的零件中的所有图纸，并打印输出图纸。
>
> 代码的主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于在调用UF函数出错时报告错误信息。
> 2. 定义检查加载状态的函数check_load_status，用于在加载零件时检查加载状态。
> 3. 定义遍历零件中图纸的函数ask_next_drawing，用于获取当前零件中的下一个图纸。
> 4. 定义主函数do_it，用于遍历零件中的所有图纸并打印输出。
> 5. 定义ufusr函数，用于在NX启动时调用do_it函数。
> 6. 定义ufusr_ask_unload函数，用于在NX关闭时卸载。
> 7. 定义主函数main，用于初始化UG，打开零件，调用do_it函数，最后关闭零件并终止UG。
>
> 通过以上步骤，这段代码实现了遍历当前零件中的所有图纸，并打印输出的功能。
>
