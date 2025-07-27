### 【0620】create bsurf thru points from file 从文件创建通过点的B样条曲面

#### 代码

```cpp
    /*HEAD CREATE_BSURF_THRU_POINTS_FROM_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中的新功能。 */  
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
    #define NUM_ROWS 4  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_points_from_file(char *fspec, double (**points)[3])  
    {  
        int  
            ii = 0,  
            n_points = 0;  
        char  
            a_line[MAX_LINE_SIZE+1];  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL) n_points++;  
            rewind(the_file);  
            UF_CALL(allocate_memory(n_points * sizeof(double[3]), (void **)points));  
            for (ii = 0; ii < n_points; ii++)  
            {  
                fscanf(the_file, "%lf, %lf, %lf,",  
                    &(*points)[ii][0], &(*points)[ii][1], &(*points)[ii][2]);  
            }  
            fclose(the_file);  
        }  
        return n_points;  
    }  
    static logical prompt_for_point_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[UF_CFI_MAX_PATH_NAME_SIZE] = { "*.sur" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Point File", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            n,  
            ii,  
            indx = 0,  
            jj,  
            kk,  
            pts_per_row[NUM_ROWS] = {23, 23, 23, 23};  
        tag_t  
            bsurf;  
        double  
            (*points)[3];  
        UF_MODL_bsurf_row_info_t  
            row_info[NUM_ROWS];  
        UF_MODL_bsurface_t  
            bsurf_data;  
        char  
            fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        if (prompt_for_point_file_name("From Point File", fspec) &&  
            ((n = ask_points_from_file(fspec, &points)) > 0))  
        {  
            /* allocate and load point and other information for each row of points 译:为每一行的点分配和加载点和其他信息。 */  
            indx = 0;  
            for (ii = 0; ii < NUM_ROWS; ii++)  
            {  
                row_info[ii].num_points = pts_per_row[ii];  
                row_info[ii].points = (double *)malloc( pts_per_row[ii] * 3 *  
                                               sizeof(double) );  
                row_info[ii].weight = (double *)malloc( pts_per_row[ii] *  
                                               sizeof(double) );  
                /* load up point and weight info 译:装载点和重量信息 */  
                for (jj=0, kk = 0; kk<pts_per_row[ii]; kk++, jj+=3, indx++)  
                {  
                    printf("Row: %d, pt[%d]: %f, %f, %f\n", ii, kk,  
                        points[indx][0], points[indx][1], points[indx][2]);  
                    row_info[ii].points[jj] = points[indx][0];  
                    row_info[ii].points[jj+1] = points[indx][1];  
                    row_info[ii].points[jj+2] = points[indx][2];  
                    row_info[ii].weight[kk] = 1.0;  
                }  
                printf("ii: %d\n", ii);  
            }  
        }  
        UF_CALL(UF_MODL_create_bsurf_thru_pts(1, 0, 0, 3, 3, NUM_ROWS,  
            row_info, &bsurf));  
        UF_CALL(UF_VIEW_fit_view(NULL_TAG, .9));  
        ECHO(bsurf);  
        /* free allocated memory 译:已分配的空闲内存 */  
        for (ii=0; ii<NUM_ROWS; ii++)  
        {  
            free(row_info[ii].points);  
            free(row_info[ii].weight);  
        }  
        UF_CALL(UF_MODL_ask_bsurf(bsurf, &bsurf_data ));  
        ECHO(bsurf_data.num_poles_u);  
        ECHO(bsurf_data.num_poles_v);  
        ECHO(bsurf_data.order_u);  
        ECHO(bsurf_data.order_v);  
        ECHO(bsurf_data.is_rational);  
        UF_CALL(UF_MODL_free_bsurf_data(&bsurf_data));  
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

```

#### 代码解析

> 这段代码是一个NX二次开发示例，其主要功能是从文件中读取点数据，并使用这些点创建一个B样条曲面。
>
> 主要步骤包括：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于打印错误信息。
> 2. 内存分配函数：定义了一个内存分配函数allocate_memory，用于从NX内存池中分配内存。
> 3. 从文件读取点数据：定义了一个函数ask_points_from_file，用于从文件中读取点的坐标数据。
> 4. 提示输入文件名：定义了一个函数prompt_for_point_file_name，用于提示用户输入点文件名。
> 5. 主函数：定义了一个主函数do_it，用于执行以下步骤：提示用户输入点文件，并读取文件中的点数据为每一行点数据分配内存，并加载点坐标和权重信息使用这些点创建一个B样条曲面调整视图以显示整个曲面打印曲面的基本属性释放分配的内存
> 6. 提示用户输入点文件，并读取文件中的点数据
> 7. 为每一行点数据分配内存，并加载点坐标和权重信息
> 8. 使用这些点创建一个B样条曲面
> 9. 调整视图以显示整个曲面
> 10. 打印曲面的基本属性
> 11. 释放分配的内存
> 12. UFusr函数：定义了UFusr函数，用于在NX中运行do_it函数。
> 13. 卸载函数：定义了一个卸载函数，用于在卸载用户函数时立即释放内存。
>
> 总体来说，这段代码通过读取文件中的点数据，并使用这些点创建一个B样条曲面，展示了NX的B样条曲面创建接口的使用方法。
>
