### 【0622】create bsurface thru points 通过点创建曲面

#### 代码

```cpp
    /*HEAD CREATE_BSURFACE_THRU_POINTS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。 */  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            indx,  
            jj,  
            kk,  
            pts_per_row[NUM_ROWS] = {4, 4, 4, 4};  
        tag_t  
            bsurf;  
        double  
            points[16][3] = {  3.5, 2.5, 0.0,  
                               4.0, 2.6, 0.2,  
                               4.5, 2.6, 0.2,  
                               5.0, 2.5, 0.0,  
                               3.3, 3.0, 0.2,  
                               4.1, 3.2, 0.3,  
                               4.5, 3.2, 0.3,  
                               5.0, 3.0, 0.2,  
                               3.0, 3.5, 0.2,  
                               3.8, 3.6, 0.3,  
                               4.5, 3.6, 0.3,  
                               5.0, 3.5, 0.2,  
                               2.6, 4.0, 0.0,  
                               3.3, 4.0, 0.2,  
                               4.5, 4.0, 0.2,  
                               5.0, 4.0, 0.0 };  
        UF_MODL_bsurf_row_info_t  
            row_info[NUM_ROWS];  
        UF_MODL_bsurface_t  
            bsurf_data;  
        /* allocate and load point and other information for each row of points 译:分配和加载每行点的点和其他信息 */  
        indx = 0;  
        for (ii = 0; ii < NUM_ROWS; ii++)  
        {  
            row_info[ii].num_points = pts_per_row[ii];  
            row_info[ii].points = (double *)malloc( pts_per_row[ii] * 3 *  
                                           sizeof(double) );  
            row_info[ii].weight = (double *)malloc( pts_per_row[ii] *  
                                           sizeof(double) );  
            /* load up point and weight info 译:载入点和重量信息 */  
            for (jj=0, kk = 0; kk<pts_per_row[ii]; kk++, jj+=3, indx++)  
            {  
                row_info[ii].points[jj] = points[indx][0];  
                row_info[ii].points[jj+1] = points[indx][1];  
                row_info[ii].points[jj+2] = points[indx][2];  
                row_info[ii].weight[kk] = 1.0;  
            }  
        }  
        UF_CALL(UF_MODL_create_bsurf_thru_pts(1, 0, 0, 3, 3, NUM_ROWS,  
            row_info, &bsurf));  
        UF_CALL(UF_VIEW_fit_view(NULL_TAG, .9));  
        ECHO(bsurf);  
        /* free allocated memory 译:释放已分配的内存 */  
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

> 这是段NX二次开发代码，主要实现了以下功能：
>
> 1. 定义了报告错误的宏函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了创建B样条曲面的函数do_it，该函数首先定义了4行共16个点的坐标，然后依次为每行点分配内存，并加载点的坐标和权重信息。接着调用UF_MODL_create_bsurf_thru_pts函数通过点创建B样条曲面，并获取曲面信息，包括控制点数量、阶数和是否为有理B样条等。最后释放内存。
> 3. 定义了ufusr函数作为NX的回调函数，在该函数中初始化NX环境，调用do_it函数，然后终止NX环境。
> 4. 定义了ufusr_ask_unload函数，返回立即卸载标志。
> 5. 使用了NX提供的UF函数接口，如UF_MODL_create_bsurf_thru_pts、UF_MODL_ask_bsurf等，用于创建和管理B样条曲面。
> 6. 使用了宏定义和C语言标准库函数，如malloc、free、printf等，用于内存分配、释放和打印输出。
> 7. 使用了错误报告机制，在函数调用失败时能够输出错误信息。
> 8. 实现了从点到B样条曲面的创建过程，展示了NX二次开发的基本流程。
> 9. 注释使用中文，便于理解代码的功能。
>
> 综上所述，这段代码通过调用NX提供的UF函数接口，实现了从点到B样条曲面的创建过程，并展示了NX二次开发的基本流程。
>
