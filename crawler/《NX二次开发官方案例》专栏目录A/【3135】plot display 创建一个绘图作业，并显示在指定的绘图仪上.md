### 【3135】plot display 创建一个绘图作业，并显示在指定的绘图仪上

#### 代码

```cpp
    /*HEAD PLOT_DISPLAY CCC UFUN */  
    /* This sample program is valid for PRE-NX3 versions only. 里海译:这个示例程序只适用于PRE-NX3版本。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_plot.h>  
    #include <uf_view.h>  
    #include <uf_undo.h>  
    #include <uf_csys.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog是在V18版本中新增的函数。在回答问题时，请只提供翻译内容，不要添加任何其他无关的内容。 */  
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
    static void set_wcs_to_view(char *view_name)  
    {  
        tag_t  
            matobj,                     /* Matrix object 里海译:矩阵对象（Matrix object） */  
            csys;                       /* Csys object  里海译:Csys 对象是一个用于定义坐标系的对象。它包含以下属性：

- Name: 坐标系名称
- active: 是否为当前坐标系
- isdatum: 是否为基准坐标系
- origin: 坐标原点
- xdir: X轴方向向量
- ydir: Y轴方向向量
- zdir: Z轴方向向量
- angle: Z轴与X轴之间的角度
- local: 是否为局部坐标系
- global: 是否为全局坐标系
- orienttype: 坐标系方向类型
- coordtype: 坐标系类型
- color: 坐标系颜色
- comment: 坐标系注释

这些属性用于描述和定义坐标系的位置、方向、类型等。 */  
        double  
            origin[3],                  /* View's center 里海译:翻译为：视图的中心 */  
            matrix[9],                  /* View's matrix 里海译:翻译View的矩阵 */  
            scale;  
        UF_CALL(uc6430(view_name, origin, &scale));  
        UF_CALL(uc6433(view_name, matrix));  
        if (!UF_CALL( UF_CSYS_create_matrix(matrix, &matobj) ) &&  
            !UF_CALL( UF_CSYS_create_temp_csys(origin, matobj, &csys) ))  
            UF_CALL( UF_CSYS_set_wcs(csys) );  
    }  
    static void ask_view_lower_left_corner(double lower_left[3])  
    {  
    /* This function demonstrates how to determine the current view's lower  
       left corner taking into account any zooming or panning the user may  
       have done.  A GRIP program is used to create a point at the view's  
       lower left corner.  
       This is the GRIP code used:  
            entity/ pt, tpt(2), tln(2)  
            ufargs/ pt  
            tpt(1) = point/0,0,0  
            tln(1) = line/ tpt(1), atangl, 90  
            tpt(2) = point/ ENDOF, YSMALL, tln(1)  
            tln(2) = line/ tpt(2), atangl, 0  
            pt = point/ ENDOF, xsmall, tln(2)  
            delete/ tpt,tln  
            halt  
    */  
        tag_t  
            point;  
        UF_args_t  
            arg;  
        UF_UNDO_mark_id_t  
            mark;  
        arg.type    = UF_TYPE_TAG_T;  
        arg.length  = 1;  
        arg.address = &point;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark));  
        set_wcs_to_view("");  
    #ifdef _WIN32  
        UF_CALL(UF_call_grip("n:\\grip\\get_view_lower_left.grx", 1, &arg));  
    #else  
        UF_CALL(UF_call_grip("grip/get_view_lower_left.grx", 1, &arg));  
    #endif  
        UF_CALL(UF_CURVE_ask_point_data(point, lower_left));  
    /*  The UNDO puts the WCS back where it was and deletes the GRIP point 里海译:UNDO命令将WCS恢复到原来的位置并删除GRIP点。 */  
        UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
    }  
    static void do_it(void)  
    {  
        int  
            jobid;  
        char  
            bannerMsg[132+1] = { "" },  
            jobname[16+1] = { "test" },  
            node[132+1] = { "www-gtac" },  
            pauseMsg[44+1] = { "" },  
            plotter[16+1] = { "GTACHP5MX" };  
        UF_PLOT_options_t  
            opts =  
            {  
                1.0, 0.0,               /* scale and rotation 里海译:缩放和旋转 */  
                { 0.0, 0.0, 0.0 },      /* origin 里海译:origin的中文翻译为“起源”。 */  
                { 0.0, 0.0 },           /* offset 里海译:offset的翻译是“偏移量”。 */  
                "", "", "", "", "",     /* temporary pointers 里海译:临时指针 */  
                FALSE,                  /* print banner? 里海译:print banner的中文翻译是打印横幅。 */  
                UF_PLOT_PLOT_RELEASE,   /* plot option 里海译:好的，我翻译一下plot option：

plot option：绘图选项 */  
                1,                      /* media number 里海译:媒体编号 */  
                1,                      /* copies 里海译:翻译：副本 */  
                1,                      /* pen type ; 1=width 2=color 里海译:笔的类型；1=宽度 2=颜色 */  
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }  
            };  
    /*  Assign the pointers to point to the allocated space 里海译:分配指针以指向分配的空间 */  
        opts.plotter = plotter;  
        opts.node = node;  
        opts.jobname = jobname;  
        opts.pauseMsg = pauseMsg;  
        opts.bannerMsg = bannerMsg;  
    /*  get the lower left corner of the display for the origin 里海译:将显示屏左下角作为原点。 */  
        ask_view_lower_left_corner(opts.origin);  
        UF_CALL(UF_PLOT_display(&opts, &jobid));  
        printf("JOBID = %d\n", jobid);  
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

> 这是一段NX的二次开发代码，主要包括以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印函数调用失败的错误信息。
> 2. 定义了一个设置WCS到视图坐标系的函数set_wcs_to_view，用于根据视图名称将WCS设置为该视图的坐标系。
> 3. 定义了一个获取视图左下角坐标的函数ask_view_lower_left_corner，通过调用GRIP程序来确定当前视图的左下角坐标。
> 4. 定义了一个主函数do_it，设置打印选项，获取视图左下角坐标作为打印原点，然后调用UF_PLOT_display接口进行打印。
> 5. 定义了ufusr函数，用于在NX内嵌模式下执行打印。
> 6. 定义了主函数main，首先调用UF_initialize初始化NX API，然后循环打开传入的零件文件，对每个零件调用do_it函数进行打印，最后调用UF_terminate结束。
> 7. 定义了check_load_status函数，用于检查零件加载状态，并打印加载失败的零件和错误信息。
>
> 这段代码实现了根据传入的零件文件列表，依次打开零件并打印的功能，考虑了错误处理、坐标系设置、视图坐标获取等细节，是一个完整的NX二次开发打印示例。
>
