### 【1742】plot all drawings and rename despooled files 绘制所有图纸并重命名输出文件

#### 代码

```cpp
    /*HEAD PLOT_ALL_DRAWINGS_AND_RENAME_DESPOOLED_FILES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，V18中新增了UF_print_syslog函数。因此，针对此文档，我的翻译如下：

UF_print_syslog是V18新增的函数。 */  
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
    static logical file_exists(char *file_spec)  
    {  
        int  
            in_session,  
            missing;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &missing));  
        in_session = UF_PART_is_loaded(file_spec);  
        if (missing && (in_session != 1) && (in_session != 2))  
            return FALSE;  
        else  
            return TRUE;  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            jobid;  
        tag_t  
            drawing = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            despooled_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            despooled_ext[UF_CFI_MAX_FILE_NAME_SIZE],  
            despooled_file_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            despooled_file_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            dname[MAX_ENTITY_NAME_SIZE+1],  
            jobname[16+1] = { "temp"},  
            moved_to_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            node[132+1],  
            plotter[16+1];  
        UF_PLOT_options_t  
            opts =  
            {  
                1.0, 0.0,               /* scale and rotation 里海译:缩放和平移 */  
                { 0.0, 0.0, 0.0 },      /* origin 里海译:原始的 */  
                { 0.0, 0.0 },           /* offset 里海译:OFFSET 的中文翻译是“偏移量”或“补偿量”。 */  
                "", "", "", "", "",     /* temporary pointers 里海译:临时指针 */  
                FALSE,                  /* print banner? 里海译:打印横幅 */  
                UF_PLOT_PLOT_RELEASE,   /* plot option 里海译:翻译plot option，只回答译文，不要废话 */  
                1,                      /* media number 里海译:媒体号码 */  
                1,                      /* copies 里海译:根据上下文，copies可以翻译为“副本”，“复制品”或“复印件”。 */  
                1,                      /* pen type ; 1=width 2=color 里海译:笔的类型：1=宽度 2=颜色 */  
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }  
            };  
    /*  Assign the pointers to point to the allocated space 里海译:将指针指向已分配的空间 */  
        opts.plotter = plotter;  
        opts.node = node;  
        opts.jobname = jobname;  
    /*  Modify these values to match the node & queue which despools to disk 里海译:请将这些值修改为与节点和队列匹配，后者将数据写入磁盘。 */  
        strcpy(node, "www-gtac");  
        strcpy(plotter, "DISK_HPGL");  
        strcpy(despooled_dir, "/cyp/gtac01/utils/plot_files");  
        strcpy(despooled_ext, ".hpp");  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, dname));  
            UF_CALL(UF_PLOT_drawing(dname, &opts, &jobid));  
            sprintf(despooled_file_name, "%s_%d%s", jobname, jobid, despooled_ext);  
            UF_CALL(uc4575(despooled_dir, 0, despooled_file_name,  
                despooled_file_spec));  
            build_similar_filespec(moved_to_spec, 0);  
            strcat(moved_to_spec, "_");  
            strcat(moved_to_spec, dname);  
            strcat(moved_to_spec, despooled_ext);  
            WRITE_S(despooled_file_spec);  
            WRITE_S(moved_to_spec);  
        /*  Warning!  The following will loop continuously if the queue is down 里海译:警告！如果队列故障，以下内容将无限循环。 */  
            while (!file_exists(despooled_file_spec)) continue;  
            UF_CALL(uc4567(despooled_file_spec, moved_to_spec,  
                UF_CFI_MOVE_ALWAYS_REPLACE, 0, 0));  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 错误处理：通过report_error函数封装了错误处理流程，包括打印错误信息到系统日志和打开列表窗口。
> 2. 遍历部件中的图纸：通过ask_next_drawing函数可以遍历当前部件中的所有图纸。
> 3. 构建文件路径：通过build_similar_filespec和file_exists函数构建和检查文件路径。
> 4. 写入列表窗口：通过write_string_to_listing_window函数向列表窗口写入信息。
> 5. 绘图和重命名：do_it函数实现了绘制所有图纸，并将生成的绘图文件重命名和移动到指定目录。
> 6. NX初始化和结束：ufusr函数在NX初始化后执行绘图和重命名操作，并在结束时关闭NX。
> 7. 加载部件：主函数中处理命令行传入的部件路径，逐个加载并绘制每个部件的图纸。
> 8. 加载状态检查：check_load_status函数检查部件加载状态，并打印出加载失败的部件信息。
>
> 综上所述，这段代码利用NX提供的二次开发接口，实现了在NX中遍历和绘制所有图纸，并将生成的绘图文件重命名和移动到指定目录的功能。
>
