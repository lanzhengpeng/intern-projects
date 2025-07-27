### 【0644】create closed spline thru selected points 创建通过选定点的封闭样条线

#### 代码

```cpp
    /*HEAD CREATE_CLOSED_SPLINE_THRU_SELECTED_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中的新增功能。 */  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n,  
            resp;  
        tag_t  
            spline;  
        UF_UI_chained_points_t  
            *points;  
        UF_CURVE_pt_slope_crvatr_t  
            *point_data;  
        double  
            *spts;  
        while (!UF_CALL(UF_UI_select_point_collection("Select points", FALSE,  
               &points, &n, &resp)) && (resp == UF_UI_OK))  
        {  
            UF_CALL(allocate_memory(n * sizeof(UF_CURVE_pt_slope_crvatr_t),  
                (void *) &point_data));  
            UF_CALL(allocate_memory(3 * n * sizeof(double), (void *)&spts));  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_VEC3_copy(points[ii].pt, point_data[ii].point);  
                spts[ii*3] = point_data[ii].point[0];  
                spts[(ii*3)+1] = point_data[ii].point[1];  
                spts[(ii*3)+2] = point_data[ii].point[2];  
                point_data[ii].slope_type = UF_CURVE_SLOPE_NONE;  
                point_data[ii].crvatr_type = UF_CURVE_CRVATR_NONE;  
            }  
            UF_CALL(UF_CURVE_create_spline_thru_pts(3, 1, n, point_data, NULL,  
                FALSE, &spline));  
            UF_free(point_data);  
            UF_free(points);  
            UF_free(spts);  
        }  
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

> 这段代码是一个NX Open C++应用程序，主要功能是创建一个通过一系列点生成的封闭样条曲线。
>
> 关键功能包括：
>
> 1. 错误报告函数：用于在函数调用失败时报告错误。
> 2. 内存分配函数：用于分配内存空间。
> 3. 主要功能函数：通过循环调用UF_UI_select_point_collection()函数，让用户选择点集，然后调用UF_CURVE_create_spline_thru_pts()函数创建封闭样条曲线。
> 4. ufusr函数：程序入口点，用于初始化和终止NX Open。
> 5. 卸载函数：用于卸载应用程序。
>
> 在创建样条曲线时，代码首先获取用户选择的点集，然后分配内存以存储点和斜率信息。接着，它将这些点传递给UF_CURVE_create_spline_thru_pts()函数，以创建封闭样条曲线。最后，释放内存，并重复循环以让用户继续选择点集。
>
> 代码结构清晰，注释详细，便于理解。它实现了选择点集创建封闭样条曲线的功能。
>
